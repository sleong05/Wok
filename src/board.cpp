#include <SFML/Graphics.hpp>
#include "board.hpp"
#include "constants.hpp"

Board::Board()
{
    tile.setSize(sf::Vector2f(constants::TILE_SIZE, constants::TILE_SIZE));
    tile.setFillColor(constants::creamTileColor);
}
void Board::draw(sf::RenderWindow &window)
{
    int offset = 0;

    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 4; y++)
        {
            tile.setPosition(x * constants::TILE_SIZE, (y * 2 + offset) * constants::TILE_SIZE);
            window.draw(tile);
        }
        offset = offset ? 0 : 1;
    }
}
