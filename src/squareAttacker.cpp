#include "squareAttacker.hpp"
#include <identifier.hpp>

using namespace constants;
// under attack from the opposite color
bool squareAttacker::isSquareUnderAttack(int col, int row, int color, const std::array<std::array<int, 8>, 8> &boardState)
{
    bool underAttack = checkKnightAttack(color, col, row, boardState);
    if (underAttack)
        return true;

    // diagonals
    underAttack = checkDiagonals(color, col, row, boardState);
    if (underAttack)
        return true;

    // horozontals
    underAttack = checkHorizontals(color, col, row, boardState);
    if (underAttack)
        return true;
    // pawn
    underAttack = checkPawnAttack(row, color, col, boardState);
    if (underAttack)
        return true;

    // king
    underAttack = checkKingAttack(color, col, row, boardState);
    if (underAttack)
        return true;

    return false;
}

bool squareAttacker::checkHorizontals(int color, int col, int row, const std::array<std::array<int, 8U>, 8U> &boardState)
{
    int enemyRook = (color == WHITE) ? BLACK_ROOK : WHITE_ROOK;
    int enemyQueen = (color == WHITE) ? BLACK_QUEEN : WHITE_QUEEN;

    for (auto [dCol, dRow] : constants::ROOK_OFFSETS)
    {
        int tempCol = col + dCol;
        int tempRow = row + dRow;

        while (tempCol >= 0 and tempRow >= 0 and tempCol < 8 and tempRow < 8)
        {
            if (boardState[tempRow][tempCol] != constants::EMPTY) // piece found
            {
                if (Identifier::getTeam(boardState[tempRow][tempCol]) == color)
                {
                    break;
                }
                else
                {
                    if (boardState[tempRow][tempCol] == enemyRook or boardState[tempRow][tempCol] == enemyQueen)
                    {
                        return true;
                    }
                    break;
                }
            }
            tempCol += dCol;
            tempRow += dRow;
        }
    }
    return false;
}

bool squareAttacker::checkDiagonals(int color, int col, int row, const std::array<std::array<int, 8U>, 8U> &boardState)
{
    int enemyBishop = (color == WHITE) ? BLACK_BISHOP : WHITE_BISHOP;
    int enemyQueen = (color == WHITE) ? BLACK_QUEEN : WHITE_QUEEN;

    for (auto [dCol, dRow] : constants::BISHOP_OFFSETS)
    {
        int tempCol = col + dCol;
        int tempRow = row + dRow;

        while (tempCol >= 0 and tempRow >= 0 and tempCol < 8 and tempRow < 8)
        {
            if (boardState[tempRow][tempCol] != constants::EMPTY) // piece found
            {
                if (Identifier::getTeam(boardState[tempRow][tempCol]) == color)
                {
                    break;
                }
                else
                {
                    if (boardState[tempRow][tempCol] == enemyBishop or boardState[tempRow][tempCol] == enemyQueen)
                    {
                        return true;
                    }
                    break;
                }
            }
            tempCol += dCol;
            tempRow += dRow;
        }
    }
    return false;
}

bool squareAttacker::checkKnightAttack(int color, int col, int row, const std::array<std::array<int, 8U>, 8U> &boardState)
{
    int enemyKnight = (color == WHITE) ? BLACK_KNIGHT : WHITE_KNIGHT;
    for (auto &direction : KNIGHT_OFFSETS)
    {
        auto [tempCol, tempRow] = direction;
        tempCol += col;
        tempRow += row;
        if (Identifier::inBounds(tempCol) and Identifier::inBounds(tempRow))
        {
            if (boardState[tempRow][tempCol] == enemyKnight)
            {
                return true;
            }
        }
    }
    return false;
}

bool squareAttacker::checkKingAttack(int color, int col, int row, const std::array<std::array<int, 8U>, 8U> &boardState)
{
    int enemyKing = (color == WHITE) ? BLACK_KING : WHITE_KING;
    for (auto &direction : KING_OFFSETS)
    {
        auto [tempCol, tempRow] = direction;
        tempCol += col;
        tempRow += row;
        if (Identifier::inBounds(tempCol) and Identifier::inBounds(tempRow))
        {
            if (boardState[tempRow][tempCol] == enemyKing)
            {
                return true;
            }
        }
    }
    return false;
}

bool squareAttacker::checkPawnAttack(int row, int color, int col, const std::array<std::array<int, 8U>, 8U> &boardState)
{
    int enemyPawn = (color == WHITE) ? BLACK_PAWN : WHITE_PAWN;
    if (Identifier::inBounds(row + color))
    {
        if ((Identifier::inBounds(col - 1) and boardState[row + color][col - 1] == enemyPawn))
        {
            return true;
        }
        if (Identifier::inBounds(col + 1) and boardState[row + color][col + 1] == enemyPawn)
        {
            return true;
        }
    }
    return false;
}
