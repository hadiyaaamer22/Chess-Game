// Piece.cpp
#include "Piece.h"
#include <cmath>
#include <cstring>


// base piece
Piece::Piece(int x, int y, PlayerColor color, PieceType type)
    : x(x), y(y), color(color), type(type), hasMoved(false) {
}

int  Piece::getX() const { return x; }
int  Piece::getY() const { return y; }
PlayerColor Piece::getColor() const { return color; }
bool Piece::getHasMoved() const { return hasMoved; }
void Piece::setMoved() { hasMoved = true; }
void Piece::setPosition(int nx, int ny) { x = nx; y = ny; }

bool Piece::isEnemy(const Piece* other) const {
    if (!other) return false;
    return other->color != color;
}

// Used by Rook, Bishop, Queen to check if the path is clear
bool Piece::isPathClear(int nx, int ny, Piece* board[8][8]) const {
    int dx = (nx - x == 0) ? 0 : (nx - x > 0 ? 1 : -1);
    int dy = (ny - y == 0) ? 0 : (ny - y > 0 ? 1 : -1);

    int cx = x + dx, cy = y + dy;
    while (cx != nx || cy != ny) {
        if (board[cx][cy] != nullptr) return false;
        cx += dx;
        cy += dy;
    }
    return true;
}

// king
King::King(int x, int y, PlayerColor color) : Piece(x, y, color, KING) {}

bool King::isMoveValid(int nx, int ny, Piece* board[8][8]) const {
    int dx = abs(nx - x), dy = abs(ny - y);
    if (dx > 1 || dy > 1 || (dx == 0 && dy == 0)) return false;
    if (board[nx][ny] && !isEnemy(board[nx][ny]))   return false;
    return true;
}
string King::getWhiteTexturePath() const { return "pieces/white/wk.png"; }
string King::getBlackTexturePath() const { return "pieces/black/bk.png"; }
PieceType King::getType() const { return KING; }

// queen
Queen::Queen(int x, int y, PlayerColor color) : Piece(x, y, color, QUEEN) {}

bool Queen::isMoveValid(int nx, int ny, Piece* board[8][8]) const {
    if (x == nx && y == ny) return false;

    if (board[nx][ny] && !isEnemy(board[nx][ny])) return false;

    int dx = abs(nx - x), dy = abs(ny - y);

    bool straight = (x == nx || y == ny);
    bool diagonal = (dx == dy);
    if (!straight && !diagonal) return false;

    return isPathClear(nx, ny, board);
}
string Queen::getWhiteTexturePath() const { return "pieces/white/wq.png"; }
string Queen::getBlackTexturePath() const { return "pieces/black/bq.png"; }
PieceType Queen::getType() const { return QUEEN; }

// rook
Rook::Rook(int x, int y, PlayerColor color) : Piece(x, y, color, ROOK) {}

bool Rook::isMoveValid(int nx, int ny, Piece* board[8][8]) const {
    if (x == nx && y == ny) return false;
    if (board[nx][ny] && !isEnemy(board[nx][ny])) return false;
    if (x != nx && y != ny) return false; // must be straight
    return isPathClear(nx, ny, board);
}
string Rook::getWhiteTexturePath() const { return "pieces/white/wr.png"; }
string Rook::getBlackTexturePath() const { return "pieces/black/br.png"; }
PieceType Rook::getType() const { return ROOK; }

// bishop
Bishop::Bishop(int x, int y, PlayerColor color) : Piece(x, y, color, BISHOP) {}

bool Bishop::isMoveValid(int nx, int ny, Piece* board[8][8]) const {
    if (x == nx && y == ny) return false;
    if (board[nx][ny] && !isEnemy(board[nx][ny])) return false;
    if (abs(nx - x) != abs(ny - y)) return false; // must be diagonal
    return isPathClear(nx, ny, board);
}
string Bishop::getWhiteTexturePath() const { return "pieces/white/wb.png"; }
string Bishop::getBlackTexturePath() const { return "pieces/black/bb.png"; }
PieceType Bishop::getType() const { return BISHOP; }

// knight
Knight::Knight(int x, int y, PlayerColor color) : Piece(x, y, color, KNIGHT) {}

