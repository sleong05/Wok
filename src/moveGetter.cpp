#include "moveGetter.hpp"
#include "constants.hpp"
#include "knight.hpp"
#include <board.hpp>
#include <iostream>
#include "pawn.hpp"
#include "bishop.hpp"

std::vector<PseudoMove> MoveGetter::getMovesFromPieceAt(int col, int row, Board &board)
{
    std::cout << "Getting Moves" << std::endl;
    auto boardState = board.getSquares();
    int piece = boardState[row][col];

    switch (piece)
    {
    case constants::WHITE_PAWN:
    {
        auto pseudoMoves = Pawn::getPseudoMoves(col, row, constants::WHITE, board);
        return pseudoMoves;
    }
    case constants::WHITE_KNIGHT:
    {
        auto pseudoMoves = Knight::getPseudoMoves(col, row, constants::WHITE, boardState);
        return pseudoMoves;
    }
    case constants::WHITE_BISHOP:
    {
        auto pseudoMoves = Bishop::getPseudoMoves(col, row, constants::WHITE, boardState);
        return pseudoMoves;
    }
    case constants::WHITE_ROOK:
        // Handle white rook
    case constants::WHITE_QUEEN:
        // Handle white queen
    case constants::WHITE_KING:
        // Handle white king

    case constants::BLACK_PAWN:
    {
        auto pseudoMoves = Pawn::getPseudoMoves(col, row, constants::BLACK, board);
        return pseudoMoves;
    }
    case constants::BLACK_KNIGHT:
    {
        auto pseudoMoves = Knight::getPseudoMoves(col, row, constants::BLACK, boardState);
        return pseudoMoves;
    }
    case constants::BLACK_BISHOP:
    {
        auto pseudoMoves = Bishop::getPseudoMoves(col, row, constants::BLACK, boardState);
        return pseudoMoves;
    }
        // Handle black bishop
    case constants::BLACK_ROOK:
        // Handle black rook
    case constants::BLACK_QUEEN:
        // Handle black queen
    case constants::BLACK_KING:
        // Handle black king

    default:
        return {};
    }
}

std::vector<PseudoMove> MoveGetter::getMovesForTeam(int color)
{
    return std::vector<PseudoMove>();
}
