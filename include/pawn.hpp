#include <pseudoMove.hpp>
#include <identifier.hpp>
#include <constants.hpp>
#include <vector>
class Pawn
{
public:
    std::vector<PseudoMove> inline getPsuedoMoves(int col, int row, int color);
};