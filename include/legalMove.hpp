#ifndef LEGALMOVE_HPP
#define LEGALMOVE_HPP
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

    LegalMove(std::tuple<int, int> to, std::tuple<int, int> from, int pieceToMove, int pieceAtEnd, bool fromHasMoved, bool toHasMoved)
        : to(to), from(from), pieceToMove(pieceToMove), pieceAtEnd(pieceAtEnd), fromHasMoved(fromHasMoved), toHasMoved(toHasMoved) {
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