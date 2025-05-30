#include "pieces/pawn.hpp"
#include "constants.hpp"
#include "legalMove.hpp"
#include "board.hpp"
#include "identifier.hpp"
#include <cstdlib>

std::vector<LegalMove> Pawn::getPseudoMoves(int col, int row, int piece, Board &board)
{

    int color = Identifier::getTeam(piece);
    std::vector<LegalMove> moves;

    auto boardState = board.getSquares();
    auto from = std::make_tuple(col, row);
    int promotionRow = (color == constants::WHITE) ? 0 : 7;

    int oppositePawn = (piece == constants::WHITE_PAWN) ? constants::BLACK_PAWN : constants::WHITE_PAWN;
    // en passant
    LegalMove lastMove = board.getLastMove();

    if (lastMove.from != constants::NO_TILE_SELECTED)
    {
        if (lastMove.pieceToMove == oppositePawn)
        {
            auto [newCol, newRow] = lastMove.to;
            auto [oldCol, oldRow] = lastMove.from;

            if (std::abs(newRow - oldRow) == 2 and newRow == row and (newCol - 1 == col or newCol + 1 == col)) // last move was a pawn move that moved 2 and pawn is in position
            {
                LegalMove enPassant = LegalMove(std::make_tuple(newCol, newRow + color), from, piece, boardState[newRow + color][newCol]);
                enPassant.isEnPassant = true;
                moves.push_back(enPassant);
            }
        }
    }
    // move foward and jump foward
    if (boardState[row + color][col] == constants::EMPTY)
    {
        if (row + color == promotionRow)
        {
            LegalMove promotion = LegalMove(std::make_tuple(col, row + color), from, piece, boardState[row + color][col]);
            promotion.isPromotion = true;
            moves.push_back(promotion);
        }
        else
        {
            moves.emplace_back(std::make_tuple(col, row + color), from, piece, boardState[row + color][col]); // foward
        }

        int startingRow = (color == constants::WHITE) ? 6 : 1;
        if (boardState[row + color * 2][col] == constants::EMPTY and row == startingRow)
        {
            moves.emplace_back(std::make_tuple(col, row + color * 2), from, piece, boardState[row + color * 2][col]); // jump foward
        }
    }

    // capture
    if (col + 1 < 8 and boardState[row + color][col + 1] != constants::EMPTY and Identifier::getTeam(boardState[row + color][col + 1]) != color)
    {
        if (row + color == promotionRow)
        {
            LegalMove promotion = LegalMove(std::make_tuple(col + 1, row + color), from, piece, boardState[row + color][col + 1]);
            promotion.isPromotion = true;
            moves.push_back(promotion);
        }
        else
        {
            moves.emplace_back(std::make_tuple(col + 1, row + color), from, piece, boardState[row + color][col + 1]); // attack right
        }
    }
    if (col - 1 >= 0 and boardState[row + color][col - 1] != constants::EMPTY and Identifier::getTeam(boardState[row + color][col - 1]) != color)
    {
        if (row + color == promotionRow)
        {
            LegalMove promotion = LegalMove(std::make_tuple(col - 1, row + color), from, piece, boardState[row + color][col - 1]);
            promotion.isPromotion = true;
            moves.push_back(promotion);
        }
        else
        {
            moves.emplace_back(std::make_tuple(col - 1, row + color), from, piece, boardState[row + color][col - 1]); // attack left
        }
    }
    return moves;
}