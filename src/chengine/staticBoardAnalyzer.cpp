#include "chengine/staticBoardAnalyzer.hpp"
#include "board.hpp"
#include "constants.hpp"

using namespace constants;

double SBAnalyzer::evaluateBoard(const Board &board)
{
    double boardValue = 0;
    const auto boardState = board.getSquares();

    for (const auto &position : board.blackPositions)
    {
        auto [col, row] = position;
        boardValue -= getPieceValue(boardState[row][col]);
    }

    for (const auto &position : board.whitePositions)
    {

        auto [col, row] = position;
        boardValue += getPieceValue(boardState[row][col]);
    }
    return boardValue;
}

double SBAnalyzer::getPieceValue(int piece)
{
    switch (piece)
    {
    case WHITE_PAWN:
    case BLACK_PAWN:
        return 1.0;

    case WHITE_KNIGHT:
    case BLACK_KNIGHT:
        return 3.0;

    case WHITE_BISHOP:
    case BLACK_BISHOP:
        return 3.1;

    case WHITE_ROOK:
    case BLACK_ROOK:
        return 5.0;

    case WHITE_QUEEN:
    case BLACK_QUEEN:
        return 9.0;

    case WHITE_KING:
    case BLACK_KING:
        return 0.0;

    default:
        std::cerr << "Invalid piece ID: " << piece << '\n';
        throw std::runtime_error("tried to evalaute not a piece");
        return 0;
    }
}