#ifndef MOVE_HPP
#define MOVE_HPP

#include <tuple>

struct PseudoMove
{
    std::tuple<int, int> to;
    std::tuple<int, int> from;

    int pieceToMove;
    PseudoMove(std::tuple<int, int> to, std::tuple<int, int> from, int pieceToMove) : to(to), from(from), pieceToMove(pieceToMove) {}
};

#endif