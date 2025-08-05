#pragma once
#include <array>

namespace weights
{
    constexpr int MATE = 999999;
    constexpr int DRAW = 0;

    constexpr double KING_SAFE = .4;

    constexpr int MAX_QUIESCE_DEPTH = 6;

    constexpr double PASSED_PAWN = .4;       // for distance from end
    constexpr double PASSED_PAWN_VALUE = .3; // is a passed pawn
    constexpr double ISOLATED_PAWN = .15;
    constexpr double CHAIN_VALUE = .1;
    constexpr double DOUBLED_PAWN = .4;
    constexpr double PAWN_INFRONT_OF_KING = .3;
    constexpr double CENTER_PAWN_VALUE = .2;
    constexpr double DISTANCE_FROM_PROMOTION_VALUE = .065;

    inline constexpr std::array<std::array<double, 8>, 8> knightValues = {{{0.0, .1, .1, .1, .1, .1, .1, 0.0},
                                                                           {.05, .15, .2, .2, .2, .2, .15, .05},
                                                                           {.1, .2, .35, .35, .35, .35, .2, .1},
                                                                           {.15, .2, .35, .4, .4, .35, .2, .15},
                                                                           {.15, .2, .35, .4, .4, .35, .2, .15},
                                                                           {.1, .2, .35, .35, .35, .35, .2, .1},
                                                                           {.05, .15, .2, .2, .2, .2, .15, .05},
                                                                           {0.0, .1, .1, .1, .1, .1, .1, 0.0}}};

    inline constexpr std::array<std::array<double, 8>, 8> kingValues = {{{0.0, .0, 2, -.5, -.3, -.4, 2, 0.0},
                                                                         {.0, .0, .0, -.4, -.4, -.4, .0, .0},
                                                                         {.0, .0, .0, .0, .0, .0, .0, .0},
                                                                         {.0, .0, .0, .0, .0, .0, .0, .0},
                                                                         {.0, .0, .0, .0, .0, .0, .0, .0},
                                                                         {.1, .0, .0, .0, .0, .0, .0, .0},
                                                                         {.0, .0, .0, -.4, -.4, -.4, .0, .0},
                                                                         {0.0, .0, 2, -.5, -.3, -.4, 2, 0.0}}};

    inline constexpr std::array<std::array<double, 8>, 8> rookValues = {{{0.2, .15, .15, .15, .15, .15, .15, 0.2}, // not used rn
                                                                         {.15, .15, .15, .1, .1, .15, .15, .15},
                                                                         {0.0, .05, .05, .05, .05, .05, .05, 0.0},
                                                                         {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
                                                                         {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
                                                                         {0.0, .05, .05, .05, .05, .05, .05, 0.0},
                                                                         {.15, .15, .15, .1, .1, .15, .15, .15},
                                                                         {0.2, .15, .15, .15, .15, .15, .15, 0.2}}};

}