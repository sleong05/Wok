#pragma once
#include <array>
#include "board.hpp"
#include "theoryEvaluator.hpp"

class SBAnalyzer
{
public:
    static double evaluateBoard(const Board &board);
    static double getPieceValue(int piece, int col, int row, const std::array<std::array<int, 8U>, 8U> &boardState, const std::array<std::array<bool, 8U>, 8U> &moveState);
};