#include "identifier.hpp"

int Identifier::getTeam(int piece)
{
    if (piece == 0)
    {
        throw std::runtime_error("An empty was passed into a get team");
    }
    return (piece < 13) ? constants::WHITE : constants::BLACK;
}