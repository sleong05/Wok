#include "chengine/theoryEvaluator.hpp"
#include "identifier.hpp"
#include "weights.hpp"
#include "pieces/bishop.hpp"
#include "pieces/rook.hpp"
#include "chengine/theoryEvaluator.hpp"

using namespace weights;

double TheoryEvaluator::getPawnValue(int col, int row, const std::array<std::array<int, 8u>, 8u> &boardState)
{
    int color = Identifier::getTeam(boardState[row][col]);
    int myPawn = (color == constants::WHITE) ? constants::WHITE_PAWN : constants::BLACK_PAWN;
    double value = 0;
    // check left chain
    if (col != 0)
    {
        if (boardState[row + color][col - 1] == myPawn)
            value += CHAIN_VALUE;
    }
    if (col != 7)
    {
        if (boardState[row + color][col + 1] == myPawn)
            value += CHAIN_VALUE;
    }

    value += passPawnValue(color, row, col, boardState);
    return value;
}

double TheoryEvaluator::passPawnValue(int color, int row, int col, const std::array<std::array<int, 8U>, 8U> &boardState)
{
    // passed pawn logic
    int promotionDistance = (color == constants::WHITE) ? row : 7 - row;
    int enemyPawn = (color == constants::WHITE) ? constants::BLACK_PAWN : constants::WHITE_PAWN;
    // see if pawn is a passed pawn
    int colStart = (col != 0) ? col - 1 : col;
    int colEnd = (col != 7) ? col + 1 : col;

    int rowStart = (color == constants::WHITE) ? 1 : 6; // start from opposite side (more likely to hit a pawn)

    for (int tempRow = rowStart; tempRow != row; tempRow -= color) // going backwards from the oponents side
    {
        for (int tempCol = colStart; tempCol <= colEnd; tempCol++)
        {
            if (boardState[tempRow][tempCol] == enemyPawn)
            {
                return 0.0;
            }
        }
    }
    return PASSED_PAWN * (7 - promotionDistance);
}

double TheoryEvaluator::getKnightValue(int col, int row)
{
    return knightValues[row][col];
}

double TheoryEvaluator::getRookValue(int col, int row, const std::array<std::array<int, 8u>, 8u> &boardState)
{
    return rookValues[row][col] + Rook::getNumberOfMoves(col, row, boardState) / 30.0;
}

double TheoryEvaluator::getQueenValue(int col, int row, const std::array<std::array<int, 8u>, 8u> &boardState)
{
    return (Rook::getNumberOfMoves(col, row, boardState) + Bishop::getNumberOfMoves(col, row, boardState)) / 56.0;
}

double TheoryEvaluator::getKingValue(int col, int row, const std::array<std::array<int, 8u>, 8u> &boardState, const std::array<std::array<bool, 8u>, 8u> &moveState)
{
    double value = 0;
    // castling
    value += kingValues[row][col]; // ADJUST FOR MID GAME

    // check if king is safe
    int color = Identifier::getTeam(boardState[row][col]);
    int myPawn = (color == constants::WHITE) ? constants::WHITE_PAWN : constants::BLACK_PAWN;
    // checks 2x3 infront of king for pawns and is happy when they exist
    if (row + color > 7 or row + color < 0)
        return value;
    for (int i = -1; i < 2; i++)
    {
        if (col + i != 3 or col + i != 4)
        {
            if ((col + i >= 0 and col + i < 8) and (boardState[row + color][col + i] == myPawn or ((row + color * 2 < 7 and row + color * 2 >= 0) and boardState[row + color * 2][col + i] == myPawn)))
            {
                value += PAWN_INFRONT_OF_KING;
            }
        }
    }
    return value;
}

double TheoryEvaluator::getBishopValue(int col, int row, const std::array<std::array<int, 8u>, 8u> &boardState)
{
    return Bishop::getNumberOfMoves(col, row, boardState) / 26.0;
}
