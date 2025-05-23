#include "pieces/king.hpp"
#include "constants.hpp"
#include "identifier.hpp"
#include <array>

std::vector<LegalMove> King::getPseudoMoves(int col, int row, int piece, Board &board)
{
    const std::vector<std::tuple<int, int>> directions = {{-1, -1}, {-1, 0}, {-1, 1}, {0, 1}, {0, -1}, {1, 1}, {1, 0}, {1, -1}};
    int color = Identifier::getTeam(piece);
    auto boardState = board.getSquares();
    std::vector<LegalMove> pseudoMoves;
    auto from = std::make_tuple(col, row);

    for (auto &direction : directions)
    {
        auto [dCol, dRow] = direction;
        int tempCol = dCol + col;
        int tempRow = dRow + row;
        if (Identifier::inBounds(tempCol) and Identifier::inBounds(tempRow))
        {
            if (boardState[tempRow][tempCol] == constants::EMPTY or Identifier::getTeam(boardState[row][col]) != color)
            {
                pseudoMoves.emplace_back(std::make_tuple(tempCol, tempRow), from, piece, boardState[tempRow][tempCol]);
            }
        }
    }
    return pseudoMoves;
}