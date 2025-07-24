#include <SFML/Graphics.hpp>
#include "identifier.hpp"
#include "board.hpp"
#include "constants.hpp"
#include "moveGetter.hpp"
#include "squareAttacker.hpp"
#include <random>
#include "pieces/king.hpp"

using namespace constants;

Board::Board() : lastMove(constants::NO_TILE_SELECTED, constants::NO_TILE_SELECTED, constants::EMPTY, constants::EMPTY)
{
    initilizeBoard();
}

void Board::initilizeBoard()
{
    whiteKingPosition = {4, 7};
    blackKingPosition = {4, 0};
    whitePositions = {{7, 7}, {6, 7}, {5, 7}, {4, 7}, {3, 7}, {2, 7}, {1, 7}, {0, 7}, {7, 6}, {6, 6}, {5, 6}, {4, 6}, {3, 6}, {2, 6}, {1, 6}, {0, 6}};
    blackPositions = {{7, 0}, {6, 0}, {5, 0}, {4, 0}, {3, 0}, {2, 0}, {1, 0}, {0, 0}, {7, 1}, {6, 1}, {5, 1}, {4, 1}, {3, 1}, {2, 1}, {1, 1}, {0, 1}};
    hasMovedArray = {{{false, false, false, false, false, false, false, false},
                      {false, false, false, false, false, false, false, false},
                      {false, false, false, false, false, false, false, false},
                      {false, false, false, false, false, false, false, false},
                      {false, false, false, false, false, false, false, false},
                      {false, false, false, false, false, false, false, false},
                      {false, false, false, false, false, false, false, false},
                      {false, false, false, false, false, false, false, false}}};

    squares = {
        std::array<int, 8>{BLACK_ROOK, BLACK_KNIGHT, BLACK_BISHOP, BLACK_QUEEN, BLACK_KING, BLACK_BISHOP, BLACK_KNIGHT, BLACK_ROOK},
        std::array<int, 8>{BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN},
        std::array<int, 8>{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        std::array<int, 8>{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        std::array<int, 8>{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        std::array<int, 8>{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        std::array<int, 8>{WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN},
        std::array<int, 8>{WHITE_ROOK, WHITE_KNIGHT, WHITE_BISHOP, WHITE_QUEEN, WHITE_KING, WHITE_BISHOP, WHITE_KNIGHT, WHITE_ROOK}};

    // set up hashing
    std::uniform_int_distribution<uint64_t> dist;
    std::mt19937_64 rng(1337);
    for (int row = 0; row < 8; ++row)
    {
        for (int col = 0; col < 8; ++col)
        {
            for (int piece = 0; piece < 12; ++piece)
            {
                zobrist[row][col][piece] = dist(rng);
            }
        }
    }
    uint64_t hash = 0;
    for (int row = 0; row < 8; ++row)
    {
        for (int col = 0; col < 8; ++col)
        {
            int piece = squares[row][col];
            if (piece != EMPTY)
            {
                hash ^= zobrist[row][col][piece - 7];
            }
        }
    }
    currentHash = hash;
    zobristSideToMove = dist(rng);

    castlingRights = 0b1111;
    for (int i = 0; i < 4; ++i)
    {
        castlingZobrist[i] = dist(rng);
    }

    for (int i = 0; i < 4; ++i)
    {
        if (castlingRights & (1 << i))
            currentHash ^= castlingZobrist[i];
    }

    for (int file = 0; file < 8; ++file)
        enPassantFileZobrist[file] = dist(rng);

    if (enPassantFile != -1)
        currentHash ^= enPassantFileZobrist[enPassantFile];
}

void Board::removeCastlingRight(CastlingRight right)
{
    if (castlingRights & right)
    {
        currentHash ^= castlingZobrist[__builtin_ctz(right)]; // get index 0–3
        castlingRights &= ~right;                             // clear the right
    }
}

inline void fastRemove(std::vector<std::tuple<int, int>> &vec, const std::tuple<int, int> &target)
{
    for (size_t i = vec.size() - 1; i < vec.size(); --i)
    {
        if (vec[i] == target)
        {
            vec[i] = vec.back();
            vec.pop_back();
            return;
        }
    }
}

const std::array<std::array<int, 8>, 8> &Board::getSquares() const
{
    return squares;
}

const uint64_t &Board::getHash() { return currentHash; }

bool Board::hasMoved(int col, int row) const
{
    return hasMovedArray[row][col];
}

LegalMove Board::getLastMove()
{
    return lastMove;
}

const std::array<std::array<bool, 8>, 8> &Board::getMovesArray()
{
    return hasMovedArray;
}

Board::Board(const Board &other)
    : zobristSideToMove(other.zobristSideToMove),
      currentHash(other.currentHash),
      zobrist{},
      castlingRights(other.castlingRights),
      castlingZobrist{
          other.castlingZobrist[0],
          other.castlingZobrist[1],
          other.castlingZobrist[2],
          other.castlingZobrist[3]},
      enPassantFileZobrist{
          other.enPassantFileZobrist[0],
          other.enPassantFileZobrist[1],
          other.enPassantFileZobrist[2],
          other.enPassantFileZobrist[3],
          other.enPassantFileZobrist[4],
          other.enPassantFileZobrist[5],
          other.enPassantFileZobrist[6],
          other.enPassantFileZobrist[7]},
      enPassantFile(other.enPassantFile),
      whiteKingPosition(other.whiteKingPosition),
      blackKingPosition(other.blackKingPosition),
      lastMove(other.lastMove),
      hasMovedArray(other.hasMovedArray),
      blackPositions(other.blackPositions),
      whitePositions(other.whitePositions),
      squares(other.squares),
      repetitionCount(other.repetitionCount),
      positionHistory(other.positionHistory)
{
    for (int x = 0; x < 8; ++x)
        for (int y = 0; y < 8; ++y)
            for (int p = 0; p < 12; ++p)
                zobrist[x][y][p] = other.zobrist[x][y][p];
}

void Board::doMove(LegalMove &move, sf::RenderWindow *window, bool fromUser)
{
    movesDone++;
    move.oldEnPassantFile = enPassantFile;
    move.oldCastlingRights = castlingRights;
    // en passant hashing
    if (enPassantFile != -1)
        currentHash ^= enPassantFileZobrist[enPassantFile];
    enPassantFile = -1;

    if (move.pieceToMove == WHITE_PAWN && std::get<1>(move.from) == 6 && std::get<1>(move.to) == 4)
    {
        enPassantFile = std::get<0>(move.from);
    }
    else if (move.pieceToMove == BLACK_PAWN && std::get<1>(move.from) == 1 && std::get<1>(move.to) == 3)
    {
        enPassantFile = std::get<0>(move.from);
    }

    // Add new en passant hash
    if (enPassantFile != -1)
        currentHash ^= enPassantFileZobrist[enPassantFile];

    currentHash ^= zobristSideToMove;

    auto [oldCol, oldRow] = move.from;
    auto [newCol, newRow] = move.to;

    // store hasMove
    move.fromHasMoved = hasMovedArray[oldRow][oldCol];
    move.toHasMoved = hasMovedArray[newRow][newCol];

    // update castling rights
    // Castling rights removal (king or rook moved)

    if (move.pieceToMove == WHITE_KING)
    {
        removeCastlingRight(WHITE_KINGSIDE);
        removeCastlingRight(WHITE_QUEENSIDE);
    }
    if (move.pieceToMove == BLACK_KING)
    {
        removeCastlingRight(BLACK_KINGSIDE);
        removeCastlingRight(BLACK_QUEENSIDE);
    }
    if (move.pieceToMove == WHITE_ROOK)
    {
        auto [fromCol, fromRow] = move.from;
        if (fromCol == 7 && fromRow == 7)
            removeCastlingRight(WHITE_KINGSIDE);
        if (fromCol == 0 && fromRow == 7)
            removeCastlingRight(WHITE_QUEENSIDE);
    }
    if (move.pieceToMove == BLACK_ROOK)
    {
        if (move.from == std::make_tuple(7, 0))
            removeCastlingRight(BLACK_KINGSIDE);
        if (move.from == std::make_tuple(0, 0))
            removeCastlingRight(BLACK_QUEENSIDE);
    }

    // update tracked positions
    updateKnownPositions(move);

    // move piece
    squares[oldRow][oldCol] = constants::EMPTY;
    squares[newRow][newCol] = move.pieceToMove;

    // update hasing
    currentHash ^= zobrist[oldRow][oldCol][move.pieceToMove - 7];
    if (move.pieceAtEnd != constants::EMPTY)
    {
        if (move.pieceAtEnd == WHITE_ROOK)
        {
            if (move.to == std::make_tuple(7, 7))
                removeCastlingRight(WHITE_KINGSIDE);
            if (move.to == std::make_tuple(0, 7))
                removeCastlingRight(WHITE_QUEENSIDE);
        }
        if (move.pieceAtEnd == BLACK_ROOK)
        {
            if (move.to == std::make_tuple(7, 0))
                removeCastlingRight(BLACK_KINGSIDE);
            if (move.to == std::make_tuple(0, 0))
                removeCastlingRight(BLACK_QUEENSIDE);
        }

        currentHash ^= zobrist[newRow][newCol][move.pieceAtEnd - 7];
    }
    currentHash ^= zobrist[newRow][newCol][move.pieceToMove - 7];

    // update tracked info
    lastMove = move;
    hasMovedArray[oldRow][oldCol] = true;
    hasMovedArray[newRow][newCol] = true;

    // Do additonal moves for special moves
    if (move.isEnPassant)
    {
        int color = Identifier::getTeam(move.pieceToMove);
        int capturedPawn = (color == WHITE) ? BLACK_PAWN : WHITE_PAWN;

        squares[newRow - color][newCol] = constants::EMPTY;
        currentHash ^= zobrist[newRow - color][newCol][capturedPawn - 7];

        removePositionFromColorTracker(color * -1, newCol, newRow - color);
    }

    if (move.isCastle)
    {
        castle(move);
    }

    // promotion logic

    if (move.isPromotion)
    {

        if (fromUser) // only prompt if from gui
        {
            int color = Identifier::getTeam(move.pieceToMove);
            move.promotionPiece = showPromotionMenu(window, color);
        }
        handlePromotion(move);
    }

    // update king position
    if (move.pieceToMove == WHITE_KING)
    {
        whiteKingPosition = move.to;
    }
    else if (move.pieceToMove == BLACK_KING)
    {
        blackKingPosition = move.to;
    }

    // keep track of hashs for threefold repitition
    auto hash = getHash();
    positionHistory.push_back(hash);
    repetitionCount[hash]++;
}

void Board::castle(LegalMove &move)
{
    int color = Identifier::getTeam(move.pieceToMove);
    int rook = (color == WHITE) ? WHITE_ROOK : BLACK_ROOK;
    auto [kingCol, kingRow] = move.to;

    if (kingCol == 2) // long castle
    {
        squares[kingRow][3] = squares[kingRow][0];
        squares[kingRow][0] = constants::EMPTY;
        currentHash ^= zobrist[kingRow][3][rook - 7];
        currentHash ^= zobrist[kingRow][0][rook - 7];
        hasMovedArray[kingRow][0] = true;
        removePositionFromColorTracker(color, 0, kingRow);
        addPositionToColorTracker(color, 3, kingRow);
    }
    else if ((kingCol == 6)) // short castle
    {
        squares[kingRow][5] = squares[kingRow][7];
        squares[kingRow][7] = constants::EMPTY;
        currentHash ^= zobrist[kingRow][5][rook - 7];
        currentHash ^= zobrist[kingRow][7][rook - 7];
        hasMovedArray[kingRow][7] = true;
        removePositionFromColorTracker(color, 7, kingRow);
        addPositionToColorTracker(color, 5, kingRow);
    }
}

void Board::handlePromotion(LegalMove &move)
{
    int color = Identifier::getTeam(move.pieceToMove);
    auto [col, row] = move.to;
    if (move.promotionPiece == constants::EMPTY)
        move.promotionPiece = (color == WHITE) ? WHITE_QUEEN : BLACK_QUEEN;

    squares[row][col] = move.promotionPiece;
    currentHash ^= zobrist[row][col][move.pieceToMove - 7];
    currentHash ^= zobrist[row][col][move.promotionPiece - 7];
}

bool Board::isThreefoldRepetition() const
{
    uint64_t current = currentHash;
    auto it = repetitionCount.find(current);
    return (it != repetitionCount.end() && it->second >= 3);
}

void Board::undoMove(LegalMove &move)
{
    movesDone--;
    // update info for threefold repition
    uint64_t hash = positionHistory.back();
    positionHistory.pop_back();
    repetitionCount[hash]--;
    if (repetitionCount[hash] == 0)
        repetitionCount.erase(hash);

    // undo move start
    auto [oldCol, oldRow] = move.from;
    auto [newCol, newRow] = move.to;

    // update known positions
    if (Identifier::getTeam(move.pieceToMove) == WHITE)
    {
        // Remove piece from where it was
        fastRemove(whitePositions, move.to);

        // Add back to where it was
        whitePositions.push_back(move.from);
        // add captured piece back
        if (move.pieceAtEnd != EMPTY)
        {
            blackPositions.push_back(move.to);
        }
    }
    else
    {
        // Remove piece from where it was
        fastRemove(blackPositions, move.to);

        // Add back to where it was
        blackPositions.push_back(move.from);
        // add captured piece back
        if (move.pieceAtEnd != EMPTY)
        {
            whitePositions.push_back(move.to);
        }
    }
    squares[oldRow][oldCol] = move.pieceToMove;
    squares[newRow][newCol] = move.pieceAtEnd;

    // undo moves on hasmoved array
    hasMovedArray[oldRow][oldCol] = move.fromHasMoved;
    hasMovedArray[newRow][newCol] = move.toHasMoved;

    if (move.isEnPassant)
    {
        int color = Identifier::getTeam(move.pieceToMove);
        int capturedPawn = (color == WHITE) ? BLACK_PAWN : WHITE_PAWN;
        squares[newRow - color][newCol] = (color == WHITE) ? BLACK_PAWN : WHITE_PAWN;
        currentHash ^= zobrist[newRow - color][newCol][capturedPawn - 7];
        addPositionToColorTracker(color * -1, newCol, newRow - color);
    }

    if (move.isCastle)
    {
        int color = Identifier::getTeam(move.pieceToMove);
        int rook = (color == WHITE) ? WHITE_ROOK : BLACK_ROOK;
        auto [kingCol, kingRow] = move.to;

        if (kingCol == 2) // long castle
        {
            squares[kingRow][0] = squares[kingRow][3];
            squares[kingRow][3] = EMPTY;
            currentHash ^= zobrist[kingRow][0][rook - 7];
            currentHash ^= zobrist[kingRow][3][rook - 7];

            hasMovedArray[kingRow][0] = false;
            removePositionFromColorTracker(color, 3, kingRow);
            addPositionToColorTracker(color, 0, kingRow);
        }
        else if ((kingCol == 6)) // short castle
        {
            squares[kingRow][7] = squares[kingRow][5];
            squares[kingRow][5] = EMPTY;
            currentHash ^= zobrist[kingRow][7][rook - 7];
            currentHash ^= zobrist[kingRow][5][rook - 7];

            hasMovedArray[kingRow][7] = false;
            removePositionFromColorTracker(color, 5, kingRow);
            addPositionToColorTracker(color, 7, kingRow);
        }
    }

    if (move.isPromotion)
    {
        currentHash ^= zobrist[newRow][newCol][move.promotionPiece - 7];
        currentHash ^= zobrist[newRow][newCol][move.pieceToMove - 7];
    }
    if (move.pieceToMove == WHITE_KING)
    {
        whiteKingPosition = move.from;
    }
    else if (move.pieceToMove == BLACK_KING)
    {
        blackKingPosition = move.from;
    }
    // undo zobrist hashiong
    currentHash ^= zobrist[newRow][newCol][move.pieceToMove - 7];

    if (move.pieceAtEnd != constants::EMPTY)
    {
        currentHash ^= zobrist[newRow][newCol][move.pieceAtEnd - 7];
    }

    currentHash ^= zobrist[oldRow][oldCol][move.pieceToMove - 7];

    // Undo en passant hash
    if (enPassantFile != -1)
        currentHash ^= enPassantFileZobrist[enPassantFile]; // remove current

    enPassantFile = move.oldEnPassantFile;

    if (enPassantFile != -1)
        currentHash ^= enPassantFileZobrist[enPassantFile]; // restore old

    currentHash ^= zobristSideToMove;
    // --- Castling rights undo ---
    if (castlingRights != move.oldCastlingRights)
    {
        // Remove current rights from hash
        for (int i = 0; i < 4; ++i)
        {
            if (castlingRights & (1 << i))
                currentHash ^= castlingZobrist[i];
        }

        // Add back old rights to hash
        for (int i = 0; i < 4; ++i)
        {
            if (move.oldCastlingRights & (1 << i))
                currentHash ^= castlingZobrist[i];
        }

        castlingRights = move.oldCastlingRights;
    }
}

bool Board::testMoveCheckLegality(LegalMove &move)
{
    LegalMove actualLastMove = lastMove;
    doMove(move);
    bool inCheck = isKingInCheck(Identifier::getTeam(move.pieceToMove));
    undoMove(move);
    lastMove = actualLastMove;
    return not inCheck;
}

bool Board::isKingInCheck(int color)
{
    auto [kingCol, kingRow] = (color == WHITE) ? whiteKingPosition : blackKingPosition;
    return squareAttacker::isSquareUnderAttack(kingCol, kingRow, color, getSquares());
}

void Board::removePositionFromColorTracker(int color, int newCol, int newRow)
{
    if (color == constants::BLACK)
    {
        fastRemove(blackPositions, std::make_tuple(newCol, newRow));
    }
    else
    {
        fastRemove(whitePositions, std::make_tuple(newCol, newRow));
    }
}

void Board::addPositionToColorTracker(int color, int newX, int newY)
{
    if (color == constants::BLACK)
    {
        blackPositions.emplace_back(newX, newY);
    }
    else
    {
        whitePositions.emplace_back(newX, newY);
    }
}

void Board::updateKnownPositions(LegalMove &move)
{
    if (Identifier::getTeam(move.pieceToMove) == WHITE)
    {
        // Remove captured piece
        fastRemove(blackPositions, move.to);

        // Update white piece position
        fastRemove(whitePositions, move.from);
        whitePositions.push_back(move.to);
    }
    else
    {
        // Remove captured piece
        fastRemove(whitePositions, move.to);

        // Update black piece position
        fastRemove(blackPositions, move.from);
        blackPositions.push_back(move.to);
    }
}

// TESTING AND GUI STUFF
void Board::verifyTrackerConsistency() const
{
    std::array<std::array<int, 8>, 8> checkBoard{};

    // Correct: board[row][col] = value
    for (const auto &pos : whitePositions)
        checkBoard[std::get<1>(pos)][std::get<0>(pos)] = 1;

    for (const auto &pos : blackPositions)
        checkBoard[std::get<1>(pos)][std::get<0>(pos)] = 2;

    for (int row = 0; row < 8; ++row)
    {
        for (int col = 0; col < 8; ++col)
        {
            int expected = 0;
            if (squares[row][col] == EMPTY)
                expected = 0;
            else if (Identifier::getTeam(squares[row][col]) == WHITE)
                expected = 1;
            else if (Identifier::getTeam(squares[row][col]) == BLACK)
                expected = 2;

            if (checkBoard[row][col] != expected)
            {
                std::cout << "Mismatch at [" << col << "][" << row << "]: tracker="
                          << checkBoard[row][col] << ", squares=" << expected << "\n";
            }
        }
    }
}

void Board::printPositionTrackerAsBoard() const
{
    std::array<std::array<char, 8>, 8> boardView{};

    for (const auto &pos : whitePositions)
    {
        int col = std::get<0>(pos);
        int row = std::get<1>(pos);
        boardView[row][col] = 'W';
    }

    // Fill black positions
    for (const auto &pos : blackPositions)
    {
        int col = std::get<0>(pos);
        int row = std::get<1>(pos);
        boardView[row][col] = 'B';
    }

    std::cout << "\n   a b c d e f g h\n";
    std::cout << "  -----------------\n";

    for (int row = 7; row >= 0; --row)
    {
        std::cout << row + 1 << "| ";
        for (int col = 0; col < 8; ++col)
        {
            char piece = boardView[row][col];
            std::cout << (piece ? piece : '.') << " ";
        }
        std::cout << "\n";
    }

    std::cout << "  -----------------\n";
}

int Board::showPromotionMenu(sf::RenderWindow *window, int color)
{
    if (!window)
    {
        std::cerr << "No RenderWindow provided to showPromotionMenu.\n";
        return (color == WHITE) ? WHITE_QUEEN : BLACK_QUEEN;
    }

    // Load piece textures
    std::vector<sf::Texture> textures(4);
    std::vector<sf::Sprite> sprites(4);
    const std::vector<std::string> pieceNames = {"Queen", "Rook", "Bishop", "Knight"};

    // Set up promotion types
    std::vector<int> promotionTypes = {
        (color == WHITE) ? WHITE_QUEEN : BLACK_QUEEN,
        (color == WHITE) ? WHITE_ROOK : BLACK_ROOK,
        (color == WHITE) ? WHITE_BISHOP : BLACK_BISHOP,
        (color == WHITE) ? WHITE_KNIGHT : BLACK_KNIGHT};

    // Load textures and create sprites
    for (int i = 0; i < 4; ++i)
    {
        std::string path = "assets/" +
                           std::string((color == WHITE) ? "white" : "black") +
                           pieceNames[i] + ".png";

        if (!textures[i].loadFromFile(path))
        {
            std::cerr << "Failed to load " << path << "\n";
            return (color == WHITE) ? WHITE_QUEEN : BLACK_QUEEN;
        }

        sprites[i].setTexture(textures[i]);
        sprites[i].setScale(1.5f, 1.5f); // Scale up if needed
    }

    // Get window dimensions
    float windowWidth = static_cast<float>(window->getSize().x);
    float windowHeight = static_cast<float>(window->getSize().y);

    // Centered background box
    sf::Vector2f dialogSize(400.f, 150.f);
    sf::Vector2f dialogPos((windowWidth - dialogSize.x) / 2.f, (windowHeight - dialogSize.y) / 2.f);

    sf::RectangleShape background(dialogSize);
    background.setFillColor(sf::Color(50, 50, 50, 220));
    background.setOutlineThickness(3);
    background.setOutlineColor(sf::Color::White);
    background.setPosition(dialogPos);

    // Position sprites evenly inside the box
    float spacing = 20.f;
    float iconWidth = textures[0].getSize().x * 1.5f; // Scaled width
    float totalWidth = 4 * iconWidth + 3 * spacing;
    float startX = dialogPos.x + (dialogSize.x - totalWidth) / 2.f;
    float y = dialogPos.y + 30.f;

    for (int i = 0; i < 4; ++i)
    {
        sprites[i].setPosition(startX + i * (iconWidth + spacing), y);
    }

    // Overlay to dim background (optional)
    sf::RectangleShape overlay(sf::Vector2f(windowWidth, windowHeight));
    overlay.setFillColor(sf::Color(0, 0, 0, 180)); // semi-transparent black

    // Show menu
    while (true)
    {
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                std::cerr << "Window closed during promotion menu.\n";
                return (color == WHITE) ? WHITE_QUEEN : BLACK_QUEEN;
            }

            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mouse(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
                for (int i = 0; i < 4; ++i)
                {
                    if (sprites[i].getGlobalBounds().contains(mouse))
                    {
                        return promotionTypes[i];
                    }
                }
            }
        }

        window->clear(); // Optional: only if you want to redraw from scratch
        // window->draw(board background) — if your board is visible, draw it here
        window->draw(overlay);
        window->draw(background);
        for (const auto &sprite : sprites)
            window->draw(sprite);
        window->display();
    }

    // Unreachable, fallback just in case
    return (color == WHITE) ? WHITE_QUEEN : BLACK_QUEEN;
}

std::vector<std::tuple<int, int>> Board::getBlackMoves()
{
    return blackPositions;
}

std::vector<std::tuple<int, int>> Board::getWhiteMoves()
{
    return whitePositions;
}

int Board::getMoveCount() const
{
    return movesDone;
}