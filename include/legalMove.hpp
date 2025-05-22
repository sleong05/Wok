#ifndef LEGALMOVE_HPP
#define LEGALMOVE_HPP

#include <tuple>

struct LegalMove
{
    std::tuple<int, int> to;
    std::tuple<int, int> from;

    int pieceToMove;
    int pieceAtEnd;

    LegalMove(std::tuple<int, int> to, std::tuple<int, int> from, int pieceToMove, int pieceAtEnd)
        : to(to), from(from), pieceToMove(pieceToMove), pieceAtEnd(pieceAtEnd) {}
};

#endif