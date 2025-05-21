#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <SFML/Graphics.hpp>

namespace constants
{
    const float TILE_SIZE = 100.f;
    const float WINDOW_HEIGHT = 800.f;
    const float WINDOW_WIDTH = 800.f;
    const sf::Color creamTileColor(238, 238, 210);
    const sf::Color greenTileColor(118, 150, 86);
    const int WHITE = 1;
    const int BLACK = -1;

    const int WHITE_PAWN = 7;
    const int WHITE_HORSE = 8;
    const int WHITE_BISHOP = 9;
    const int WHITE_ROOK = 10;
    const int WHITE_KING = 11;
    const int WHITE_QUEEN = 12;

    const int BLACK_PAWN = 13;
    const int BLACK_HORSE = 14;
    const int BLACK_BISHOP = 15;
    const int BLACK_ROOK = 16;
    const int BLACK_KING = 17;
    const int BLACK_QUEEN = 18;
}

#endif