#include "bishop.hpp"
#include "constants.hpp"
#include <array>
#include "identifier.hpp"
std::vector<PseudoMove> Bishop::getPseudoMoves(int col, int row, int color, std::array<std::array<int, 8U>, 8U> boardState)
{
    std::vector<PseudoMove> moves;
    int piece = (color == constants::WHITE) ? constants::WHITE_BISHOP : constants::BLACK_BISHOP;
    const std::vector<std::pair<int, int>> directions = {
        {-1, -1}, // up-left
        {1, -1},  // up-right
        {-1, 1},  // down-left
        {1, 1}    // down-right
    };
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
                    moves.emplace_back(std::make_tuple(tempCol, tempRow), from, piece); // add other team
                    break;
                }
            }
            moves.emplace_back(std::make_tuple(tempCol, tempRow), from, piece);
            tempCol += dCol;
            tempRow += dRow;
        }
    }

    return moves;
}