#include "identifier.hpp"
#include "constants.hpp"
#include <iostream>
using namespace constants;
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

double Identifier::getPieceValue(int piece)
{
    switch (piece)
    {
    case WHITE_PAWN:
    case BLACK_PAWN:

        return 1.0;

    case WHITE_KNIGHT:
    case BLACK_KNIGHT:
        return 3.0;

    case WHITE_BISHOP:
    case BLACK_BISHOP:
        return 3.1;

    case WHITE_ROOK:
    case BLACK_ROOK:
        return 5.0; // we dont want these peices to move unless they arleady have

    case WHITE_QUEEN:
    case BLACK_QUEEN:
        return 9.0; // we dont want these peices to move unless they arleady have

    case WHITE_KING:
    case BLACK_KING:
        return 1;

    default:
        std::cout << "getPieceValue called with piece = " << piece << std::endl;
        throw std::runtime_error("tried to eget the value of not a piece");
    }
}
