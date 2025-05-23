#include "moveGetter.hpp"
#include "constants.hpp"
#include <board.hpp>
#include <iostream>
#include "allPieces.hpp"
std::vector<LegalMove> MoveGetter::getMovesFromPieceAt(int col, int row, Board &board)
{
    std::cout << "Getting Moves" << std::endl;
    auto boardState = board.getSquares();
    int piece = boardState[row][col];

    switch (piece)
    {
    case constants::WHITE_PAWN:
    {
        auto pseudoMoves = Pawn::getPseudoMoves(col, row, constants::WHITE_PAWN, board);
        return pseudoMoves;
    }
    case constants::WHITE_KNIGHT:
    {
        auto pseudoMoves = Knight::getPseudoMoves(col, row, constants::WHITE_KNIGHT, boardState);
        return pseudoMoves;
    }
    case constants::WHITE_BISHOP:
    {
        auto pseudoMoves = Bishop::getPseudoMoves(col, row, constants::WHITE_BISHOP, boardState);
        return pseudoMoves;
    }
    case constants::WHITE_ROOK:
    {
        auto pseudoMoves = Rook::getPseudoMoves(col, row, constants::WHITE_ROOK, boardState);
        return pseudoMoves;
    }
    case constants::WHITE_QUEEN:
    {
        auto pseudoMoves = Queen::getPseudoMoves(col, row, constants::WHITE_QUEEN, boardState);
        return pseudoMoves;
    }
    case constants::WHITE_KING:
    {
        auto pseudoMoves = King::getPseudoMoves(col, row, constants::WHITE_KING, board);
        return pseudoMoves;
    }

    case constants::BLACK_PAWN:
    {
        auto pseudoMoves = Pawn::getPseudoMoves(col, row, constants::BLACK_PAWN, board);
        return pseudoMoves;
    }
    case constants::BLACK_KNIGHT:
    {
        auto pseudoMoves = Knight::getPseudoMoves(col, row, constants::BLACK_KNIGHT, boardState);
        return pseudoMoves;
    }
    case constants::BLACK_BISHOP:
    {
        auto pseudoMoves = Bishop::getPseudoMoves(col, row, constants::BLACK_BISHOP, boardState);
        return pseudoMoves;
    }
    case constants::BLACK_ROOK:
    {
        auto pseudoMoves = Rook::getPseudoMoves(col, row, constants::BLACK_ROOK, boardState);
        return pseudoMoves;
    }
    case constants::BLACK_QUEEN:
    {
        auto pseudoMoves = Queen::getPseudoMoves(col, row, constants::BLACK_QUEEN, boardState);
        return pseudoMoves;
    }
    case constants::BLACK_KING:
    {
        auto pseudoMoves = King::getPseudoMoves(col, row, constants::BLACK_KING, board);
        return pseudoMoves;
    }

    default:
        std::cout << "default case return empty";
        return {};
    }
}

std::vector<LegalMove> MoveGetter::getMovesForTeam(int color)
{
    return std::vector<LegalMove>();
}
