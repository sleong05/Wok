#include "chengine/minMaxTree.hpp"
#include "chengine/staticBoardAnalyzer.hpp"
#include "legalMove.hpp"
#include "board.hpp"
#include "moveGetter.hpp"
#include "weights.hpp"
#include "constants.hpp"
#include <identifier.hpp>
#include <algorithm>

using namespace constants;

MinMaxTree::MinMaxTree(Board &board) : board(board)
{
}

LegalMove MinMaxTree::getBestMove(int color) // chengine is black so make them alkways look for the lowest value move
{
    int INF = 1000000000.0;

    return lookIntoFutureMoves(color, 0, -INF, INF);
};

LegalMove MinMaxTree::lookIntoFutureMoves(int color, int depth, double alpha, double beta)
{
    auto allMoves = MoveGetter::getMovesForTeam(color, board);
    // -----------------------------------------BASE CASES--------------------------------------------------
    // base case mate/draw
    if (allMoves.empty())
    {
        std::cout << "Mate spotted!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
        int value = board.isKingInCheck(color) ? weights::MATE * color * -1 : weights::DRAW;
        LegalMove dummyMove = LegalMove();
        dummyMove.value = value;
        return dummyMove;
    }

    // base case depth hit
    if (depth == weights::MAX_DEPTH)
    {
        int value = SBAnalyzer::evaluateBoard(board);
        LegalMove dummyMove = LegalMove();
        dummyMove.value = value;
        return dummyMove;
    }
    // -----------------------------------------RECURSIVE CASES--------------------------------------------------
    LegalMove bestMove;
    bestMove.value = (color == WHITE) ? -999999999.0 : 999999999.0;

    for (auto &move : allMoves)
    {
        board.doMove(move);

        move.value = lookIntoFutureMoves(color * -1, depth + 1, alpha, beta).value;

        board.undoMove(move);

        // update bestMove
        if (color == WHITE)
        {
            if (move.value > bestMove.value)
            {
                bestMove = move;
            }
            alpha = std::max(alpha, move.value);
            if (alpha >= beta)
            {
                break;
            }
        }
        if (color == BLACK)
        {
            if (move.value < bestMove.value)
            {
                bestMove = move;
            }

            beta = std::min(beta, move.value);
            if (alpha >= beta)
            {
                break;
            }
        }
    }

    return bestMove;
}
