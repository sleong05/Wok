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
    // std::ofstream logFile("move_evaluations.log", std::ios::app); // logger
    //      -----------------------------------------BASE CASES--------------------------------------------------
    //      base case mate/draw
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
    // check cache for data
    std::vector<LegalMove> allMoves;
    std::vector<CompactMove> compactMoves;
    bool cached = false;

    auto it = moveCache.find(board.getHash());
    if (it != moveCache.end())
    {
        cached = true;
        allMoves.reserve(it->second.size());

        for (const auto &cm : it->second)
        {
            allMoves.push_back(toLegalMove(cm));
        }
    }
    else
    {
        allMoves = MoveGetter::getMovesForTeam(color, board);
        // cache
        compactMoves.reserve(allMoves.size());

        for (auto &move : allMoves)
        {
            compactMoves.push_back(toCompactMove(move));
        }
    }
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
    if (not cached)
        moveCache[board.getHash()] = std::move(compactMoves);
    return bestMove;
}
