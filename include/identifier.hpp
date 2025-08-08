#ifndef IDENTIFIER_HPP
#define IDENTIFIER_HPP
#include <string>

#include "constants.hpp"

namespace Identifier
{
    inline int getTeam(int piece)
    {
        return (piece < 13) ? constants::WHITE : constants::BLACK;
    }

    bool inline inBounds(int rowOrCol) { return (rowOrCol >= 0 and rowOrCol < 8) ? true : false; }

    inline std::string getPieceName(int piece)
    {
        switch (piece)
        {
        case constants::WHITE_PAWN:
            return "White Pawn";
        case constants::WHITE_KNIGHT:
            return "White Knight";
        case constants::WHITE_BISHOP:
            return "White Bishop";
        case constants::WHITE_ROOK:
            return "White Rook";
        case constants::WHITE_KING:
            return "White King";
        case constants::WHITE_QUEEN:
            return "White Queen";

        case constants::BLACK_PAWN:
            return "Black Pawn";
        case constants::BLACK_KNIGHT:
            return "Black Knight";
        case constants::BLACK_BISHOP:
            return "Black Bishop";
        case constants::BLACK_ROOK:
            return "Black Rook";
        case constants::BLACK_KING:
            return "Black King";
        case constants::BLACK_QUEEN:
            return "Black Queen";

        default:
            return "Unknown";
        }
    }

    inline double getPieceValue(int piece)
    {
        switch (piece)
        {
        case constants::WHITE_PAWN:
        case constants::BLACK_PAWN:

            return 1.0;

        case constants::WHITE_KNIGHT:
        case constants::BLACK_KNIGHT:
            return 3.0;

        case constants::WHITE_BISHOP:
        case constants::BLACK_BISHOP:
            return 3.1;

        case constants::WHITE_ROOK:
        case constants::BLACK_ROOK:
            return 5.0; // we dont want these peices to move unless they arleady have

        case constants::WHITE_QUEEN:
        case constants::BLACK_QUEEN:
            return 9.0; // we dont want these peices to move unless they arleady have

        case constants::WHITE_KING:
        case constants::BLACK_KING:
            return 1;
        }
        return constants::EMPTY;
    }
}
#endif