#ifndef MOVEGETTER_HPP
#define MOVEGETTER_HPP
#include "legalMove.hpp"
#include <vector>
#include "board.hpp"

class MoveGetter
{
public:
    static std::vector<LegalMove> getMovesFromPieceAt(int x, int y, Board &board, bool capturesOnly = false);
    static std::vector<LegalMove> getMovesForTeam(int color, Board &board, bool capturesOnly = false);
    static bool hasMoveLeft(int color, Board &board);
};
#endif