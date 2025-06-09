#pragma once
#include "legalMove.hpp"
#include "board.hpp"
#include "theoryEvaluator.hpp"
#include <unordered_map>
#include "compactMove.hpp"

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
    LegalMove lookIntoFutureMoves(int color, int depth, double alpha, double beta);

private:
    std::unordered_map<uint64_t, TTEntry> transpositionTable;
    Board &board;
};