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

    bool fromHasMoved;
    bool toHasMoved;

    double value;

    int priorityOfSearchValue;

    LegalMove(std::tuple<int, int> to, std::tuple<int, int> from, int pieceToMove, int pieceAtEnd, bool fromHasMoved, bool toHasMoved)
        : to(to), from(from), pieceToMove(pieceToMove), pieceAtEnd(pieceAtEnd), fromHasMoved(fromHasMoved), toHasMoved(toHasMoved)
    {
        auto [aCol, aRow] = to;
        auto [fromCol, fromRow] = from;
        if (aCol == -1 && aRow == -1 && fromCol == -1 && fromRow == -1)
        {
            priorityOfSearchValue = -10000;
            return;
        }
        int queValue = 0;

        // promotion most important
        if (isPromotion)
        {
            queValue += 1000;
        }

        // captures

        if (pieceAtEnd != constants::EMPTY)
        {

            queValue += 100 * (Identifier::getPieceValue(pieceAtEnd) - Identifier::getPieceValue(pieceToMove));
        }

        //----------------------------------------- normal moves

        // check central bonus e4/d4/e5/d5

        int CapturingPieceValue = Identifier::getPieceValue(pieceToMove);
        int OrderIngValue = 10 - CapturingPieceValue;

        // order of looking = pawn/king 9-> knight/bishop 7-> rook 5-> queen captures 1

        if ((aCol == 3 or aCol == 4) && (aRow == 3 or aRow == 4))
        {
            queValue += 10 * (OrderIngValue);
        }

        // search pawn -> queen
        queValue += OrderIngValue;

        priorityOfSearchValue = queValue;

        auto [toCol, toRow] = to;
        std::cout << "legalmove Created: " << "(from=(" << fromCol << ", " << fromRow << "), to=("
                  << toCol << ", " << toRow << "), pieceToMove=" << pieceToMove
                  << ", pieceAtEnd=" << pieceAtEnd << ") with value: " << priorityOfSearchValue << "\n";
    };

    LegalMove() : to(constants::NO_TILE_SELECTED), from(constants::NO_TILE_SELECTED),
                  pieceToMove(constants::EMPTY), pieceAtEnd(constants::EMPTY), fromHasMoved(false), toHasMoved(false)
    {
        // this is for making DummyMoves for mate and drawing so they should be highest priorirty
        auto [toCol, toRow] = to;
        auto [fromCol, fromRow] = from;
        priorityOfSearchValue = 10000;
        std::cout << "legalmove Created DUMMY VERSION: " << "(from=(" << fromCol << ", " << fromRow << "), to=("
                  << toCol << ", " << toRow << "), pieceToMove=" << pieceToMove
                  << ", pieceAtEnd=" << pieceAtEnd << ")  with value: " << priorityOfSearchValue << "\n";
    };
};

inline std::ostream &
operator<<(std::ostream &os, const LegalMove &move)
{
    auto [toCol, toRow] = move.to;
    auto [fromCol, fromRow] = move.from;

    os << "LegalMove(from=(" << fromCol << ", " << fromRow << "), to=("
       << toCol << ", " << toRow << "), pieceToMove=" << move.pieceToMove
       << ", pieceAtEnd=" << move.pieceAtEnd << ")\n";
    return os;
}

#endif