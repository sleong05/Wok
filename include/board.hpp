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
    void doMove(LegalMove &move, sf::RenderWindow &window, bool fromUser = false);
    void castle(LegalMove &move);
    void handlePromotion(LegalMove &move);
    void removePositionFromColorTracker(int color, int newCol, int newRow);
    void addPositionToColorTracker(int color, int newCol, int newRow);
    void updateKnownPositions(LegalMove &move);
    void undoMove(LegalMove &move);
    void verifyTrackerConsistency() const;
    void printPositionTrackerAsBoard() const;
    int showPromotionMenu(sf::RenderWindow &window, int color);
    Board();
    const std::array<std::array<int, 8>, 8> &getSquares() const;
    bool hasMoved(int col, int row);
    LegalMove getLastMove();

    std::array<std::array<bool, 8>, 8> getMovesArray();

private:
    LegalMove lastMove;
    std::array<std::array<bool, 8>, 8> hasMovedArray;
    std::vector<std::tuple<int, int>> blackPositions;
    std::vector<std::tuple<int, int>> whitePositions;
    void initilizeBoard();
    std::array<std::array<int, 8>, 8> squares;
};

#endif