#pragma once
#include "legalMove.hpp"
#include "board.hpp"
#include "theoryEvaluator.hpp"
#include <unordered_map>
#include "compactMove.hpp"
#include <chrono>
#include <shared_mutex>
enum BoundFlag : uint8_t
{
    EXACT,
    LOWERBOUND,
    UPPERBOUND
};

struct TTEntry
{
    double value;
    int depth;
    BoundFlag flag;
    LegalMove bestMove;
};

class MinMaxTree
{
public:
    MinMaxTree();
    LegalMove getBestMove(Board &board, int color, std::atomic<bool> &killThread, std::chrono::duration<double> time, int startDepth = 2);
    LegalMove lookIntoFutureMoves(Board &board, int color, int depth, double alpha, double beta, std::chrono::duration<double> maxTimeMs, std::atomic<bool> &kill, int startDepth = 2);

private:
    LegalMove evaluateMoves(std::vector<LegalMove> &moves, Board board, int color, int maxDepth, std::chrono::duration<double> maxTimeMs, std::atomic<bool> &kill);

    std::shared_mutex ttMutex;
    bool timeUp = false;
    std::chrono::_V2::system_clock::time_point start;
    std::unordered_map<uint64_t, TTEntry> transpositionTable;
};