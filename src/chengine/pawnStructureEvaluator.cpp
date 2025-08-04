#include "chengine/pawnStructureEvaluator.hpp"
#include "constants.hpp"
#include "identifier.hpp"
#include "weights.hpp"

using namespace constants;
using namespace weights;
using namespace Identifier;
double PawnStructureEvaluator::evaluatePawnStructure()
{
    double pawnScore = 0;
    // black
    for (int i = 0; i < 8; i++)
    {
        if (blackPawns[i] == 0)
            continue;
        // doubled pawns
        if (blackPawns[i] > 1)
            pawnScore += blackPawns[i] / 4.0; // double pawn = -.5, triple = -.75

        // isolated
        if ((i == 0 or blackPawns[i - 1] == 0) and (i == 7 or blackPawns[i + 1] == 0))
            pawnScore += ISOLATED_PAWN;
        // passed pawn
        if (whitePawns[i] == 0 and (i == 0 or whitePawns[i - 1] == 0) and (i == 7 or whitePawns[i + 1] == 0))
            pawnScore -= PASSED_PAWN_VALUE;
    }

    // white
    for (int i = 0; i < 8; i++)
    {
        if (whitePawns[i] == 0)
            continue;
        // doubled pawns
        if (whitePawns[i] > 1)
        {
            pawnScore -= whitePawns[i] / 4.0; // double pawn = -.5, triple = -.75
        }
        // isolated
        if ((i == 0 or whitePawns[i - 1] == 0) and (i == 7 or whitePawns[i + 1] == 0))
            pawnScore -= ISOLATED_PAWN;

        // passed pawn
        if (blackPawns[i] == 0 and (i == 0 or blackPawns[i - 1] == 0) and (i == 7 or blackPawns[i + 1] == 0))
            pawnScore += PASSED_PAWN_VALUE;
    }

    return pawnScore;
}

void PawnStructureEvaluator::updatePawnStructure(LegalMove &move, bool undoMove)
{
    if (isPawnMove(move))
        adjustPawnMovement(move, undoMove);

    if (isPawnCapture(move))
        adjustPawnCapture(move, undoMove);
}

void PawnStructureEvaluator::printPawnStructure()
{
    for (auto &i : blackPawns)
    {
        std::cout << i << " ";
    }
    std::cout << "\n";

    for (auto &i : whitePawns)
    {
        std::cout << i << " ";
    }
    std::cout << "\n";
}
void PawnStructureEvaluator::adjustPawnMovement(LegalMove &move, bool undoMove)
{
    int startCol = std::get<0>(move.from);
    int endCol = std::get<0>(move.to);

    if (startCol == endCol)
        return;

    int color = getTeam(move.pieceToMove);
    auto &pawns = (color == WHITE) ? whitePawns : blackPawns;

    if (undoMove)
    {
        pawns[startCol]++;
        pawns[endCol]--;
    }
    else
    {
        pawns[startCol]--;
        pawns[endCol]++;
    }
}

void PawnStructureEvaluator::adjustPawnCapture(LegalMove &move, bool undoMove)
{
    int endCol = std::get<0>(move.to);
    int color = getTeam(move.pieceToMove);
    auto &pawns = (color == WHITE) ? blackPawns : whitePawns;

    if (undoMove)
    {
        pawns[endCol]++;
    }
    else
    {
        pawns[endCol]--;
    }
}

PawnStructureEvaluator::PawnStructureEvaluator()
{
    blackPawns.assign(8, 1);
    whitePawns.assign(8, 1);
}

bool PawnStructureEvaluator::isPawnMove(LegalMove &move)
{
    return move.pieceToMove == BLACK_PAWN || move.pieceToMove == WHITE_PAWN;
}

bool PawnStructureEvaluator::isPawnCapture(LegalMove &move)
{
    return move.pieceAtEnd == BLACK_PAWN || move.pieceAtEnd == WHITE_PAWN;
}