#include "moveGetter.hpp"
#include "constants.hpp"
#include "legalMove.hpp"
#include "weights.hpp"
#include "chengine/staticBoardAnalyzer.hpp"
using namespace constants;

LegalMove quiesceSearch(Board &board, int color, double alpha, double beta, int depthFromRoot, int depth =0) {
    // checkmate behavior
    if (!MoveGetter::hasMoveLeft(color, board)) {
    double value = board.isKingInCheck(color) ? (weights::MATE - (100 * depthFromRoot)) * color : weights::DRAW; // depth = 0 here
    LegalMove mateMove;
    mateMove.value = value;
    return mateMove;
}


    LegalMove bestMove;
    double standPat = SBAnalyzer::evaluateBoard(board);
    bestMove.value = standPat;

    if (depth >weights::MAX_QUIESCE_DEPTH) return bestMove;
    // alpha-beta on static eval
    if (color == WHITE) {
        if (standPat >= beta) return bestMove;
        alpha = std::max(alpha, standPat);
    } else {
        if (standPat <= alpha) return bestMove;
        beta = std::min(beta, standPat);
    }

    // get capturing moves
    std::vector<LegalMove> moves = MoveGetter::getMovesForTeam(color, board);
moves.erase(std::remove_if(moves.begin(), moves.end(), [](const LegalMove &move) {
                return move.pieceAtEnd == EMPTY;
            }), moves.end());
    


    if (moves.empty()) {
    return bestMove; 
}

 for (auto &move : moves)
        {
            move.computePriority();
        }

        std::stable_sort(moves.begin(), moves.end(), [](const LegalMove &a, const LegalMove &b)
                         { return a.priorityOfSearchValue > b.priorityOfSearchValue; });

    for (auto &move : moves) {
        board.doMove(move);
        double score = quiesceSearch(board, -color, alpha, beta, depthFromRoot+1, ++depth).value;
        board.undoMove(move);

        if (color == WHITE) {
            if (score > bestMove.value) bestMove = move, bestMove.value = score;
            alpha = std::max(alpha, score);
            if (alpha >= beta) break;
        } else {
            if (score < bestMove.value) bestMove = move, bestMove.value = score;
            beta = std::min(beta, score);
            if (alpha >= beta) break;
        }
    }

    return bestMove;
}
