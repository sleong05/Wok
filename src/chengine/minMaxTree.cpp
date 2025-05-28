#include "chengine/minMaxTree.hpp"
#include "chengine/staticBoardAnalyzer.hpp"
#include "legalMove.hpp"
#include "board.hpp"
#include "moveGetter.hpp"
#include "weights.hpp"
#include "constants.hpp"
#include <identifier.hpp>
#include <algorithm>
#include <string>
#include <fstream>
#include <limits>
using namespace constants;

MinMaxTree::MinMaxTree(Board &board) : board(board)
{
}

LegalMove MinMaxTree::getBestMove(int color) // chengine is black so make them alkways look for the lowest value move
{
    double INF = 1000000000.0;
    std::ofstream clearLog("move_evaluations.log", std::ios::trunc);
    clearLog.close();

    return lookIntoFutureMoves(color, 1, -INF, INF);
};

LegalMove MinMaxTree::lookIntoFutureMoves(int color, int depth, double alpha, double beta)
{
    // std::ofstream logFile("move_evaluations.log", std::ios::app); // logger
    //    -----------------------------------------BASE CASES--------------------------------------------------
    //    base case mate/draw
    if (not MoveGetter::hasMoveLeft(color, board))
    {
        double value = board.isKingInCheck(color) ? (weights::MATE * color) - (depth * 100) : weights::DRAW;

        LegalMove dummyMove = LegalMove();
        dummyMove.value = value;
        return dummyMove;
    }

    // base case depth hit
    if (depth == weights::MAX_DEPTH)
    {
        double value = SBAnalyzer::evaluateBoard(board);
        LegalMove dummyMove = LegalMove();
        dummyMove.value = value;
        return dummyMove;
    }
    // -----------------------------------------RECURSIVE CASES--------------------------------------------------
    auto allMoves = MoveGetter::getMovesForTeam(color, board);
    LegalMove bestMove;
    double INF = std::numeric_limits<double>::infinity();
    bestMove.value = (color == WHITE) ? -INF : INF;

    for (auto &move : allMoves)
    {
        // std::cout << std::string(depth * 4, ' ') << move << std::endl;
        board.doMove(move);

        move.value = lookIntoFutureMoves(color * -1, depth + 1, alpha, beta).value;

        board.undoMove(move);

        // --- LOG MOVE & EVALUATION ---
        /*
                if (logFile.is_open())
                {
                    // indentation for depth
                    logFile << std::string((depth - 1) * 4, ' ')
                            << Identifier::getPieceName(move.pieceToMove) << " to " << std::get<0>(move.to) << ", " << std::get<1>(move.to) << " = "
                            << move.value << std::endl;
                }
        */
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
                // std::cout << std::string((depth + 1) * 4, ' ') << "PRUNING" << std::endl;
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
                // std::cout << std::string((depth + 1) * 4, ' ') << "PRUNING" << std::endl;
                break;
            }
        }
    }

    return bestMove;
}
