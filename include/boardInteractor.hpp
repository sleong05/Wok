#ifndef BOARDINTERACTOR_HPP
#define BOARDINTERACTOR_HPP

#include "chengine/chengine.hpp"
#include "legalMove.hpp"
#include <vector>
#include "board.hpp"
#include <tuple>
#include "constants.hpp"
#include "boardDrawer.hpp"

class BoardInteractor
{
public:
    BoardInteractor(Board &board, BoardDrawer &boardDrawer);
    void click(int row, int col, Board &board, sf::RenderWindow &window);
    void drawInteractionInfo(sf::RenderWindow &window);

private:
    Chengine chengine;
    BoardDrawer boardDrawer;
    sf::RectangleShape square;
    sf::CircleShape circle;
    int playersTurn;
    std::vector<std::tuple<int, int>> possibleMoveLocations;
    std::tuple<int, int> selectedPiece;
    std::map<std::tuple<int, int>, LegalMove> movesMap;
    std::tuple<int, int> chengineMoveSquare = constants::NO_TILE_SELECTED;
};

#endif