bool Knight::isMoveValid(int nx, int ny, Piece* board[8][8]) const {
    if (board[nx][ny] && !isEnemy(board[nx][ny])) return false;
    int dx = abs(nx - x), dy = abs(ny - y);
    return (dx == 2 && dy == 1) || (dx == 1 && dy == 2);
}
string Knight::getWhiteTexturePath() const { return "pieces/white/wn.png"; }
string Knight::getBlackTexturePath() const { return "pieces/black/bn.png"; }
PieceType Knight::getType() const { return KNIGHT; }

// pawn
Pawn::Pawn(int x, int y, PlayerColor color) : Piece(x, y, color, PAWN_TYPE) {}

bool Pawn::isMoveValid(int nx, int ny, Piece* board[8][8]) const {
    int dir = (color == WHITE) ? -1 : 1; // White moves up (decreasing y), Black moves down
    int dx = nx - x;
    int dy = ny - y;

    // One step forward
    if (dx == 0 && dy == dir && !board[nx][ny])
        return true;

    // Two steps from starting row
    int startRow = (color == WHITE) ? 6 : 1;
    if (dx == 0 && dy == 2 * dir && y == startRow && !board[nx][ny] && !board[x][y + dir])
        return true;

    // Diagonal capture
    if (abs(dx) == 1 && dy == dir && board[nx][ny] && isEnemy(board[nx][ny]))
        return true;

    return false;
}
string Pawn::getWhiteTexturePath() const { return "pieces//white//wp.png"; }
string Pawn::getBlackTexturePath() const { return "pieces//black//bp.png"; }
PieceType Pawn::getType() const { return PAWN_TYPE; }

// board
chessBoard::chessBoard()
    : currentTurn(WHITE), selectedPiece(nullptr),
    selectedX(-1), selectedY(-1), gameOver(false), winner(WHITE)
{
    clearBoard();
    for (int t = 0; t < 6; t++)
        for (int c = 0; c < 2; c++)
            texturesLoaded[t][c] = false;
}

chessBoard::~chessBoard() {
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            if (grid[i][j]) { delete grid[i][j]; grid[i][j] = nullptr; }
}

void chessBoard::clearBoard() {
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            grid[i][j] = nullptr;
}

void chessBoard::placePiece(Piece* p) {
    grid[p->getX()][p->getY()] = p;
}

bool chessBoard::isInBounds(int x, int y) const {
    return x >= 0 && x < 8 && y >= 0 && y < 8;
}

// init standard chess layout
void chessBoard::initPieces() {
    // Clear any existing pieces
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            if (grid[i][j]) { delete grid[i][j]; grid[i][j] = nullptr; }

    // White back rank (row 7)
    placePiece(new Rook(0, 7, WHITE));
    placePiece(new Knight(1, 7, WHITE));
    placePiece(new Bishop(2, 7, WHITE));
    placePiece(new Queen(3, 7, WHITE));
    placePiece(new King(4, 7, WHITE));
    placePiece(new Bishop(5, 7, WHITE));
    placePiece(new Knight(6, 7, WHITE));
    placePiece(new Rook(7, 7, WHITE));

    // White pawns (row 6)
    for (int i = 0; i < 8; i++)
        placePiece(new Pawn(i, 6, WHITE));

    // Black back rank (row 0)
    placePiece(new Rook(0, 0, BLACK));
    placePiece(new Knight(1, 0, BLACK));
    placePiece(new Bishop(2, 0, BLACK));
    placePiece(new Queen(3, 0, BLACK));
    placePiece(new King(4, 0, BLACK));
    placePiece(new Bishop(5, 0, BLACK));
    placePiece(new Knight(6, 0, BLACK));
    placePiece(new Rook(7, 0, BLACK));

    // Black pawns (row 1)
    for (int i = 0; i < 8; i++)
        placePiece(new Pawn(i, 1, BLACK));
}

// texture loading
void chessBoard::loadTexture(Piece* p) {
    int t = (int)p->getType();
    int c = (int)p->getColor();

    if (texturesLoaded[t][c]) return;

    string path = (c == WHITE) ? p->getWhiteTexturePath() : p->getBlackTexturePath();


    if (!textures[t][c].loadFromFile(path)) {
        throw "Failed to load";
    }
    texturesLoaded[t][c] = true;

}

