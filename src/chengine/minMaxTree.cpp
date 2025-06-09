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
#include <chrono>
using namespace constants;

static int leafCount = 0;
static int pruneCount1 = 0;
static int pruneCount2 = 0;
static int pruneCount3 = 0;
static int pruneCount4 = 0;
static int pruneCount5 = 0;
static int totalNodesVisited = 0;
static std::vector<int> movesLookedAtBeforePrune;

MinMaxTree::MinMaxTree(Board &board) : board(board)
{
}

LegalMove MinMaxTree::getBestMove(int color) // chengine is black so make them alkways look for the lowest value move
{
    double INF = 1000000000.0;
    // std::ofstream clearLog("move_evaluations.log", std::ios::trunc);
    // clearLog.close();
    auto start = std::chrono::high_resolution_clock::now();

    auto blackMoves = MoveGetter::getMovesForTeam(BLACK, board);
    auto whiteMoves = MoveGetter::getMovesForTeam(WHITE, board);

    int totalMoves = blackMoves.size() + whiteMoves.size();
    std::cout << "Number of available moves = " << totalMoves << std::endl;

    LegalMove bestMove = lookIntoFutureMoves(color, 1, -INF, INF);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "-------------------------------------------- \n";
    std::cout << "lookIntoFutureMoves took " << duration.count() << " seconds\n";

    std::cout << "Leaf evaluations: " << leafCount << std::endl;
    std::cout << "Prunes1: " << pruneCount2 << std::endl;
    std::cout << "Total Nodes Visited(no leafs): " << totalNodesVisited << std::endl;

    double averageMovesBeforePrune = 0.0;
    for (const auto &numOfMoves : movesLookedAtBeforePrune)
    {
        averageMovesBeforePrune += numOfMoves;
    }
    std::cout << "Average number of moves before prune: " << averageMovesBeforePrune / movesLookedAtBeforePrune.size() << std::endl;

    return bestMove;
};

LegalMove MinMaxTree::lookIntoFutureMoves(int color, int depth, double alpha, double beta)
{
    // std::ofstream logFile("move_evaluations.log", std::ios::app); // logger
    //      -----------------------------------------BASE CASES--------------------------------------------------
    //      base case mate/draw
    if (not MoveGetter::hasMoveLeft(color, board))
    {
        double value = board.isKingInCheck(color) ? (weights::MATE - (depth * 100)) * color : weights::DRAW;

        LegalMove dummyMove = LegalMove();
        dummyMove.value = value;
        return dummyMove;
    }

    // base case depth hit
    if (depth == weights::MAX_DEPTH)
    {
        leafCount++;
        double value = SBAnalyzer::evaluateBoard(board);
        LegalMove dummyMove = LegalMove();
        dummyMove.value = value;
        return dummyMove;
    }
    // -----------------------------------------RECURSIVE CASES--------------------------------------------------
    auto allMoves = MoveGetter::getMovesForTeam(color, board);

    if (depth <= 5)
    {
        for (auto &move : allMoves)
        {
            move.computePriority();
        }

        std::stable_sort(allMoves.begin(), allMoves.end(), [](const LegalMove &a, const LegalMove &b)
                         { return a.priorityOfSearchValue > b.priorityOfSearchValue; });
    }
    LegalMove bestMove;
    double INF = std::numeric_limits<double>::infinity();
    bestMove.value = (color == WHITE) ? -INF : INF;

    int numberOfChildrenTraveresed = 0;
    for (auto &move : allMoves)
    {
        numberOfChildrenTraveresed++;
        totalNodesVisited++;

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
                movesLookedAtBeforePrune.push_back(numberOfChildrenTraveresed);
                switch (depth)
                {
                case 1:
                    pruneCount1++;
                    break;
                case 2:
                    pruneCount2++;
                    break;
                case 3:
                    pruneCount3++;
                    break;
                case 4:
                    pruneCount4++;
                    break;
                case 5:
                    pruneCount5++;
                    break;
                default:
                    break;
                }
                break;
            }
        }
        else if (color == BLACK)
        {
            if (move.value < bestMove.value)
            {
                bestMove = move;
            }

            beta = std::min(beta, move.value);

            if (alpha >= beta)
            {
                switch (depth)
                {
                case 1:
                    pruneCount1++;
                    break;
                case 2:
                    pruneCount2++;
                    break;
                case 3:
                    pruneCount3++;
                    break;
                case 4:
                    pruneCount4++;
                    break;
                case 5:
                    pruneCount5++;
                    break;
                default:
                    break;
                }
                break;
            }
        }
    }
    return bestMove;
}
