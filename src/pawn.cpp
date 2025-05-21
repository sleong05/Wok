#include "pawn.hpp"
#include <string>
#include "constants.hpp"
#include <sstream>
#include <SFML/Graphics.hpp>

Pawn::Pawn(int x, int y, int color) : Piece(x, y, color)
{
    std::string team = (color == constants::WHITE) ? "white" : "black";

    std::ostringstream oss;
    oss << "assets/" << team << "pawn.png";
    texture.loadFromFile(oss.str());

    sprite.setTexture(texture);
}

void Pawn::draw(sf::RenderWindow &window)
{
    auto [x, y] = position;
    sprite.setPosition(x, y);
    window.draw(sprite);
}