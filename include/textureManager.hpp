#ifndef TEXTUREMANAGER_CPP
#define TEXTUREMANAGER_CPP
#include <SFML/Graphics.hpp>

class TextureManager
{
public:
    sf::Texture getTexture(int piece);
    void loadTextures();

private:
    std::map<std::string, sf::Texture> pieceTextures;
}

#endif