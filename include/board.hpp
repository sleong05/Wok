#ifndef BOARD_HPP
#define BOARD_HPP

#include "legalMove.hpp"
#include <array>
#include <SFML/Graphics.hpp>
#include "legalMove.hpp"
#include "legalMove.hpp"

class Board
{
public:
    void doMove(LegalMove &move);
    void undoMove(LegalMove &move);
    Board();
    const std::array<std::array<int, 8>, 8> &getSquares() const;

private:
    std::vector<std::tuple<int, int>> blackPositions;
    std::vector<std::tuple<int, int>> whitePositions;
    void initilizeBoard();
    std::array<std::array<int, 8>, 8> squares;
};

#endif