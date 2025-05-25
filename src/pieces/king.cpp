#include "pieces/king.hpp"
#include "squareAttacker.hpp"
#include "constants.hpp"
#include "identifier.hpp"
#include <array>
#include "pieces/king.hpp"

std::vector<LegalMove> King::getPseudoMoves(int col, int row, int piece, Board &board)
{
    int color = Identifier::getTeam(piece);
    auto boardState = board.getSquares();
    std::vector<LegalMove> pseudoMoves;
    auto from = std::make_tuple(col, row);
    // castling
    checkCastling(board, col, row, boardState, color, from, piece, pseudoMoves);
    // normal moves
    for (auto &direction : constants::KING_OFFSETS)
    {
        auto [dCol, dRow] = direction;
        int tempCol = dCol + col;
        int tempRow = dRow + row;
        if (Identifier::inBounds(tempCol) and Identifier::inBounds(tempRow))
        {
            if (boardState[tempRow][tempCol] == constants::EMPTY or Identifier::getTeam(boardState[tempRow][tempCol]) != color)
            {
                pseudoMoves.emplace_back(std::make_tuple(tempCol, tempRow), from, piece, boardState[tempRow][tempCol]);
            }
        }
    }
    return pseudoMoves;
}

void King::checkCastling(Board &board, int col, int row, std::array<std::array<int, 8U>, 8U> &boardState, int color, const std::tuple<int, int> &from, int piece, std::vector<LegalMove> &pseudoMoves)
{
    int startingCol = 4;
    int startingRow = (Identifier::getTeam(piece) == constants::WHITE) ? 7 : 0;
    if (not board.hasMoved(col, row) and startingCol == col and startingRow == row)
    {
        // short castle
        if (not board.hasMoved(7, row)) // check if rook has moved
        {
            if (boardState[row][6] == constants::EMPTY and boardState[row][5] == constants::EMPTY)
            {
                // check if safe
                bool castleSpotsSafe = true;
                for (int i = 4; i < 7; i++)
                {

                    if (squareAttacker::isSquareUnderAttack(i, row, color, boardState))
                    {
                        castleSpotsSafe = false;
                    }
                }
                if (castleSpotsSafe)
                {
                    LegalMove shortCastle = LegalMove(std::make_tuple(6, startingRow), from, piece, constants::EMPTY);
                    shortCastle.isCastle = true;
                    shortCastle.additionalPieceHasMoved = false;
                    pseudoMoves.push_back(shortCastle);
                }
            }
        }
        // long castle
        if (not board.hasMoved(0, row)) // check if rook has moved
        {
            if (boardState[row][1] == constants::EMPTY and boardState[row][2] == constants::EMPTY and boardState[row][3] == constants::EMPTY)
            {
                // check if safe
                bool castleSpotsSafe = true;
                for (int i = 2; i < 5; i++)
                {

                    if (squareAttacker::isSquareUnderAttack(i, row, color, boardState))
                    {
                        castleSpotsSafe = false;
                    }
                }
                if (castleSpotsSafe)
                {
                    LegalMove shortCastle = LegalMove(std::make_tuple(2, startingRow), from, piece, constants::EMPTY);
                    shortCastle.isCastle = true;
                    shortCastle.additionalPieceHasMoved = false;
                    pseudoMoves.push_back(shortCastle);
                }
            }
        }
    }
}