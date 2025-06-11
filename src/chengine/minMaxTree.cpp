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

static inline double estimateTime(double permutations) {
    return .005 * pow(permutations/(pow(10, 8)), 3);
}

LegalMove MinMaxTree::getBestMove(int color) // chengine is black so make them alkways look for the lowest value move
{
   currentAge++;

if (transpositionTable.size() > 500000) {
    int checked = 0;
    for (auto it = transpositionTable.begin(); it != transpositionTable.end() && checked < 1000; ) {
        if (currentAge - it->second.age > 8) {
            it = transpositionTable.erase(it);
        } else {
            ++it;
        }
        ++checked;
    }
}
    double INF = 1000000000.0;
    // std::ofstream clearLog("move_evaluations.log", std::ios::trunc);
    // clearLog.close();
    auto start = std::chrono::high_resolution_clock::now();

    auto blackMoves = MoveGetter::getMovesForTeam(BLACK, board);
    auto whiteMoves = MoveGetter::getMovesForTeam(WHITE, board);

    // std::cout << "Number of black moves = " << blackMoves.size() << std::endl;
    // std::cout << "Number of white moves = " << whiteMoves.size() << std::endl;
    double permutations = std::pow(whiteMoves.size(), 3) * std::pow(blackMoves.size(), 3);
    std::cout << "estimated permutations = " <<  permutations << std::endl;
    MAX_DEPTH = 7;
    if (permutations < 50000000) {
        std::cout << "DEPTH NINE TRIGGERED" <<std::endl;
        MAX_DEPTH = 9;
    }
    if (permutations < 500000) {
        std::cout << "DEPTH ELEVEN TRIGGERED" <<std::endl;
        MAX_DEPTH = 9;
    }
    LegalMove bestMove = lookIntoFutureMoves(color, 1, -INF, INF);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "lookIntoFutureMoves took " << duration.count() << " seconds\n";
 std::cout << "-------------------------------------------- \n";
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
    if (depth == MAX_DEPTH)
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
    if (depth <= 5) {
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
        .bestMove = bestMove,
        .age = currentAge, 
    };
    transpositionTable[hash] = entry;
    }
    return bestMove;
}
