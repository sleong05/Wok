#ifndef LEGALMOVE_HPP
#define LEGALMOVE_HPP

#include <tuple>

struct LegalMove
{
    std::tuple<int, int> to;
    std::tuple<int, int> from;

    int pieceToMove;
    int pieceAtEnd;

    std::vector<std::pair<int, std::tuple<int, int>>> additionalPiecesToRemove;
    std::vector<std::pair<int, std::tuple<int, int>>> additionalPiecesMove;
    bool additionalPieceHasMoved;

    LegalMove(std::tuple<int, int> to, std::tuple<int, int> from, int pieceToMove, int pieceAtEnd)
        : to(to), from(from), pieceToMove(pieceToMove), pieceAtEnd(pieceAtEnd) {}
};

#endif