void chessBoard::preloadTextures() {
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            if (grid[i][j])
                loadTexture(grid[i][j]);
}

Texture* chessBoard::getTexture(Piece* p) {
    loadTexture(p);
    return &textures[(int)p->getType()][(int)p->getColor()];
}

// check detection
bool chessBoard::isKingInCheck(PlayerColor color) const {
    int kx = -1, ky = -1;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            if (grid[i][j] != nullptr && grid[i][j]->getType() == KING && grid[i][j]->getColor() == color)
            {
                kx = i; ky = j;
            }

    if (kx == -1) return false;

    // Non-const copy for isMoveValid
    Piece* tempGrid[8][8];
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            tempGrid[i][j] = grid[i][j];

    PlayerColor enemy = (color == WHITE) ? BLACK : WHITE;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            if (grid[i][j] && grid[i][j]->getColor() == enemy) {
                Piece* enemyPlr = grid[i][j];
                if (enemyPlr->isMoveValid(kx, ky, tempGrid))
                    return true;
            }
    return false;
}


// simulate move to check if it leaves king in check
bool chessBoard::wouldLeaveKingInCheck(int fx, int fy, int tx, int ty, PlayerColor color) const {
    // Temporarily make the move
    Piece* temp = grid[tx][ty];
    Piece* mover = grid[fx][fy];

    const_cast<chessBoard*>(this)->grid[tx][ty] = mover;
    const_cast<chessBoard*>(this)->grid[fx][fy] = nullptr;
    if (mover) const_cast<Piece*>(mover)->setPosition(tx, ty);

    bool inCheck = isKingInCheck(color);

    // Undo the move
    const_cast<chessBoard*>(this)->grid[fx][fy] = mover;
    const_cast<chessBoard*>(this)->grid[tx][ty] = temp;
    if (mover) const_cast<Piece*>(mover)->setPosition(fx, fy);

    return inCheck;
}

bool chessBoard::hasAnyValidMove(PlayerColor color) const {
    // Local non-const copy for passing to isMoveValid
    Piece* tempGrid[8][8];
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            tempGrid[i][j] = grid[i][j];

    for (int fx = 0; fx < 8; fx++)
        for (int fy = 0; fy < 8; fy++)
            if (grid[fx][fy] && grid[fx][fy]->getColor() == color)
                for (int tx = 0; tx < 8; tx++)
                    for (int ty = 0; ty < 8; ty++)
                        if (isInBounds(tx, ty) &&
                            grid[fx][fy]->isMoveValid(tx, ty, tempGrid) &&
                            !wouldLeaveKingInCheck(fx, fy, tx, ty, color))
                            return true;
    return false;
}

// move execution
bool chessBoard::movePiece(int fx, int fy, int tx, int ty) {
    if (!isInBounds(fx, fy) || !isInBounds(tx, ty)) return false;
    Piece* mover = grid[fx][fy];
    if (!mover) return false;
    if (!mover->isMoveValid(tx, ty, grid)) return false;
    if (wouldLeaveKingInCheck(fx, fy, tx, ty, mover->getColor())) return false;

    // Capture enemy piece
    if (grid[tx][ty]) { delete grid[tx][ty]; grid[tx][ty] = nullptr; }

    // Execute move
    grid[tx][ty] = mover;
    grid[fx][fy] = nullptr;
    mover->setPosition(tx, ty);
    mover->setMoved();

    // Pawn promotion to Queen
    if (mover->getType() == PAWN_TYPE) {
        if ((mover->getColor() == WHITE && ty == 0) ||
            (mover->getColor() == BLACK && ty == 7)) {
            PlayerColor c = mover->getColor();
            delete grid[tx][ty];
            grid[tx][ty] = new Queen(tx, ty, c);
        }
    }

    // Switch turn
    currentTurn = (currentTurn == WHITE) ? BLACK : WHITE;

    // Check for checkmate or stalemate
    if (!hasAnyValidMove(currentTurn)) {
        gameOver = true;
        winner = (currentTurn == WHITE) ? BLACK : WHITE;
    }

    return true;
}

