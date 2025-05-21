#include "textureManager.hpp"
#include <sstream>
#include <SFML/Graphics.hpp>
#include "constants.hpp"

void TextureManager::loadTextures()
{
    std::vector<std::string> pieceNames = {
        "whitepawn", "blackpawn",
        "whiterook", "blackrook",
        "whiteknight", "blackknight",
        "whitebishop", "blackbishop",
        "whitequeen", "blackqueen",
        "whiteking", "blackking"};

    for (const std::string &name : pieceNames)
    {
        sf::Texture texture;
        texture.loadFromFile("assets/" + name + ".png");
        pieceTextures[name] = texture;
    }
}

sf::Texture getTexture(int piece)
{
}