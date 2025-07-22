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

    inline void computePriority(bool isBestMove)
    {
        if (isPromotion)
            priorityOfSearchValue += 100;

        if (pieceAtEnd != constants::EMPTY)
        {
            int gain = Identifier::getPieceValue(pieceAtEnd) - Identifier::getPieceValue(pieceToMove);
            priorityOfSearchValue += std::max(gain, 1);
        }

        if (isBestMove)
        {
            priorityOfSearchValue += 3;
        }

        return;
    }
};

inline std::ostream &
operator<<(std::ostream &os, const LegalMove &move)
{
    auto [toCol, toRow] = move.to;
    auto [fromCol, fromRow] = move.from;

    os << Identifier::getPieceName(move.pieceToMove) << " to " << " " << toCol << ", " << toRow << " " << " from " << fromCol << ", " << fromRow << " with value " << move.value << '\n';
    return os;
}

inline bool operator==(const LegalMove &lhs, const LegalMove &rhs)
{
    return lhs.to == rhs.to &&
           lhs.from == rhs.from &&
           lhs.pieceToMove == rhs.pieceToMove &&
           lhs.pieceAtEnd == rhs.pieceAtEnd &&
           lhs.isCastle == rhs.isCastle &&
           lhs.isPromotion == rhs.isPromotion &&
           lhs.isEnPassant == rhs.isEnPassant &&
           lhs.promotionPiece == rhs.promotionPiece &&
           lhs.fromHasMoved == rhs.fromHasMoved &&
           lhs.toHasMoved == rhs.toHasMoved &&
           lhs.oldCastlingRights == rhs.oldCastlingRights &&
           lhs.oldEnPassantFile == rhs.oldEnPassantFile;
}

#endif