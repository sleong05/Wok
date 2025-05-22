#include "textureManager.hpp"
#include <sstream>
#include <SFML/Graphics.hpp>
#include "constants.hpp"

std::map<std::string, sf::Texture> TextureManager::pieceTextures;
std::vector<std::string> TextureManager::pieceNames;

void TextureManager::loadTextures()
{
    pieceNames = {"whitePawn", "whiteKnight",
                  "whiteBishop", "whiteRook",
                  "whiteKing", "whiteQueen",
                  "blackPawn", "blackKnight",
                  "blackBishop", "blackRook",
                  "blackKing", "blackQueen"};

    for (const std::string &name : pieceNames)
    {
        sf::Texture texture;
        texture.loadFromFile("assets/" + name + ".png");
        pieceTextures[name] = std::move(texture);
    }
}

sf::Texture &TextureManager::getTexture(int piece)
{
    int index = piece - 7; // piece numbers start at 7 (i.e. whitePawn = 7)
    return pieceTextures[pieceNames[index]];
}