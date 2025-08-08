#pragma once
#include "legalMove.hpp"
#include "board.hpp"
#include "minMaxTree.hpp"
#include <atomic>
#include <thread>
class Chengine
{
public:
    Chengine(Board &board);
    void loadFEN(const std::string &fen);
    std::string findBestMove();
    LegalMove getMove(Board &board, int color, int startDepth = 2);
    void startPondering(Board board, int color);
    int stopPondering();

private:
    std::chrono::duration<double> searchTime = std::chrono::duration<double>(3.0);
    int ponderDepth = 0;
    std::atomic<bool> stopPonder = false;
    std::thread ponderingThread;

    MinMaxTree miniMax;
    Board &board;
};