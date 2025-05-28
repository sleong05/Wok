#ifndef IDENTIFIER_HPP
#define IDENTIFIER_HPP
#include <string>

#include "constants.hpp"

class Identifier
{
public:
    int static getTeam(int piece);
    bool static inline inBounds(int rowOrCol) { return (rowOrCol >= 0 and rowOrCol < 8) ? true : false; }
    std::string static getPieceName(int piece);
    double static getPieceValue(int piece);
};

#endif