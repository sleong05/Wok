#include "pawn.hpp"
#include "constants.hpp"
#include "pseudoMove.hpp"
#include "board.hpp"

std::vector<PseudoMove> Pawn::getPseudoMoves(int col, int row, int color, Board &board)
{

    int piece = (color == constants::WHITE) ? constants::WHITE_PAWN : constants::BLACK_PAWN;
    std::vector<PseudoMove> moves;

    auto from = std::make_tuple(col, row);
    moves.emplace_back(std::make_tuple(col, row + color), from, piece); // foward
    auto boardState = board.getSquares();

    if (boardState[row + color][col + 1] != constants::EMPTY and Identifier::getTeam(boardState[row + color][col + 1]) != color)
    {
        moves.emplace_back(std::make_tuple(col + 1, row + color), from, piece); // attack right
    }
    if (boardState[row + color][col - 1] != constants::EMPTY and Identifier::getTeam(boardState[row + color][col - 1]) != color)
    {
        moves.emplace_back(std::make_tuple(col - 1, row + color), from, piece); // attack left
    }
    return moves;
}