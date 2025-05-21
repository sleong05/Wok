#include <iostream>
#include <SFML/Graphics.hpp>
#include "constants.hpp"
#include "board.hpp"

int main()
{

    sf::RenderWindow window(sf::VideoMode(constants::WINDOW_WIDTH, constants::WINDOW_HEIGHT), "Wok");
    Board board;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(constants::greenTileColor);
        // draw pieces and board
        board.draw(window);
        window.display();
    }

    return 0;
}