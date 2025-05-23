#ifndef KNIGHT_HPP
#define KNIGHT_HPP

#include <pseudoMove.hpp>
#include <vector>

class Knight
{
public:
    static std::vector<PseudoMove> getPseudoMoves(int col, int row, int color, std::array<std::array<int, 8U>, 8U> boardState);
};
#endif