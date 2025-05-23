#include "pieces/rook.hpp"
#include <vector>
#include "constants.hpp"
#include "identifier.hpp"
#include <array>

std::vector<LegalMove> Rook::getPseudoMoves(int col, int row, int piece, std::array<std::array<int, 8U>, 8U> &boardState)
{
    std::vector<LegalMove> moves;
    int color = Identifier::getTeam(piece);
    const std::vector<std::pair<int, int>> directions = {
        {0, -1},
        {-1, 0},
        {0, 1},
        {1, 0}};
    auto from = std::make_tuple(col, row);

    for (auto [dCol, dRow] : directions)
    {
        int tempCol = col + dCol;
        int tempRow = row + dRow;
        // left down
        while (tempCol >= 0 and tempRow >= 0 and tempCol < 8 and tempRow < 8)
        {
            if (boardState[tempRow][tempCol] != constants::EMPTY) // piece found
            {
                if (Identifier::getTeam(boardState[tempRow][tempCol]) == color) // dont add my team
                {
                    break;
                }
                else
                {
                    moves.emplace_back(std::make_tuple(tempCol, tempRow), from, piece, boardState[tempRow][tempCol]); // add other team
                    break;
                }
            }
            moves.emplace_back(std::make_tuple(tempCol, tempRow), from, piece, boardState[tempRow][tempCol]);
            tempCol += dCol;
            tempRow += dRow;
        }
    }

    return moves;
}