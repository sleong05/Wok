#include "boardInteractor.hpp"
#include "constants.hpp"
#include "board.hpp"
#include "moveGetter.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <tuple>

const std::tuple<int, int> EMPTY_TILE = {-1, -1};

BoardInteractor::BoardInteractor()
{
    square.setSize(sf::Vector2f(constants::TILE_SIZE, constants::TILE_SIZE));
    selectedPiece = {-1, -1}; // no value
    playersTurn = constants::WHITE;

    circle.setRadius(constants::TILE_SIZE / 2.25);            // smaller than tile for visual clarity
    circle.setOrigin(circle.getRadius(), circle.getRadius()); // center it
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineColor(sf::Color(80, 80, 80, 200));
    circle.setOutlineThickness(6.f);
}

void BoardInteractor::click(int col, int row, Board &board)
{
    std::tuple<int, int> clickedTile = {col, row};
    std::cout << "clicked on row col" << row << ", " << col << std::endl;

    auto it = movesMap.find(clickedTile);
    if (it != movesMap.end()) // tile is in movesMap keys
    {
        std::cout << "MOVE SELECTed" << it->second << '\n';
        board.doMove(it->second);

        selectedPiece = constants::NO_TILE_SELECTED;
        movesMap.clear();
        return;
    }
    // click is not on a possible move
    movesMap.clear();
    auto moves = MoveGetter::getMovesFromPieceAt(col, row, board);

    // empty case
    if (moves.empty() && board.getSquares()[row][col] == constants::EMPTY)
    {
        std::cout << "EMPTY" << '\n';
        selectedPiece = constants::NO_TILE_SELECTED;
        return;
    }
    // we have selected a piece
    selectedPiece = clickedTile;
    // draw square on moves
    square.setFillColor(sf::Color::Yellow);
    for (const LegalMove &move : moves)
    {
        std::cout << "possible move in loop" << std::endl;
        std::tuple<int, int> position = move.to;
        movesMap.insert_or_assign(position, move);
    }
}

void BoardInteractor::drawInteractionInfo(sf::RenderWindow &window)
{
    // draw square on piece
    if (selectedPiece != EMPTY_TILE)
    {
        auto [col, row] = selectedPiece;
        square.setPosition(col * constants::TILE_SIZE, row * constants::TILE_SIZE);
        square.setFillColor(sf::Color::Red);
        window.draw(square);

        square.setFillColor(sf::Color::Yellow);
        for (auto &move : movesMap)
        {
            auto [col, row] = move.first;
            circle.setPosition(col * constants::TILE_SIZE + constants::TILE_SIZE / 2,
                               row * constants::TILE_SIZE + constants::TILE_SIZE / 2);
            window.draw(circle);
        }
    }
}
