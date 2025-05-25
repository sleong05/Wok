#pragma once
#include <array>

class squareAttacker
{
public:
    bool static isSquareUnderAttack(int col, int row, int color, const std::array<std::array<int, 8>, 8> &boardState);

private:
    bool static checkHorizontals(int color, int col, int row, const std::array<std::array<int, 8U>, 8U> &boardState);
    bool static checkDiagonals(int color, int col, int row, const std::array<std::array<int, 8U>, 8U> &boardState);
    bool static checkKnightAttack(int color, int col, int row, const std::array<std::array<int, 8U>, 8U> &boardState);
    bool static checkPawnAttack(int row, int color, int col, const std::array<std::array<int, 8U>, 8U> &boardState);
    bool static checkKingAttack(int color, int col, int row, const std::array<std::array<int, 8U>, 8U> &boardState);
};