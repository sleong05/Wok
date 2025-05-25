#pragma once
#include "legalMove.hpp"
#include <vector>
#include "board.hpp"
class King
{
public:
    static std::vector<LegalMove> getPseudoMoves(int col, int row, int piece, Board &board);

private:
    void static checkCastling(Board &board, int col, int row, std::array<std::array<bool, 8U>, 8U> &moveState, std::array<std::array<int, 8U>, 8U> &boardState, int color, const std::tuple<int, int> &from, int piece, std::vector<LegalMove> &pseudoMoves);
};