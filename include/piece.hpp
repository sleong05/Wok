#ifndef PIECE_HPP
#define PIECE_HPP
#include <SFML/Graphics.hpp>
#include "constants.hpp"

class Piece
{
public:
    Piece(int x, int y, int color);
    int getColor();
    std::pair<int, int> getPosition();

    virtual ~Piece();
    virtual void draw(sf::RenderWindow &window) = 0;

protected:
    sf::Sprite sprite;
    std::pair<int, int> position;
    int color;
};
#endif