// click handler
void chessBoard::handleClick(int pixelX, int pixelY) {
    if (gameOver) return;
    int col = pixelX / TILE_SIZE;
    int row = pixelY / TILE_SIZE;
    if (!isInBounds(col, row)) return;

    if (selectedPiece == nullptr) {

        // Select a piece
        if (grid[col][row] && grid[col][row]->getColor() == currentTurn) {
            selectedPiece = grid[col][row];
            selectedX = col;
            selectedY = row;
        }
    }
    else {
        // Attempt move
        if (col == selectedX && row == selectedY) {
            // Deselect
            selectedPiece = nullptr;
            selectedX = selectedY = -1;
        }
        else if (grid[col][row] && grid[col][row]->getColor() == currentTurn) {
            // Switch selection to another friendly piece
            selectedPiece = grid[col][row];
            selectedX = col;
            selectedY = row;
        }
        else {
            movePiece(selectedX, selectedY, col, row);
            selectedPiece = nullptr;
            selectedX = selectedY = -1;
        }
    }
}

// drawing
void chessBoard::drawBoard(RenderWindow& window) {
    // Draw tiles
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            RectangleShape tile(Vector2f((float)TILE_SIZE, (float)TILE_SIZE));
            tile.setPosition(Vector2f((float)(i * TILE_SIZE), (float)(j * TILE_SIZE)));
            if ((i + j) % 2 == 0)
                tile.setFillColor(Color(235, 236, 208)); // light
            else
                tile.setFillColor(Color(115, 149, 82));  // dark
            window.draw(tile);
        }
    }
    drawHighlights(window);
    drawPieces(window);
}

void chessBoard::drawHighlights(RenderWindow& window) {
    if (!selectedPiece) return;

    // Highlight selected tile
    RectangleShape sel(Vector2f((float)TILE_SIZE, (float)TILE_SIZE));
    sel.setPosition(Vector2f((float)(selectedX * TILE_SIZE), (float)(selectedY * TILE_SIZE)));
    sel.setFillColor(Color(186, 202, 68, 180));
    window.draw(sel);

    // Highlight valid moves
    for (int tx = 0; tx < 8; tx++) {
        for (int ty = 0; ty < 8; ty++) {
            if (selectedPiece->isMoveValid(tx, ty, grid) &&
                !wouldLeaveKingInCheck(selectedX, selectedY, tx, ty, currentTurn)) {
                RectangleShape hint(Vector2f((float)TILE_SIZE, (float)TILE_SIZE));
                hint.setPosition(Vector2f((float)(tx * TILE_SIZE), (float)(ty * TILE_SIZE)));
                if (grid[tx][ty]) // enemy capture
                    hint.setFillColor(Color(220, 60, 60, 150));
                else
                    hint.setFillColor(Color(100, 200, 100, 120));
                window.draw(hint);
            }
        }
    }

    // Highlight king in check
    PlayerColor enemy = (currentTurn == WHITE) ? BLACK : WHITE;
    if (isKingInCheck(currentTurn)) {
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                if (grid[i][j] && grid[i][j]->getType() == KING && grid[i][j]->getColor() == currentTurn) {
                    RectangleShape checkHL(Vector2f((float)TILE_SIZE, (float)TILE_SIZE));
                    checkHL.setPosition(Vector2f((float)(i * TILE_SIZE), (float)(j * TILE_SIZE)));
                    checkHL.setFillColor(Color(255, 0, 0, 140));
                    window.draw(checkHL);
                }
    }
}

void chessBoard::drawPieces(RenderWindow& window) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (!grid[i][j]) continue;
            Piece* p = grid[i][j];
            Texture* tex = getTexture(p); // [ty][c]
            Sprite sprite(*tex);

            // Scale to fit tile
            Vector2u tSize = tex->getSize();
            float scaleX = (float)TILE_SIZE / tSize.x;
            float scaleY = (float)TILE_SIZE / tSize.y;
            sprite.setScale({ scaleX, scaleY });
            sprite.setPosition({ (float)(i * TILE_SIZE), (float)(j * TILE_SIZE) });
            window.draw(sprite);
        }
    }
}

bool chessBoard::isGameOver() const { return gameOver; }
PlayerColor chessBoard::getCurrentTurn() const { return currentTurn; }
PlayerColor chessBoard::getWinner() const { return winner; }
