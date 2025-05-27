#pragma once
#include <array>
#include "board.hpp"
class SBAnalyzer
{
public:
    static double evaluateBoard(const Board &board);
    static double getPieceValue(int piece);
};