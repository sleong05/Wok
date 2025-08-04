#include "chengine/chengine.hpp"
#include "legalMove.hpp"
#include "board.hpp"
#include "constants.hpp"
#include <random>
#include "moveGetter.hpp"
#include "chengine/minMaxTree.hpp"
#include "chengine/chengine.hpp"
#include "chengine/pawnStructureEvaluator.hpp"

using namespace constants;

LegalMove Chengine::getMove(Board &board, int color, int startDepth)
{
    return miniMax.getBestMove(board, color, stopPonder, searchTime, startDepth);
}
// returns the depth of the move found
void Chengine::startPondering(Board board, int color)
{
    // ensure pondering is not currently occuring
    if (ponderingThread.joinable())
    {
        stopPondering(); // prevent zombie threads
    }
    // let chengine ponder five minutes (this will be stopped early). if longer assume, they are afk so stop thinking
    ponderDepth = 0;
    stopPonder.store(false);
    int startDepth = 2;
    std::chrono::duration<double> maxTimeMs = std::chrono::duration<double>(300);

    ponderingThread = std::thread([this, board, color, startDepth, maxTimeMs]() mutable
                                  { ponderDepth = miniMax.getBestMove(board, color, stopPonder, maxTimeMs, startDepth).valueDepth;
                                std::cout<< "ponder finished at depth " << ponderDepth << "\n"; });
}

int Chengine::stopPondering()
{
    if (ponderingThread.joinable())
    {
        stopPonder.store(true);
        ponderingThread.join();
        stopPonder.store(false);
        return ponderDepth;
    }
    return 2;
}
Chengine::Chengine(Board &board) : miniMax(), board(board)
{
}
