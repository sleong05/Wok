#include <string>
#include <emscripten.h>
#include "chengine/chengine.hpp"
#include "board.hpp"
#include "moveGetter.hpp"
#include <legalMove.hpp>
#include "constants.hpp"

Board board = Board();
Chengine engine = Chengine(board); // global and reused across calls

extern "C"
{

    EMSCRIPTEN_KEEPALIVE
    const char *getBestMove(const char *fen, bool isWhite)
    {
        static std::string returnBuffer;

        int color = (isWhite) ? constants::WHITE : constants::BLACK;
        LegalMove bestMove = engine.getMove(board, color);

        auto [fromCol, fromRow] = bestMove.from;
        auto [toCol, toRow] = bestMove.to;

        char fromFile = 'a' + fromCol;
        char fromRank = '8' - fromRow;
        char toFile = 'a' + toCol;
        char toRank = '8' - toRow;

        returnBuffer = {fromFile, fromRank, toFile, toRank};

        return returnBuffer.c_str();
    }

    EMSCRIPTEN_KEEPALIVE
    void makeMove(const char *moveStr, bool isWhite)
    {
        int fromRow, fromCol, toRow, toCol;
        sscanf(moveStr, "%d,%d,%d,%d", &fromRow, &fromCol, &toRow, &toCol);

        int color = (isWhite) ? constants::WHITE : constants::BLACK;
        std::vector<LegalMove> allMoves = MoveGetter::getMovesForTeam(color, board);

        for (auto &move : allMoves)
        {
            if (move.from == std::make_tuple(fromCol, fromRow) and move.to == std::make_tuple(toCol, toRow))
            {
                board.doMove(move);
                break;
            }
        }
    }
}
