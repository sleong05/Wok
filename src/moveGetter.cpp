#include "moveGetter.hpp"
#include "squareAttacker.hpp"
#include "identifier.hpp"
#include "constants.hpp"
#include <board.hpp>
#include <iostream>
#include "allPieces.hpp"
std::vector<LegalMove> MoveGetter::getMovesFromPieceAt(int col, int row, Board &board)
{
    auto boardState = board.getSquares();
    int piece = boardState[row][col];
    auto moveState = board.getMovesArray();

    switch (piece)
    {
    case constants::WHITE_PAWN:
    {
        auto pseudoMoves = Pawn::getPseudoMoves(col, row, constants::WHITE_PAWN, board);
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
        auto pseudoMoves = Knight::getPseudoMoves(col, row, constants::WHITE_KNIGHT, boardState, moveState);
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
        auto pseudoMoves = Bishop::getPseudoMoves(col, row, constants::WHITE_BISHOP, boardState, moveState);
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
        auto pseudoMoves = Rook::getPseudoMoves(col, row, constants::WHITE_ROOK, boardState, moveState);
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
        auto pseudoMoves = Queen::getPseudoMoves(col, row, constants::WHITE_QUEEN, boardState, moveState);

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
        auto pseudoMoves = King::getPseudoMoves(col, row, constants::WHITE_KING, board);
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
        auto pseudoMoves = Pawn::getPseudoMoves(col, row, constants::BLACK_PAWN, board);
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
        auto pseudoMoves = Knight::getPseudoMoves(col, row, constants::BLACK_KNIGHT, boardState, moveState);
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
        auto pseudoMoves = Bishop::getPseudoMoves(col, row, constants::BLACK_BISHOP, boardState, moveState);
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
        auto pseudoMoves = Rook::getPseudoMoves(col, row, constants::BLACK_ROOK, boardState, moveState);
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
        auto pseudoMoves = Queen::getPseudoMoves(col, row, constants::BLACK_QUEEN, boardState, moveState);
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
        auto pseudoMoves = King::getPseudoMoves(col, row, constants::BLACK_KING, board);
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
        std::cout << "default case return empty";
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

std::vector<LegalMove> MoveGetter::getMovesForTeam(int color, Board &board)
{
    std::vector<LegalMove> LegalMoves;
    auto &piecePositions = (color == constants::WHITE) ? board.whitePositions : board.blackPositions;
    const auto boardState = board.getSquares();

    // Validate all positions before using them
    piecePositions.erase(
        std::remove_if(piecePositions.begin(), piecePositions.end(),
            [&boardState, color](const auto &pos) {
                auto [col, row] = pos;
                return col < 0 || col >= 8 || row < 0 || row >= 8 || 
                       Identifier::getTeam(boardState[row][col]) != color;
            }),
        piecePositions.end()
    );

    bool queenInDanger = false;
    for (auto &position : piecePositions)
    {
        auto [col, row] = position;
        // get if queen is in danger for optimizzations later

        if (boardState[row][col] == constants::WHITE_QUEEN or boardState[row][col] == constants::BLACK_QUEEN)
        {
            // std::cout << "checking attack at spot " << row << ", " << col << std::endl;
            queenInDanger = squareAttacker::isSquareUnderAttack(col, row, color, boardState);
            // std::cout << "finished check\n";
        }
        std::vector<LegalMove> pieceMoves = getMovesFromPieceAt(col, row, board);

        LegalMoves.insert(LegalMoves.end(), pieceMoves.begin(), pieceMoves.end());
    }

    std::sort(LegalMoves.begin(), LegalMoves.end(), [&](const LegalMove &a, const LegalMove &b)
              { return sortingFunction(a, b, queenInDanger); });
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