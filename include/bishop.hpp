#pragma once
#include <pseudoMove.hpp>
#include <vector>

class Bishop
{
public:
    static std::vector<PseudoMove> getPseudoMoves(int col, int row, int color, std::array<std::array<int, 8U>, 8U> boardState);
};
