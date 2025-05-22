#ifndef BOARDDRAWER_HPP
#define BOARDDRAWER_HPP
#include <SFML/Graphics.hpp>
#include "board.hpp"

class BoardDrawer
{

public:
    BoardDrawer();
    void drawBoard(sf::RenderWindow &window, const Board &board);

private:
    void loadSprites();
    sf::RectangleShape tile;
    std::map<int, sf::Sprite> sprites;
};
#endif