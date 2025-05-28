#pragma once
#include "legalMove.hpp"
#include "board.hpp"
#include "theoryEvaluator.hpp"

class MinMaxTree
{
public:
    MinMaxTree(Board &board);
    LegalMove getBestMove(int color);
    LegalMove lookIntoFutureMoves(int color, int depth, double alpha, double beta);

private:
    Board &board;
};