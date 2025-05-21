#include "piece.hpp"
#include <SFML/Graphics.hpp>

class Pawn : public Piece
{
public:
    Pawn(int x, int y, int color);
    void draw(sf::RenderWindow &window) override;

private:
    sf::Texture texture;
};