#pragma once
#include <array>
#include "board.hpp"
#include "theoryEvaluator.hpp"

class SBAnalyzer
{
public:
    static double evaluateBoard(const Board &board);
    static double getPieceValue(int piece, int col, int row, const Board &board);
};