#include <SFML/Graphics.hpp>
#include "identifier.hpp"
#include "board.hpp"
#include "constants.hpp"
#include "moveGetter.hpp"

using namespace constants;

Board::Board()
{
    initilizeBoard();
}

void Board::initilizeBoard()
{
    whitePositions = {{7, 7}, {7, 6}, {7, 5}, {7, 4}, {7, 3}, {7, 2}, {7, 1}, {7, 0}, {6, 7}, {6, 6}, {6, 5}, {6, 4}, {6, 3}, {6, 2}, {6, 1}, {6, 0}};
    blackPositions = {{0, 7}, {0, 6}, {0, 5}, {0, 4}, {0, 3}, {0, 2}, {0, 1}, {0, 0}, {1, 7}, {1, 6}, {1, 5}, {1, 4}, {1, 3}, {1, 2}, {1, 1}, {1, 0}};
    squares = {
        std::array<int, 8>{BLACK_ROOK, BLACK_KNIGHT, BLACK_BISHOP, BLACK_QUEEN, BLACK_KING, BLACK_BISHOP, BLACK_KNIGHT, BLACK_ROOK},
        std::array<int, 8>{BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN},
        std::array<int, 8>{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        std::array<int, 8>{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        std::array<int, 8>{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        std::array<int, 8>{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        std::array<int, 8>{WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN},
        std::array<int, 8>{WHITE_ROOK, WHITE_KNIGHT, WHITE_BISHOP, WHITE_QUEEN, WHITE_KING, WHITE_BISHOP, WHITE_KNIGHT, WHITE_ROOK}};
}

const std::array<std::array<int, 8>, 8> &Board::getSquares() const
{
    return squares;
}

void Board::doMove(LegalMove &move)
{
    std::cout << "DOING MOVE: " << move;
    auto [oldX, oldY] = move.from;
    auto [newX, newY] = move.to;
    std::cout << "Move from (" << oldX << ", " << oldY << ") to (" << newX << ", " << newY << ")\n";
    // update tracked positions
    if (Identifier::getTeam(move.pieceToMove) == WHITE)
    {
        // Remove captured piece
        blackPositions.erase(std::remove(blackPositions.begin(), blackPositions.end(), move.to), blackPositions.end());

        // Update white piece position
        whitePositions.erase(std::remove(whitePositions.begin(), whitePositions.end(), move.from), whitePositions.end());
        whitePositions.push_back(move.to);
    }
    else
    {
        // Remove captured piece
        whitePositions.erase(std::remove(whitePositions.begin(), whitePositions.end(), move.to), whitePositions.end());

        // Update black piece position
        blackPositions.erase(std::remove(blackPositions.begin(), blackPositions.end(), move.from), blackPositions.end());
        blackPositions.push_back(move.to);
    }
    // move piece
    std::cout << "Doing actual move";
    squares[oldY][oldX] = constants::EMPTY;
    squares[newY][newX] = move.pieceToMove;
}

void Board::undoMove(LegalMove &move)
{
}
