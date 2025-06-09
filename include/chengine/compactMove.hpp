#pragma once
#include <cstdint>
#include "legalMove.hpp"

struct CompactMove
{
    uint8_t fromCol : 3;
    uint8_t fromRow : 3;
    uint8_t toCol : 3;
    uint8_t toRow : 3;

    uint8_t pieceToMove : 5;
    uint8_t pieceAtEnd : 5;
    uint8_t promotionPiece : 5; // 0 if not used

    uint8_t isCastle : 1;
    uint8_t isPromotion : 1;
    uint8_t isEnPassant : 1;
    uint8_t fromHasMoved : 1;
    uint8_t toHasMoved : 1;
};

inline CompactMove toCompactMove(const LegalMove &lm)
{
    auto [toCol, toRow] = lm.to;
    auto [fromCol, fromRow] = lm.from;

    CompactMove cm{};
    cm.fromCol = static_cast<uint8_t>(fromCol);
    cm.fromRow = static_cast<uint8_t>(fromRow);
    cm.toCol = static_cast<uint8_t>(toCol);
    cm.toRow = static_cast<uint8_t>(toRow);

    cm.pieceToMove = static_cast<uint8_t>(lm.pieceToMove);
    cm.pieceAtEnd = static_cast<uint8_t>(lm.pieceAtEnd);
    cm.promotionPiece = static_cast<uint8_t>(lm.promotionPiece);

    cm.isCastle = lm.isCastle;
    cm.isPromotion = lm.isPromotion;
    cm.isEnPassant = lm.isEnPassant;
    cm.fromHasMoved = lm.fromHasMoved;
    cm.toHasMoved = lm.toHasMoved;

    return cm;
}

inline LegalMove toLegalMove(const CompactMove &cm)
{
    LegalMove lm;

    lm.from = std::make_tuple(static_cast<int>(cm.fromCol), static_cast<int>(cm.fromRow));
    lm.to = std::make_tuple(static_cast<int>(cm.toCol), static_cast<int>(cm.toRow));

    lm.pieceToMove = static_cast<int>(cm.pieceToMove);
    lm.pieceAtEnd = static_cast<int>(cm.pieceAtEnd);
    lm.promotionPiece = static_cast<int>(cm.promotionPiece);

    lm.isCastle = cm.isCastle;
    lm.isPromotion = cm.isPromotion;
    lm.isEnPassant = cm.isEnPassant;
    lm.fromHasMoved = cm.fromHasMoved;
    lm.toHasMoved = cm.toHasMoved;

    return lm;
}
