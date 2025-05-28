#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <SFML/Graphics.hpp>
#include <tuple>
#include <array>

namespace constants
{
    constexpr float TILE_SIZE = 100.f;
    constexpr float WINDOW_HEIGHT = 800.f;
    constexpr float WINDOW_WIDTH = 800.f;
    inline const sf::Color creamTileColor(238, 238, 210);
    inline const sf::Color greenTileColor(118, 150, 86);

    constexpr int QUEEN_ATTACK_QUE = 900;

    constexpr int WHITE = -1;
    constexpr int BLACK = 1;
    constexpr int PLAYING_PLAYER = WHITE;

    constexpr int WHITE_PAWN = 7;
    constexpr int WHITE_KNIGHT = 8;
    constexpr int WHITE_BISHOP = 9;
    constexpr int WHITE_ROOK = 10;
    constexpr int WHITE_KING = 11;
    constexpr int WHITE_QUEEN = 12;

    constexpr int BLACK_PAWN = 13;
    constexpr int BLACK_KNIGHT = 14;
    constexpr int BLACK_BISHOP = 15;
    constexpr int BLACK_ROOK = 16;
    constexpr int BLACK_KING = 17;
    constexpr int BLACK_QUEEN = 18;

    constexpr int EMPTY = 0;

    inline constexpr std::array<std::tuple<int, int>, 8> KING_OFFSETS = {{{-1, -1}, {-1, 0}, {-1, 1}, {0, 1}, {0, -1}, {1, 1}, {1, 0}, {1, -1}}};

    inline constexpr std::array<std::tuple<int, int>, 8> KNIGHT_OFFSETS = {{{2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2}}};

    inline constexpr std::array<std::tuple<int, int>, 4> BISHOP_OFFSETS = {{{-1, -1}, {-1, 1}, {1, -1}, {1, 1}}};

    inline constexpr std::array<std::tuple<int, int>, 4> ROOK_OFFSETS = {{{0, -1}, {-1, 0}, {0, 1}, {1, 0}}};

    inline const std::tuple<int, int> NO_TILE_SELECTED = {-1, -1};
};

#endif