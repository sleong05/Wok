#pragma once
#include "legalMove.hpp"
#include "board.hpp"
#include "theoryEvaluator.hpp"
#include <unordered_map>
#include "compactMove.hpp"
class MinMaxTree
{
public:
    MinMaxTree(Board &board);
    LegalMove getBestMove(int color);
    LegalMove lookIntoFutureMoves(int color, int depth, double alpha, double beta);

private:
    std::unordered_map<uint64_t, std::vector<CompactMove>> moveCache;
    Board &board;
};