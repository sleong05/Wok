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

LegalMove MinMaxTree::getBestMove(Board &board, int color, std::atomic<bool> &kill, std::chrono::duration<double> maxTimeMs, int startDepth) // chengine is black so make them alkways look for the lowest value move
{
    start = high_resolution_clock::now();

    unsigned int totalThreads = std::thread::hardware_concurrency();
    int maxThreads = (totalThreads > 1) ? totalThreads - 2 : 1;
    // get moves
    std::vector<LegalMove> legalMoves = MoveGetter::getMovesForTeam(color, board);
    if (legalMoves.size() == 0)
    {
        LegalMove gameOver = LegalMove();
        gameOver.value = 0;
        gameOver.from = constants::NO_TILE_SELECTED;
        return gameOver;
    }
    // split the moves into buckets based on thread count
    int threadCount = std::min((int)legalMoves.size(), maxThreads);

    std::vector<std::vector<LegalMove>> buckets(threadCount);
    for (int i = 0; i < int(legalMoves.size()); ++i)
        buckets[i % threadCount].push_back(legalMoves[i]);

    LegalMove bestMove;
    timeUp = false;
    int maxDepth = startDepth;
    // iterative deepening
    while (!timeUp or !kill.load())
    {
        auto now = high_resolution_clock::now();
        duration<double> duration = now - start;

        if (duration > maxTimeMs or kill.load())
            break;

        std::vector<LegalMove> threadResults(threadCount);
        std::vector<std::thread> threads;

        // split work into threads
        for (int i = 0; i < threadCount; i++)
        {
            threads.emplace_back([&, i]()
                                 { threadResults[i] = evaluateMoves(buckets[i], board, color, maxDepth, maxTimeMs, kill); });
        }
        // join threads
        for (auto &t : threads)
        {
            t.join();
        }
        // choose best move
        LegalMove bestMoveAtDepth;
        double bestEval = (color == WHITE) ? -INF : INF;
        bestMoveAtDepth.value = bestEval;

        for (auto &move : threadResults)
        {

            bool isBetter = (color == WHITE) ? move.value > bestEval : move.value < bestEval;
            if (isBetter)
            {
                bestEval = move.value;
                bestMoveAtDepth = move;
            }

            if (timeUp or kill.load())
                break;
        }
        if (timeUp or kill.load())
            break;
        bestMove = bestMoveAtDepth;
        std::cout << "at " << maxDepth + 1 << " best move is " << bestMove << std::endl;
        maxDepth++;
    }
    bestMove.valueDepth = maxDepth;
    return bestMove;
};

LegalMove MinMaxTree::evaluateMoves(std::vector<LegalMove> &bucket, Board boardCopy, int color, int maxDepth, std::chrono::duration<double> maxTimeMs, std::atomic<bool> &kill)
{
    double bestEval = (color == WHITE) ? -INF : INF;
    LegalMove bestMove;
    for (auto &move : bucket)
    {
        boardCopy.doMove(move);

        move.value = lookIntoFutureMoves(boardCopy, color * -1, 1, -INF, INF, maxTimeMs, kill, maxDepth).value;

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

LegalMove MinMaxTree::lookIntoFutureMoves(Board &board, int color, int depth, double alpha, double beta, std::chrono::duration<double> maxTimeMs, std::atomic<bool> &kill, int maxDepth)
{
    // if we are out of time return and stop recursion
    bool zobristMismatch = false;
    auto now = high_resolution_clock::now();

    duration<double> duration = now - start;
    if (duration > maxTimeMs or kill.load())
    {
        timeUp = true;
        return LegalMove();
    }

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
    if (depth == maxDepth)
    {
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
            std::cout << "Threefold detected \n";
            LegalMove dummyMove;
            dummyMove.value = 0;
        }
        else
        {
            move.value = lookIntoFutureMoves(board, color * -1, depth + 1, alpha, beta, maxTimeMs, kill, maxDepth).value;
        }

        board.undoMove(move);
        uint64_t hashAfter = board.getHash();
        if (hashBefore != hashAfter)
        {
            zobristMismatch = true;
            std::cerr << "Zobrist hash mismatch after undo!\n";
        }
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
    if (depth <= 5 and !zobristMismatch)
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
    return bestMove;
}
