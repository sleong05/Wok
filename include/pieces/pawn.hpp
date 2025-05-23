#pragma once
#include "legalMove.hpp"
#include <vector>
#include "board.hpp"
class Pawn
{
public:
    static std::vector<LegalMove> getPseudoMoves(int col, int row, int piece, Board &board);
};