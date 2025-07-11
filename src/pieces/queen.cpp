#include "pieces/queen.hpp"
#include "pieces/rook.hpp"
#include "pieces/bishop.hpp"
#include <array>
#include "identifier.hpp"
std::vector<LegalMove> Queen::getPseudoMoves(int col, int row, int piece, std::array<std::array<int, 8U>, 8U> &boardState, bool capturesOnly)
{
    auto rookMoves = Rook::getPseudoMoves(col, row, piece, boardState, capturesOnly);
    auto bishopMoves = Bishop::getPseudoMoves(col, row, piece, boardState, capturesOnly);
    rookMoves.insert(rookMoves.end(), bishopMoves.begin(), bishopMoves.end());

    return rookMoves;
}