#ifndef KNIGHT_HPP
#define KNIGHT_HPP

#include <legalMove.hpp>
#include <vector>
#include <array>
class Knight
{
public:
    static std::vector<LegalMove> getPseudoMoves(int col, int row, int piece, std::array<std::array<int, 8U>, 8U> &boardState, std::array<std::array<bool, 8U>, 8U> &moveState);
};
#endif