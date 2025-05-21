#ifndef BOARD_HPP
#define BOARD_HPP

#include <SFML/Graphics.hpp>

class Board
{
public:
    Board();
    void draw(sf::RenderWindow &window);

private:
    sf::RectangleShape tile;
};
#endif