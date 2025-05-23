#include "boardInteractor.hpp"
#include "constants.hpp"
#include "board.hpp"
#include "moveGetter.hpp"
#include <SFML/Graphics.hpp>

#include <iostream>

const std::tuple<int, int> EMPTY_TILE = {-1, -1};

BoardInteractor::BoardInteractor()
{
    square.setSize(sf::Vector2f(constants::TILE_SIZE, constants::TILE_SIZE));
    selectedPiece = {-1, -1}; // no value
    playersTurn = constants::WHITE;
}

void BoardInteractor::click(int col, int row, Board &board)
{
    std::tuple<int, int> clickedTile = {col, row};
    std::cout << "clicked on row col" << row << ", " << col << std::endl;
    if (std::find(possibleMoveLocations.begin(), possibleMoveLocations.end(), clickedTile) != possibleMoveLocations.end())
    {
        std::cout << "MOVE SELECTed" << '\n';

        selectedPiece = {-1, -1};
        possibleMoveLocations.clear();
        return;
    }
    // click is not on a possible move
    possibleMoveLocations.clear();
    auto moves = MoveGetter::getMovesFromPieceAt(col, row, board);

    // empty case
    if (moves.empty() && board.getSquares()[row][col] == constants::EMPTY)
    {
        std::cout << "EMPTY" << '\n';
        selectedPiece = {-1, -1};
        return;
    }
    // we have selected a piece
    selectedPiece = clickedTile;
    // draw square on moves
    square.setFillColor(sf::Color::Yellow);
    for (const PseudoMove &move : moves)
    {
        std::cout << "possible move in loop" << std::endl;
        auto [x, y] = move.to;
        possibleMoveLocations.emplace_back(x, y);
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
        for (auto &move : possibleMoveLocations)
        {
            auto [col, row] = move;
            square.setPosition(col * constants::TILE_SIZE, row * constants::TILE_SIZE);
            window.draw(square);
        }
    }
}
