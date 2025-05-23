#ifndef MOVEGETTER_HPP
#define MOVEGETTER_HPP
#include "pseudoMove.hpp"
#include <vector>
#include "board.hpp"

class MoveGetter
{
public:
    static std::vector<PseudoMove> getMovesFromPieceAt(int x, int y, Board &board);
    static std::vector<PseudoMove> getMovesForTeam(int color);
};
#endif