#include "identifier.hpp"

int Identifier::getTeam(int piece)
{
    if (piece == 0)
    {
        throw std::runtime_error("An empty was passed into a get team");
    }
    return (piece < 13) ? constants::WHITE : constants::BLACK;
}

std::string Identifier::getPieceName(int piece)
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
