#pragma once
#include "legalMove.hpp"
#include <vector>
#include <array>
class Queen
{
public:
    static std::vector<LegalMove> getPseudoMoves(int col, int row, int piece, std::array<std::array<int, 8U>, 8U> &boardState);
};