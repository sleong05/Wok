#include <pseudoMove.hpp>
#include <identifier.hpp>
#include <constants.hpp>
#include <vector>
#include "board.hpp"
class Pawn
{
public:
    static std::vector<PseudoMove> getPseudoMoves(int col, int row, int color, Board &board);
};