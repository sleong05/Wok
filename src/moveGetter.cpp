#include "moveGetter.hpp"
#include "squareAttacker.hpp"
#include "identifier.hpp"
#include "constants.hpp"
#include <board.hpp>
#include <iostream>
#include "allPieces.hpp"
std::vector<LegalMove> MoveGetter::getMovesFromPieceAt(int col, int row, Board &board, bool capturesOnly)
{
    auto boardState = board.getSquares();
    int piece = boardState[row][col];

    switch (piece)
    {
    case constants::WHITE_PAWN:
    {
        auto pseudoMoves = Pawn::getPseudoMoves(col, row, constants::WHITE_PAWN, board, capturesOnly);
        std::vector<LegalMove> legalMoves;
        for (auto &move : pseudoMoves)
        {
            if (board.testMoveCheckLegality(move))
            {
                legalMoves.push_back(move);
            }
        }
        return legalMoves;
    }

    case constants::WHITE_KNIGHT:
    {
        auto pseudoMoves = Knight::getPseudoMoves(col, row, constants::WHITE_KNIGHT, boardState, capturesOnly);
        std::vector<LegalMove> legalMoves;
        for (auto &move : pseudoMoves)
        {
            if (board.testMoveCheckLegality(move))
            {
                legalMoves.push_back(move);
            }
        }
        return legalMoves;
    }

    case constants::WHITE_BISHOP:
    {
        auto pseudoMoves = Bishop::getPseudoMoves(col, row, constants::WHITE_BISHOP, boardState, capturesOnly);
        std::vector<LegalMove> legalMoves;
        for (auto &move : pseudoMoves)
        {
            if (board.testMoveCheckLegality(move))
            {
                legalMoves.push_back(move);
            }
        }
        return legalMoves;
    }

    case constants::WHITE_ROOK:
    {
        auto pseudoMoves = Rook::getPseudoMoves(col, row, constants::WHITE_ROOK, boardState, capturesOnly);
        std::vector<LegalMove> legalMoves;
        for (auto &move : pseudoMoves)
        {
            if (board.testMoveCheckLegality(move))
            {
                legalMoves.push_back(move);
            }
        }
        return legalMoves;
    }

    case constants::WHITE_QUEEN:
    {
        auto pseudoMoves = Queen::getPseudoMoves(col, row, constants::WHITE_QUEEN, boardState, capturesOnly);

        std::vector<LegalMove> legalMoves;
        for (auto &move : pseudoMoves)
        {
            if (board.testMoveCheckLegality(move))
            {
                legalMoves.push_back(move);
            }
        }
        return legalMoves;
    }

    case constants::WHITE_KING:
    {
        auto pseudoMoves = King::getPseudoMoves(col, row, constants::WHITE_KING, board, capturesOnly);
        std::vector<LegalMove> legalMoves;
        for (auto &move : pseudoMoves)
        {
            if (board.testMoveCheckLegality(move))
            {
                legalMoves.push_back(move);
            }
        }
        return legalMoves;
    }

    case constants::BLACK_PAWN:

    {
        auto pseudoMoves = Pawn::getPseudoMoves(col, row, constants::BLACK_PAWN, board, capturesOnly);
        std::vector<LegalMove> legalMoves;
        for (auto &move : pseudoMoves)
        {
            if (board.testMoveCheckLegality(move))
            {
                legalMoves.push_back(move);
            }
        }
        return legalMoves;
    }
    case constants::BLACK_KNIGHT:
    {
        auto pseudoMoves = Knight::getPseudoMoves(col, row, constants::BLACK_KNIGHT, boardState, capturesOnly);
        std::vector<LegalMove> legalMoves;
        for (auto &move : pseudoMoves)
        {
            if (board.testMoveCheckLegality(move))
            {
                legalMoves.push_back(move);
            }
        }
        return legalMoves;
    }
    case constants::BLACK_BISHOP:
    {
        auto pseudoMoves = Bishop::getPseudoMoves(col, row, constants::BLACK_BISHOP, boardState, capturesOnly);
        std::vector<LegalMove> legalMoves;
        for (auto &move : pseudoMoves)
        {
            if (board.testMoveCheckLegality(move))
            {
                legalMoves.push_back(move);
            }
        }
        return legalMoves;
    }
    case constants::BLACK_ROOK:

    {
        auto pseudoMoves = Rook::getPseudoMoves(col, row, constants::BLACK_ROOK, boardState, capturesOnly);
        std::vector<LegalMove> legalMoves;
        for (auto &move : pseudoMoves)
        {
            if (board.testMoveCheckLegality(move))
            {
                legalMoves.push_back(move);
            }
        }
        return legalMoves;
    }
    case constants::BLACK_QUEEN:

    {
        auto pseudoMoves = Queen::getPseudoMoves(col, row, constants::BLACK_QUEEN, boardState, capturesOnly);
        std::vector<LegalMove> legalMoves;
        for (auto &move : pseudoMoves)
        {
            if (board.testMoveCheckLegality(move))
            {
                legalMoves.push_back(move);
            }
        }
        return legalMoves;
    }
    case constants::BLACK_KING:

    {
        auto pseudoMoves = King::getPseudoMoves(col, row, constants::BLACK_KING, board, capturesOnly);
        std::vector<LegalMove> legalMoves;
        for (auto &move : pseudoMoves)
        {
            if (board.testMoveCheckLegality(move))
            {
                legalMoves.push_back(move);
            }
        }
        return legalMoves;
    }

    default:
        return {};
    }
}

// does a come before b
bool static inline sortingFunction(const LegalMove &a, const LegalMove &b, bool queenInDanger)
{
    if (queenInDanger)
        return constants::QUEEN_ATTACK_QUE + a.priorityOfSearchValue > b.priorityOfSearchValue;
    return a.priorityOfSearchValue > b.priorityOfSearchValue;
}

std::vector<LegalMove> MoveGetter::getMovesForTeam(int color, Board &board, bool capturesOnly)
{
    std::vector<LegalMove> LegalMoves;
    LegalMoves.reserve(50);
    auto piecePositions = (color == constants::WHITE) ? board.whitePositions : board.blackPositions;

    // bool queenInDanger = false;

    for (auto &position : piecePositions)
    {
        auto [col, row] = position;
        std::vector<LegalMove> pieceMoves = getMovesFromPieceAt(col, row, board, capturesOnly);

        LegalMoves.insert(LegalMoves.end(), pieceMoves.begin(), pieceMoves.end());
    }

    return LegalMoves;
}

bool MoveGetter::hasMoveLeft(int color, Board &board)
{
    auto piecePositions = (color == constants::WHITE) ? board.whitePositions : board.blackPositions;

    for (auto &position : piecePositions)
    {
        auto [col, row] = position;
        std::vector<LegalMove> pieceMoves = getMovesFromPieceAt(col, row, board);
        if (not pieceMoves.empty())
            return true;
    }
    return false;
}