#ifndef BOARD_HPP
#define BOARD_HPP

#include "legalMove.hpp"
#include <array>
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "chengine/pawnStructureEvaluator.hpp"
class MoveGetter;
class SBAnalyzer;

enum CastlingRight
{
    WHITE_KINGSIDE = 1 << 0,  // 0001
    WHITE_QUEENSIDE = 1 << 1, // 0010
    BLACK_KINGSIDE = 1 << 2,  // 0100
    BLACK_QUEENSIDE = 1 << 3  // 1000
};

class Board
{
    friend class MoveGetter;
    friend class SBAnalyzer;

public:
    void doMove(LegalMove &move, sf::RenderWindow *window = nullptr, bool fromUser = false);
    bool isKingInCheck(int color);
    void castle(LegalMove &move);
    void handlePromotion(LegalMove &move);
    bool isThreefoldRepetition() const;
    void removePositionFromColorTracker(int color, int newCol, int newRow);
    void addPositionToColorTracker(int color, int newCol, int newRow);
    void updateKnownPositions(LegalMove &move);
    void undoMove(LegalMove &move);
    bool testMoveCheckLegality(LegalMove &move);
    void verifyTrackerConsistency() const;
    void printPositionTrackerAsBoard() const;
    int showPromotionMenu(sf::RenderWindow *window, int color);
    std::vector<std::tuple<int, int>> getBlackMoves();
    std::vector<std::tuple<int, int>> getWhiteMoves();
    int getMoveCount() const;
    PawnStructureEvaluator getPawnStructureEvaluator() const &;
    Board();
    const std::array<std::array<int, 8>, 8> &getSquares() const;
    const uint64_t &getHash();
    bool hasMoved(int col, int row) const;
    LegalMove getLastMove();

    const std::array<std::array<bool, 8>, 8> &getMovesArray();
    Board(const Board &other);

private:
    PawnStructureEvaluator pawnEvaluator;
    int movesDone = 0;
    void removeCastlingRight(CastlingRight right);
    uint64_t zobristSideToMove;
    uint64_t currentHash;
    uint64_t zobrist[8][8][12];

    uint8_t castlingRights;
    uint64_t castlingZobrist[4];

    uint64_t enPassantFileZobrist[8];
    int enPassantFile = -1;

    std::tuple<int, int> whiteKingPosition;
    std::tuple<int, int> blackKingPosition;
    LegalMove lastMove;
    std::array<std::array<bool, 8>, 8> hasMovedArray;
    std::vector<std::tuple<int, int>> blackPositions;
    std::vector<std::tuple<int, int>> whitePositions;
    void initilizeBoard();
    std::array<std::array<int, 8>, 8> squares;

    std::unordered_map<uint64_t, int> repetitionCount;
    std::vector<uint64_t> positionHistory;
};

#endif