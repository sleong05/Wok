#pragma once
#include <array>

class TheoryEvaluator
{
public:
    static double getPawnValue(int col, int row, const std::array<std::array<int, 8u>, 8u> &boardState);
    static double passPawnValue(int color, int row, int col, const std::array<std::array<int, 8U>, 8U> &boardState);
    static double getKnightValue(int col, int row);
    static double getRookValue(int col, int row, const std::array<std::array<int, 8u>, 8u> &boardState);
    static double getQueenValue(int col, int row, const std::array<std::array<int, 8u>, 8u> &boardState);
    static double getKingValue(int col, int row, const std::array<std::array<int, 8u>, 8u> &boardState, const std::array<std::array<bool, 8u>, 8u> &moveState, int movesDone);
    static double getBishopValue(int col, int row, const std::array<std::array<int, 8u>, 8u> &boardState);
};