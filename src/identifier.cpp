#include "identifier.hpp"

int Identifier::getTeam(int piece)
{
    return (piece < 13) ? constants::WHITE : constants::BLACK;
}