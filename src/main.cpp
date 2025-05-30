#include <iostream>
#include <SFML/Graphics.hpp>
#include "constants.hpp"
#include "board.hpp"
#include "textureManager.hpp"
#include "boardDrawer.hpp"
#include "boardInteractor.hpp"
#include "identifier.hpp"
#include "moveGetter.hpp"

int main()
{
    std::pair<int, int> selectedPieceLocation;
    TextureManager::loadTextures();

    Board board = Board();
    BoardDrawer boardDrawer = BoardDrawer();
    BoardInteractor boardInteractor = BoardInteractor(board, boardDrawer);
    sf::RenderWindow window(sf::VideoMode(constants::WINDOW_WIDTH, constants::WINDOW_HEIGHT), "Wok");
    bool gameOver = false;

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
                if (not gameOver)
                {
                    if (event.mouseButton.button == sf::Mouse::Left)
                    {
                        int x = event.mouseButton.x / constants::TILE_SIZE;
                        int y = event.mouseButton.y / constants::TILE_SIZE;

                        boardInteractor.click(x, y, board, window);
                        // board.verifyTrackerConsistency();
                        // board.printPositionTrackerAsBoard();
                    }

                    // check for game end

                    if (!gameOver and board.getLastMove().from != constants::NO_TILE_SELECTED)
                    {
                        int currentColor = Identifier::getTeam(board.getLastMove().pieceToMove) == constants::WHITE ? constants::BLACK : constants::WHITE;

                        std::vector<LegalMove> allMoves = MoveGetter::getMovesForTeam(currentColor, board);
                        if (allMoves.empty())
                        {
                            gameOver = true;

                            if (board.isKingInCheck(currentColor))
                                std::cout << ((currentColor == constants::WHITE) ? "Black wins by checkmate!" : "White wins by checkmate!") << "\n";
                            else
                                std::cout << "Stalemate! It's a draw.\n";
                        }
                    }
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
        // draw pieces and board
        window.clear(constants::greenTileColor);
        boardDrawer.drawBoard(window);
        boardInteractor.drawInteractionInfo(window);
        boardDrawer.drawPieces(board, window);

        window.display();
    }
    return 0;
}