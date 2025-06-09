#pragma once
#include "legalMove.hpp"
#include "board.hpp"
#include "minMaxTree.hpp"
class Chengine
{
public:
    Chengine(Board &board);
    LegalMove getMove();

    LegalMove getRandomMove();

private:
    MinMaxTree miniMax;
    Board &board;
};