#include <iostream>
#include <SFML/Graphics.hpp>
#include "constants.hpp"
#include "board.hpp"
#include "textureManager.hpp"
#include "boardDrawer.hpp"
int main()
{
    std::pair<int, int> selectedPieceLocation;
    TextureManager::loadTextures();

    Board board = Board();
    BoardDrawer boardDrawer = BoardDrawer();

    sf::RenderWindow window(sf::VideoMode(constants::WINDOW_WIDTH, constants::WINDOW_HEIGHT), "Wok");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;

            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    int x = event.mouseButton.x / constants::TILE_SIZE;
                    int y = event.mouseButton.y / constants::TILE_SIZE;

                    const auto squares = board.getSquares();
                    std::cout << squares[y][x] << '\n';
                }
                break;

            default:
                break;
            }
        }
        window.clear(constants::greenTileColor);

        // draw pieces and board
        boardDrawer.drawBoard(window, board);
        window.display();
    }
    return 0;
}