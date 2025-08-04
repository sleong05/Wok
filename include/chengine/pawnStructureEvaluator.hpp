#pragma once
#include "legalMove.hpp"

class PawnStructureEvaluator
{
public:
    double evaluatePawnStructure();
    void updatePawnStructure(LegalMove &move, bool undoMove = false);
    void printPawnStructure();

    PawnStructureEvaluator();

private:
    bool isPawnMove(LegalMove &move);
    bool isPawnCapture(LegalMove &move);

    void adjustPawnCapture(LegalMove &move, bool undoMove);
    void adjustPawnMovement(LegalMove &move, bool undoMove);

    std::vector<int> whitePawns;
    std::vector<int> blackPawns;
};