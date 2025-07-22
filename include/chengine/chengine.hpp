#pragma once
#include "legalMove.hpp"
#include "board.hpp"
#include "minMaxTree.hpp"
class Chengine
{
public:
    Chengine(Board &board);
    LegalMove getMove(Board &board, int color);

    LegalMove getRandomMove();

private:
    MinMaxTree miniMax;
    Board &board;
};