#include "pieces/pawn.hpp"
#include "constants.hpp"
#include "legalMove.hpp"
#include "board.hpp"
#include "identifier.hpp"

std::vector<LegalMove> Pawn::getPseudoMoves(int col, int row, int piece, Board &board)
{

    int color = Identifier::getTeam(piece);
    std::vector<LegalMove> moves;

    auto boardState = board.getSquares();
    auto from = std::make_tuple(col, row);
    if (boardState[row + color][col] == constants::EMPTY)
    {
        moves.emplace_back(std::make_tuple(col, row + color), from, piece, boardState[row + col][col]); // foward
    }

    if (col + 1 < 8 and boardState[row + color][col + 1] != constants::EMPTY and Identifier::getTeam(boardState[row + color][col + 1]) != color)
    {
        moves.emplace_back(std::make_tuple(col + 1, row + color), from, piece, boardState[row + color][col + 1]); // attack right
    }
    if (col - 1 >= 0 and boardState[row + color][col - 1] != constants::EMPTY and Identifier::getTeam(boardState[row + color][col - 1]) != color)
    {
        moves.emplace_back(std::make_tuple(col - 1, row + color), from, piece, boardState[row + color][col - 1]); // attack left
    }
    return moves;
}