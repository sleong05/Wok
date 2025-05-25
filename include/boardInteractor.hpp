#ifndef BOARDINTERACTOR_HPP
#define BOARDINTERACTOR_HPP

#include "legalMove.hpp"
#include <vector>
#include "board.hpp"
#include <tuple>
class BoardInteractor
{
public:
    BoardInteractor();
    void click(int row, int col, Board &board, sf::RenderWindow &window);
    void drawInteractionInfo(sf::RenderWindow &window);

private:
    sf::RectangleShape square;
    sf::CircleShape circle;
    int playersTurn;
    std::vector<std::tuple<int, int>> possibleMoveLocations;
    std::tuple<int, int> selectedPiece;
    std::map<std::tuple<int, int>, LegalMove> movesMap;
};

#endif