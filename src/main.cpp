#include <iostream>
#include <SFML/Graphics.hpp>
#include "constants.hpp"
#include "board.hpp"
#include "textureManager.hpp"
#include "boardDrawer.hpp"
#include "boardInteractor.hpp"
int main()
{
    std::pair<int, int> selectedPieceLocation;
    TextureManager::loadTextures();

    Board board = Board();
    BoardInteractor boardInteractor = BoardInteractor();
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
                    ;
                    boardInteractor.click(x, y, board, window);
                    board.verifyTrackerConsistency();
                    board.printPositionTrackerAsBoard();
                }
                break;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::L)
                {
                    LegalMove lastMove = board.getLastMove();
                    board.undoMove(lastMove);
                }
            default:
                break;
            }
        }

        window.clear(constants::greenTileColor);
        boardDrawer.drawBoard(window);
        boardInteractor.drawInteractionInfo(window);
        boardDrawer.drawPieces(board, window);
        // draw pieces and board
        window.display();
    }
    return 0;
}