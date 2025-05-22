#ifndef TEXTUREMANAGER_HPP
#define TEXTUREMANAGER_HPP
#include <SFML/Graphics.hpp>

class TextureManager
{
public:
    static sf::Texture &getTexture(int piece);
    static void loadTextures();

private:
    static std::map<std::string, sf::Texture> pieceTextures;
    static std::vector<std::string> pieceNames;
};

#endif