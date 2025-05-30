#include "boardInteractor.hpp"
#include "boardDrawer.hpp"
#include "chengine/chengine.hpp"
#include "identifier.hpp"
#include "constants.hpp"
#include "board.hpp"
#include "moveGetter.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <tuple>
#include "squareAttacker.hpp"

const std::tuple<int, int> EMPTY_TILE = {-1, -1};

BoardInteractor::BoardInteractor(Board &board, BoardDrawer &boardDrawer) : chengine(board), boardDrawer(boardDrawer)
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

void BoardInteractor::click(int col, int row, Board &board, sf::RenderWindow &window)
{
    std::tuple<int, int> clickedTile = {col, row};

    auto it = movesMap.find(clickedTile);
    auto boardState = board.getSquares();
    auto [selectedCol, selectedRow] = selectedPiece;
    if (it != movesMap.end()) // tile is in movesMap keys
    {
        if (Identifier::getTeam(boardState[selectedRow][selectedCol]) == playersTurn)
        {

            sf::RenderWindow *windowPtr = &window;
            board.doMove(it->second, windowPtr, true);

            playersTurn *= -1;
            selectedPiece = constants::NO_TILE_SELECTED;
            movesMap.clear();

            // update screen
            window.clear(constants::greenTileColor);
            boardDrawer.drawBoard(window);
            drawInteractionInfo(window);
            boardDrawer.drawPieces(board, window);
            window.display();
            // do engine Move
            LegalMove chengineMove = chengine.getMove();
            std::cout << "move selected value was " << chengineMove.value << std::endl;
            if (chengineMove.from == constants::NO_TILE_SELECTED)
            { // game is over
                std::cout << "GAME OVER" << std::endl;
                return;
            }
            board.doMove(chengineMove);
            chengineMoveSquare = chengineMove.to;
            playersTurn *= -1;

            return;
        }
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
        std::tuple<int, int> position = move.to;
        movesMap.insert_or_assign(position, move);
    }
}

void BoardInteractor::drawInteractionInfo(sf::RenderWindow &window)
{
    if (chengineMoveSquare != EMPTY_TILE)
    {
        auto [col, row] = chengineMoveSquare;
        square.setPosition(col * constants::TILE_SIZE, row * constants::TILE_SIZE);
        square.setFillColor(sf::Color(255, 255, 0, 160));
        window.draw(square);
    }
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
