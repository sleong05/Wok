#include "chengine/minMaxTree.hpp"
#include "chengine/compactMove.hpp"
#include "chengine/staticBoardAnalyzer.hpp"
#include "legalMove.hpp"
#include "board.hpp"
#include "moveGetter.hpp"
#include "weights.hpp"
#include "constants.hpp"
#include <identifier.hpp>
#include <algorithm>
#include <string>
#include <fstream>
#include <limits>
#include <chrono>
#include <random>
using namespace constants;
MinMaxTree::MinMaxTree(Board &board) : board(board)
{
}

LegalMove MinMaxTree::getBestMove(int color) // chengine is black so make them alkways look for the lowest value move
{
    transpositionTable.clear();
    double INF = 1000000000.0;
    // std::ofstream clearLog("move_evaluations.log", std::ios::trunc);
    // clearLog.close();
    auto start = std::chrono::high_resolution_clock::now();

    auto blackMoves = MoveGetter::getMovesForTeam(BLACK, board);
    auto whiteMoves = MoveGetter::getMovesForTeam(WHITE, board);

    int totalMoves = blackMoves.size() + whiteMoves.size();
    std::cout << "Number of available moves = " << totalMoves << std::endl;

    LegalMove bestMove = lookIntoFutureMoves(color, 1, -INF, INF);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "-------------------------------------------- \n";
    std::cout << "lookIntoFutureMoves took " << duration.count() << " seconds\n";

    return bestMove;
};

LegalMove MinMaxTree::lookIntoFutureMoves(int color, int depth, double alpha, double beta)
{
    double originalAlpha = alpha;
    double originalBeta = beta;

    //      -----------------------------------------BASE CASES--------------------------------------------------
    if (not MoveGetter::hasMoveLeft(color, board))
    {
        double value = board.isKingInCheck(color) ? (weights::MATE - (depth * 100)) * color : weights::DRAW;

        LegalMove dummyMove = LegalMove();
        dummyMove.value = value;
        return dummyMove;
    }

    // base case depth hit
    if (depth == weights::MAX_DEPTH)
    {
        double value = SBAnalyzer::evaluateBoard(board);
        LegalMove dummyMove = LegalMove();
        dummyMove.value = value;
        return dummyMove;
    }
    // -----------------------------------------RECURSIVE CASES--------------------------------------------------
    // use transpositon table if already calcualated
    uint64_t hash = board.getHash();
    auto ttIt = transpositionTable.find(hash);
    if (ttIt != transpositionTable.end() && ttIt->second.depth >= depth)
    {
        const TTEntry &entry = ttIt->second;

        if (entry.flag == EXACT)
        {
            return entry.bestMove;
        }
        if (entry.flag == LOWERBOUND && entry.value >= beta)
        {
            return entry.bestMove;
        }
        if (entry.flag == UPPERBOUND && entry.value <= alpha)
        {
            return entry.bestMove;
        }
    }
    // generate and evalaute all moves
    std::vector<LegalMove> allMoves = MoveGetter::getMovesForTeam(color, board);

    if (depth <= 5)
    {
        for (auto &move : allMoves)
        {
            move.computePriority();
        }

        std::stable_sort(allMoves.begin(), allMoves.end(), [](const LegalMove &a, const LegalMove &b)
                         { return a.priorityOfSearchValue > b.priorityOfSearchValue; });
    }
    LegalMove bestMove;
    double INF = std::numeric_limits<double>::infinity();
    bestMove.value = (color == WHITE) ? -INF : INF;

    for (auto &move : allMoves)
    {
        // cache
        uint64_t hashBefore = board.getHash();
        board.doMove(move);

        move.value = lookIntoFutureMoves(color * -1, depth + 1, alpha, beta).value;

        board.undoMove(move);
        uint64_t hashAfter = board.getHash();
        if (hashBefore != hashAfter)
        {
            std::cerr << "Zobrist hash mismatch after undo!\n";
            std::abort();
        }
        // update bestMove
        if (color == WHITE)
        {
            if (move.value > bestMove.value)
            {
                bestMove = move;
            }

            alpha = std::max(alpha, move.value);
            if (alpha >= beta)
            {
                break;
            }
        }
        else if (color == BLACK)
        {
            if (move.value < bestMove.value)
            {
                bestMove = move;
            }

            beta = std::min(beta, move.value);

            if (alpha >= beta)
            {
                break;
            }
        }
    }
    // store result in TT
    BoundFlag flag = EXACT;
    if (bestMove.value <= originalAlpha)
        flag = UPPERBOUND;
    else if (bestMove.value >= originalBeta)
        flag = LOWERBOUND;

    TTEntry entry = {
        .value = bestMove.value,
        .depth = depth,
        .flag = flag,
        .bestMove = bestMove};
    transpositionTable[hash] = entry;

    return bestMove;
}
