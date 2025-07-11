#pragma once
#include "legalMove.hpp"
#include <vector>
#include "board.hpp"
class King
{
public:
    static std::vector<LegalMove> getPseudoMoves(int col, int row, int piece, Board &board, bool capturesOnly = false);
    static std::vector<LegalMove> checkCastling(std::array<std::array<bool, 8U>, 8U> &moveState, std::array<std::array<int, 8U>, 8U> &boardState, int color, const std::tuple<int, int> &from);
};