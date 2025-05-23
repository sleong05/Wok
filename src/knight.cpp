#include "knight.hpp"
#include "pseudoMove.hpp"
#include "constants.hpp"
#include <array>
#include "identifier.hpp"

std::vector<PseudoMove> Knight::getPseudoMoves(int col, int row, int color, std::array<std::array<int, 8U>, 8U> boardState)
{
    std::tuple<int, int> from = {col, row};
    int piece = (color == constants::WHITE) ? constants::WHITE_KNIGHT : constants::BLACK_KNIGHT;
    std::vector<PseudoMove> pseudoMoves;

    const std::vector<std::pair<int, int>> offsets = {
        {2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2}};

    for (auto [dc, dr] : offsets)
    {
        int newCol = col + dc;
        int newRow = row + dr;

        if (newCol >= 0 && newCol < 8 && newRow >= 0 && newRow < 8)
        {
            if (boardState[newRow][newCol] == constants::EMPTY or Identifier::getTeam(boardState[newRow][newCol]) != color)
            {
                pseudoMoves.emplace_back(std::make_tuple(newCol, newRow), from, piece);
            }
        }
    }

    return pseudoMoves;
}