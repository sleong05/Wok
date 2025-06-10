#ifndef LEGALMOVE_HPP
#define LEGALMOVE_HPP

#include "identifier.hpp"
#include <vector>
#include <tuple>
#include <iostream>
#include <memory>
#include "constants.hpp"

struct LegalMove
{
    std::tuple<int, int> to;
    std::tuple<int, int> from;

    int pieceToMove;
    int pieceAtEnd;

    bool isCastle = false;
    bool isPromotion = false;
    bool isEnPassant = false;
    int promotionPiece = constants::EMPTY;

    bool fromHasMoved = false;
    bool toHasMoved = false;

    uint8_t oldCastlingRights = 0;
    int oldEnPassantFile = -1;



    double value;

    double priorityOfSearchValue = 0;

    LegalMove(std::tuple<int, int> to, std::tuple<int, int> from, int pieceToMove, int pieceAtEnd)
        : to(to), from(from), pieceToMove(pieceToMove), pieceAtEnd(pieceAtEnd) {};

    LegalMove() : to(constants::NO_TILE_SELECTED), from(constants::NO_TILE_SELECTED),
                  pieceToMove(constants::EMPTY), pieceAtEnd(constants::EMPTY) {};

        inline void computePriority()
    {
        if (isPromotion)
        {
            priorityOfSearchValue += 100;
        }

        if (pieceAtEnd != constants::EMPTY)
        {
            int gain = Identifier::getPieceValue(pieceAtEnd) - Identifier::getPieceValue(pieceToMove);
            priorityOfSearchValue += std::max(gain, 1);
        }
        return;
    }
};

inline std::ostream &
operator<<(std::ostream &os, const LegalMove &move)
{
    auto [toCol, toRow] = move.to;
    auto [fromCol, fromRow] = move.from;

    os << "move " << Identifier::getPieceName(move.pieceToMove) << " to " << " " << toCol << ", " << toRow << " " << Identifier::getPieceName(move.pieceAtEnd) << " from " << fromRow << ", " << fromCol << " with value " << move.priorityOfSearchValue << '\n';
    return os;
}

#endif