#pragma once
#include "legalMove.hpp"
#include "board.hpp"
#include "theoryEvaluator.hpp"
#include <unordered_map>
#include "compactMove.hpp"
#include <chrono>

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
    MinMaxTree(Board &board);
    LegalMove getBestMove(int color);
    LegalMove lookIntoFutureMoves(int color, int depth, double alpha, double beta, int maxDepth);

private:
    bool timeUp = false;
    int currentAge = 0;
    std::chrono::_V2::system_clock::time_point start;
    std::chrono::duration<double> maxTimeMs = std::chrono::seconds(3);
    std::unordered_map<uint64_t, TTEntry> transpositionTable;
    Board &board;
};