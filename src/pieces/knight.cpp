#include "pieces/knight.hpp"
#include "legalMove.hpp"
#include "constants.hpp"
#include <array>
#include "identifier.hpp"
#include <tuple>
std::vector<LegalMove> Knight::getPseudoMoves(int col, int row, int piece, std::array<std::array<int, 8U>, 8U> &boardState, std::array<std::array<bool, 8U>, 8U> &moveState)
{
    std::tuple<int, int> from = {col, row};
    int color = Identifier::getTeam(piece);
    std::vector<LegalMove> pseudoMoves;

    for (auto [dc, dr] : constants::KNIGHT_OFFSETS)
    {
        int newCol = col + dc;
        int newRow = row + dr;

        if (newCol >= 0 && newCol < 8 && newRow >= 0 && newRow < 8)
        {
            if (newRow >= 0 and newRow < 8 and newCol < 8 and newCol >= 0 and (boardState[newRow][newCol] == constants::EMPTY or Identifier::getTeam(boardState[newRow][newCol]) != color))
            {
                pseudoMoves.emplace_back(std::make_tuple(newCol, newRow), from, piece, boardState[newRow][newCol]);
            }
        }
    }

    return pseudoMoves;
}