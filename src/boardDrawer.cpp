#include "boardDrawer.hpp"
#include "constants.hpp"
#include "textureManager.hpp"

using namespace constants;
BoardDrawer::BoardDrawer()
{
    loadSprites();
    tile.setSize(sf::Vector2f(constants::TILE_SIZE, constants::TILE_SIZE));
    tile.setFillColor(constants::creamTileColor);
}

void BoardDrawer::loadSprites()
{
    std::vector<int> pieceValues = {
        WHITE_PAWN,
        WHITE_KNIGHT,
        WHITE_BISHOP,
        WHITE_ROOK,
        WHITE_KING,
        WHITE_QUEEN,

        BLACK_PAWN,
        BLACK_KNIGHT,
        BLACK_BISHOP,
        BLACK_ROOK,
        BLACK_KING,
        BLACK_QUEEN,
    };

    for (const int &pieceValue : pieceValues)
    {
        sf::Sprite sprite;
        sf::Texture &texture = TextureManager::getTexture(pieceValue);
        sprite.setTexture(texture);

        sf::Vector2u texSize = texture.getSize();
        sprite.setScale(
            TILE_SIZE / texSize.x,
            TILE_SIZE / texSize.y);

        sprites[pieceValue] = sprite;
    }
}

void BoardDrawer::drawBoard(sf::RenderWindow &window, const Board &board)
{
    // draw background
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
    // draw pieces
    auto squares = board.getSquares();

    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            if (squares[row][col] != constants::EMPTY)
            {
                sprites[squares[row][col]].setPosition(col * TILE_SIZE, row * TILE_SIZE);
                window.draw(sprites[squares[row][col]]);
            }
        }
    }
}