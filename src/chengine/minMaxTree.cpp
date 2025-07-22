#include "chengine/minMaxTree.hpp"
#include "chengine/compactMove.hpp"
#include "chengine/staticBoardAnalyzer.hpp"
#include "legalMove.hpp"
#include "board.hpp"
#include "moveGetter.hpp"
#include "weights.hpp"
#include "constants.hpp"
#include "chengine/quiesceSearch.hpp"
#include <identifier.hpp>
#include <algorithm>
#include <string>
#include <fstream>
#include <limits>
#include <chrono>
#include <random>
#include <thread>
#include <shared_mutex>
#include <bits/stdc++.h>
using namespace constants;
using namespace std::chrono;

constexpr double INF = 1000000000.0;
MinMaxTree::MinMaxTree()
{
}

LegalMove MinMaxTree::getBestMove(Board &board, int color) // chengine is black so make them alkways look for the lowest value move
{
    // double INF = 1000000000.0;
    // start = high_resolution_clock::now();
    // timeUp = false;

    // LegalMove bestMove;
    // int maxDepth = 2;

    // // iterative deepening
    // int maxThreads = std::thread::hardware_concurrency();
    // if (maxThreads == 0)
    //     maxThreads = 4;
    // maxThreads = std::min(maxThreads, 1);
    // std::cout << "Threads used: " << maxThreads << std::endl;
    // while (true)
    // {
    //     auto now = high_resolution_clock::now();
    //     duration<double> duration = now - start;

    //     if (duration > maxTimeMs or timeUp)
    //         break;

    //     // get moves
    //     std::vector<LegalMove> legalMoves = MoveGetter::getMovesForTeam(color, board);
    //     if (legalMoves.empty())
    //         break;

    //     // distrubte moves into buckets for threads to look through
    //     int threadCount = std::min((int)legalMoves.size(), maxThreads);
    //     std::vector<std::vector<LegalMove>> buckets(threadCount);
    //     for (int i = 0; i < int(legalMoves.size()); ++i)
    //         buckets[i % threadCount].push_back(legalMoves[i]);

    //     std::mutex bestMutex;
    //     double bestEval = (color == WHITE) ? -INF : INF;
    //     LegalMove bestMoveAtDepth;

    //     std::vector<std::thread> threads;
    //     for (int t = 0; t < threadCount; ++t)
    //     {
    //         threads.emplace_back(
    //             [&, t]()
    //             {
    //                 for (auto move : buckets[t])
    //                 {
    //                     auto now = std::chrono::high_resolution_clock::now();
    //                     if (now - start > maxTimeMs || timeUp)
    //                         return;
    //                     Board boardCopy = board;
    //                     boardCopy.doMove(move);
    //                     double eval = lookIntoFutureMoves(boardCopy, color, 1, -INF, INF, maxDepth).value;
    //                     std::cout << eval << move << std::endl;
    //                     boardCopy.undoMove(move);

    //                     std::lock_guard<std::mutex> lock(bestMutex);
    //                     if ((color == WHITE && eval > bestEval) || (color == BLACK && eval < bestEval))
    //                     {
    //                         bestEval = eval;
    //                         bestMoveAtDepth = move;
    //                         move.value = eval;
    //                     }
    //                 }
    //             });
    //     }

    //     for (auto &t : threads)
    //         t.join();

    //     if (duration > maxTimeMs or timeUp)
    //         break;
    //     bestMove = bestMoveAtDepth;

    //     std::cout << "at " << maxDepth << " best move is " << bestMove << std::endl;
    //     maxDepth++;
    // }
    // std::cout << "at " << maxDepth << " best move is " << bestMove << std::endl;
    // return bestMove;

    start = high_resolution_clock::now();

    int maxThreads = 8;
    // get moves
    std::vector<LegalMove> legalMoves = MoveGetter::getMovesForTeam(color, board);

    // split the moves into buckets based on thread count
    int threadCount = std::min((int)legalMoves.size(), maxThreads);

    std::vector<std::vector<LegalMove>> buckets(threadCount);
    for (int i = 0; i < int(legalMoves.size()); ++i)
        buckets[i % threadCount].push_back(legalMoves[i]);

    LegalMove bestMove;
    timeUp = false;
    int maxDepth = 2;
    // iterative deepening
    while (true)
    {
        auto now = high_resolution_clock::now();
        duration<double> duration = now - start;

        if (duration > maxTimeMs)
            break;

        std::vector<LegalMove> threadResults(threadCount);
        std::vector<std::thread> threads;

        // split work into threads
        for (int i = 0; i < threadCount; i++)
        {
            threads.emplace_back([&, i]()
                                 { threadResults[i] = evaluateMoves(buckets[i], board, color, maxDepth); });
        }
        // join threads
        for (auto &t : threads)
            t.join();
        // choose best move
        LegalMove bestMoveAtDepth;
        double bestEval = (color == WHITE) ? -INF : INF;

        for (auto &move : threadResults)
        {

            bool isBetter = (color == WHITE) ? move.value > bestEval : move.value < bestEval;
            if (isBetter)
            {
                bestEval = move.value;
                bestMoveAtDepth = move;
            }

            if (timeUp)
                break;
        }
        if (timeUp)
            break;
        bestMove = bestMoveAtDepth;
        std::cout << "at " << maxDepth << " best move is " << bestMove << std::endl;
        maxDepth++;
    }

    return bestMove;
};

