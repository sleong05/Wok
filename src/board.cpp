#include <SFML/Graphics.hpp>
#include "identifier.hpp"
#include "board.hpp"
#include "constants.hpp"
#include "moveGetter.hpp"
#include "squareAttacker.hpp"

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
}

inline void fastRemove(std::vector<std::tuple<int, int>> &vec, const std::tuple<int, int> &target)
{
    for (size_t i = 0; i < vec.size(); ++i)
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

void Board::doMove(LegalMove &move, sf::RenderWindow *window, bool fromUser)
{
    auto [oldCol, oldRow] = move.from;
    auto [newCol, newRow] = move.to;

    // store hasMove
    move.fromHasMoved = hasMovedArray[oldRow][oldCol];
    move.toHasMoved = hasMovedArray[newRow][newCol];

    // update tracked positions
    updateKnownPositions(move);

    // move piece
    squares[oldRow][oldCol] = constants::EMPTY;
    squares[newRow][newCol] = move.pieceToMove;

    // Do additonal moves for special moves
    if (move.isEnPassant)
    {
        int color = Identifier::getTeam(move.pieceToMove);

        squares[newRow - color][newCol] = constants::EMPTY;
        removePositionFromColorTracker(color * -1, newCol, newRow - color);
    }

    if (move.isCastle)
    {
        castle(move);
    }
    // update tracked info
    lastMove = move;
    hasMovedArray[oldRow][oldCol] = true;
    hasMovedArray[newRow][newCol] = true;

    // promotion logic
    int promotedRow = (Identifier::getTeam(move.pieceToMove) == WHITE) ? 0 : 7;
    if ((move.pieceToMove == WHITE_PAWN or move.pieceToMove == BLACK_PAWN) && newRow == promotedRow)
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

    /*
    for (const auto &row : hasMovedArray)
    {
        for (int val : row)
        {
            std::cout << val << " ";
        }
        std::cout << "\n";
    } */
}

void Board::undoMove(LegalMove &move)
{
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
        squares[newRow - color][newCol] = (color == WHITE) ? BLACK_PAWN : WHITE_PAWN;

        addPositionToColorTracker(color * -1, newCol, newRow - color);
    }

    if (move.isCastle)
    {
        int color = Identifier::getTeam(move.pieceToMove);
        auto [kingCol, kingRow] = move.to;

        if (kingCol == 2) // long castle
        {
            squares[kingRow][0] = squares[kingRow][3];
            squares[kingRow][3] = EMPTY;
            hasMovedArray[kingRow][0] = false;
            removePositionFromColorTracker(color, 3, kingRow);
            addPositionToColorTracker(color, 0, kingRow);
        }
        else if ((kingCol == 6)) // short castle
        {
            squares[kingRow][7] = squares[kingRow][5];
            squares[kingRow][5] = EMPTY;
            hasMovedArray[kingRow][7] = false;
            removePositionFromColorTracker(color, 5, kingRow);
            addPositionToColorTracker(color, 7, kingRow);
        }
    }
    if (move.pieceToMove == WHITE_KING)
    {
        whiteKingPosition = move.from;
    }
    else if (move.pieceToMove == BLACK_KING)
    {
        blackKingPosition = move.from;
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
void Board::castle(LegalMove &move)
{
    int color = Identifier::getTeam(move.pieceToMove);
    auto [kingCol, kingRow] = move.to;

    if (kingCol == 2) // long castle
    {
        squares[kingRow][3] = squares[kingRow][0];
        squares[kingRow][0] = constants::EMPTY;
        hasMovedArray[kingRow][0] = true;
        removePositionFromColorTracker(color, 0, kingRow);
        addPositionToColorTracker(color, 3, kingRow);
    }
    else if ((kingCol == 6)) // short castle
    {
        squares[kingRow][5] = squares[kingRow][7];
        squares[kingRow][7] = constants::EMPTY;
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