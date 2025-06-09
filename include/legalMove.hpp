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
        /*
        if (pieceAtEnd != constants::EMPTY)
        {
            int gain = Identifier::getPieceValue(pieceAtEnd) - Identifier::getPieceValue(pieceToMove);
            priorityOfSearchValue += std::max(gain, 0);
        }

        auto [aCol, aRow] = to;
        auto [fromCol, fromRow] = from;
        if (aCol == -1 && aRow == -1 && fromCol == -1 && fromRow == -1)
        {
            priorityOfSearchValue = -10000;
            return;
        }

        // promotion most important
        if (isPromotion)
        {
            priorityOfSearchValue += 1000;
        }

        // captures

        if (pieceAtEnd != constants::EMPTY)
        {
            int gain = Identifier::getPieceValue(pieceAtEnd) - Identifier::getPieceValue(pieceToMove);
            priorityOfSearchValue += std::max(gain, 1) * 100; // captures are sitll jsut interesing
        }

        //----------------------------------------- normal moves

        // check central bonus e4/d4/e5/d5

        int CapturingPieceValue = Identifier::getPieceValue(pieceToMove);
        int OrderIngValue = 10 - CapturingPieceValue;

        // order of looking = pawn/king 9-> knight/bishop 7-> rook 5-> queen captures 1

        // search pawn -> queen
        priorityOfSearchValue += OrderIngValue; */
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