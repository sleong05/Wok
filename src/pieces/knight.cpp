#include "pieces/knight.hpp"
#include "legalMove.hpp"
#include "constants.hpp"
#include <array>
#include "identifier.hpp"

std::vector<LegalMove> Knight::getPseudoMoves(int col, int row, int piece, std::array<std::array<int, 8U>, 8U> &boardState)
{
    std::tuple<int, int> from = {col, row};
    int color = Identifier::getTeam(piece);
    std::vector<LegalMove> pseudoMoves;

    const std::vector<std::pair<int, int>> offsets = {
        {2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2}};

    for (auto [dc, dr] : offsets)
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