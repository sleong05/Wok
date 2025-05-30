#include "chengine/chengine.hpp"
#include "legalMove.hpp"
#include "board.hpp"
#include "constants.hpp"
#include <random>
#include "moveGetter.hpp"
#include "chengine/minMaxTree.hpp"

using namespace constants;

LegalMove Chengine::getMove()
{

    return miniMax.getBestMove(constants::BLACK);
}

LegalMove Chengine::getRandomMove()
{
    auto possibleMoves = MoveGetter::getMovesForTeam(constants::BLACK, board);
    if (possibleMoves.empty())
        return LegalMove(NO_TILE_SELECTED, NO_TILE_SELECTED, EMPTY, EMPTY);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, possibleMoves.size() - 1);

    int randomNumber = dist(gen);

    std::cout << "doing Move " << possibleMoves[randomNumber] << std::endl;
    return possibleMoves[randomNumber];
}

Chengine::Chengine(Board &board) : miniMax(board), board(board)
{
}
