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
    LegalMove getBestMove(Board &board, int color);
    LegalMove lookIntoFutureMoves(Board &board, int color, int depth, double alpha, double beta, int maxDepth);

private:
    LegalMove evaluateMoves(std::vector<LegalMove> &moves, Board board, int color, int maxDepth);

    std::shared_mutex ttMutex;
    bool timeUp = false;
    std::chrono::_V2::system_clock::time_point start;
    std::chrono::duration<double> maxTimeMs = std::chrono::seconds(5);
    std::unordered_map<uint64_t, TTEntry> transpositionTable;
};