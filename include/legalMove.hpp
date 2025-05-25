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
    bool hasMoved = false;

    bool isEnPassant = false;
    bool isCastle = false;
    bool isPromotion = false;
    int promotionPiece = constants::EMPTY;
    bool additionalPieceHasMoved;

    LegalMove(std::tuple<int, int> to, std::tuple<int, int> from, int pieceToMove, int pieceAtEnd)
        : to(to), from(from), pieceToMove(pieceToMove), pieceAtEnd(pieceAtEnd) {
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