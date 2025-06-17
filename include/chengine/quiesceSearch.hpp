#pragma once

#include "board.hpp"
#include "legalMove.hpp"

LegalMove quiesceSearch(Board &board, int color, double alpha, double beta, int DepthOfStartint, int depth =0);