LegalMove MinMaxTree::evaluateMoves(std::vector<LegalMove> &bucket, Board boardCopy, int color, int maxDepth)
{
    double bestEval = (color == WHITE) ? -INF : INF;
    LegalMove bestMove;
    for (auto &move : bucket)
    {
        boardCopy.doMove(move);

        move.value = lookIntoFutureMoves(boardCopy, color * -1, 1, -INF, INF, maxDepth).value;

        // std::cout << move << "\n";
        bool isBetter = (color == WHITE) ? move.value > bestEval : move.value < bestEval;
        if (isBetter)
        {
            bestEval = move.value;
            bestMove = move;
        }

        if (timeUp)
            break;

        boardCopy.undoMove(move);
    }

    return bestMove;
}

LegalMove MinMaxTree::lookIntoFutureMoves(Board &board, int color, int depth, double alpha, double beta, int maxDepth)
{
    // if we are out of time return and stop recursion

    auto now = high_resolution_clock::now();

    // std::cout << "Checking time elapsed at depth " << depth << std::endl;

    duration<double> duration = now - start;
    if (duration > maxTimeMs)
    {
        timeUp = true;
        return LegalMove();
    }

    double originalAlpha = alpha;
    double originalBeta = beta;

    //      -----------------------------------------BASE CASES--------------------------------------------------
    if (not MoveGetter::hasMoveLeft(color, board))
    {
        // std::cout << "mate base casse at " << depth << std::endl;
        double value = board.isKingInCheck(color) ? (weights::MATE - (depth * 100)) * color : weights::DRAW;

        LegalMove dummyMove = LegalMove();
        dummyMove.value = value;
        return dummyMove;
    }

    // base case depth hit
    if (depth == maxDepth)
    {
        // std::cout << "entering quiesesearch at depth " << depth << std::endl;
        return quiesceSearch(board, color, alpha, beta, depth);
    }
    // -----------------------------------------RECURSIVE CASES--------------------------------------------------
    // use transpositon table if already calcualated
    bool hasTTEntry = false;
    TTEntry entry;
    uint64_t hash = board.getHash();
    std::unordered_map<uint64_t, TTEntry>::iterator ttIt;
    {
        std::shared_lock<std::shared_mutex> readLock(ttMutex);
        ttIt = transpositionTable.find(hash);
        if (ttIt != transpositionTable.end() && ttIt->second.depth >= maxDepth - depth)
        {
            hasTTEntry = true;
            entry = ttIt->second;
        }
    }
    if (hasTTEntry)
    {
        // std::cout << "ttentry found " << std::endl;
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
    // std::cout << "ttentry not found " << std::endl;
    //  generate and evalaute all moves
    std::vector<LegalMove> allMoves = MoveGetter::getMovesForTeam(color, board);
    bool isBestMove = false;

    for (auto &move : allMoves)
    {
        isBestMove = false;
        if (hasTTEntry && ttIt->second.bestMove == move)
            isBestMove = true;
        move.computePriority(isBestMove);
    }

    std::sort(allMoves.begin(), allMoves.end(), [](const LegalMove &a, const LegalMove &b)
              { return a.priorityOfSearchValue > b.priorityOfSearchValue; });

    LegalMove bestMove;
    double INF = std::numeric_limits<double>::infinity();
    bestMove.value = (color == WHITE) ? -INF : INF;

    for (auto &move : allMoves)
    {
        // cache
        uint64_t hashBefore = board.getHash();
        board.doMove(move);
        // check for three fold repition
        if (board.isThreefoldRepetition())
        {
            LegalMove dummyMove;
            dummyMove.value = 0;
            continue;
        }
        move.value = lookIntoFutureMoves(board, color * -1, depth + 1, alpha, beta, maxDepth).value;
        // std::cout << "Looked into future moves. bestmove value is " << move.value << std::endl;
        board.undoMove(move);
        uint64_t hashAfter = board.getHash();
        if (hashBefore != hashAfter)
        {
            std::cerr << "Zobrist hash mismatch after undo!\n";
        }
        // std::cout << "alpha beta cutoffs begin" << std::endl;
        //  update bestMove
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
    // std::cout << "alpha beta cutoffsend" << std::endl;
    // std::cout << "transpo table start" << std::endl;
    if (depth <= 5)
    {
        std::unique_lock<std::shared_mutex> writeLock(ttMutex);
        auto hashedMove = transpositionTable.find(hash);
        if (hashedMove == transpositionTable.end() || hashedMove->second.depth < maxDepth - depth)
        {
            // store result in TT
            BoundFlag flag = EXACT;
            if (bestMove.value <= originalAlpha)
                flag = UPPERBOUND;
            else if (bestMove.value >= originalBeta)
                flag = LOWERBOUND;

            TTEntry entry = {
                .value = bestMove.value,
                .depth = maxDepth - depth,
                .flag = flag,
                .bestMove = bestMove,
            };
            transpositionTable[hash] = entry;
        }
    }
    // std::cout << "transpo table end" << std::endl;
    return bestMove;
}
