// chess.cpp
// Contains implementation of all Piece subclasses and Board class

#include "chess.h"
#include <cmath>   // for abs()
#include <cstring> // for memset

// ============================================================
//  BASE CLASS: Piece - Constructor
// ============================================================
Piece::Piece(char color, char symbol) {
    this->color = color;
    this->symbol = symbol;
    this->hasMoved = false;
}

// ============================================================
//  PAWN IMPLEMENTATION
//  - Moves 1 forward (or 2 on first move)
//  - Captures diagonally
// ============================================================
Pawn::Pawn(char color) : Piece(color, 'P') {}

bool Pawn::isValidMove(int fromRow, int fromCol, int toRow, int toCol,
    Piece* board[8][8]) {
    // Direction: White moves UP (row decreases), Black moves DOWN (row increases)
    int direction = (color == 'W') ? -1 : 1;
    int rowDiff = toRow - fromRow;
    int colDiff = toCol - fromCol;

    // --- Move 1 square forward (no capture) ---
    if (colDiff == 0 && rowDiff == direction) {
        // Destination must be empty
        if (board[toRow][toCol] == nullptr) return true;
    }

    // --- Move 2 squares forward on first move (no capture) ---
    if (colDiff == 0 && rowDiff == 2 * direction && !hasMoved) {
        // Both squares in path must be empty
        int middleRow = fromRow + direction;
        if (board[middleRow][fromCol] == nullptr &&
            board[toRow][toCol] == nullptr) {
            return true;
        }
    }

    // --- Diagonal capture ---
    if (abs(colDiff) == 1 && rowDiff == direction) {
        // Must be an enemy piece at destination
        if (board[toRow][toCol] != nullptr &&
            board[toRow][toCol]->getColor() != color) {
            return true;
        }
    }

    return false; // All other moves are invalid
}

// ============================================================
//  ROOK IMPLEMENTATION
//  - Moves horizontally or vertically any number of squares
// ============================================================
Rook::Rook(char color) : Piece(color, 'R') {}

bool Rook::isValidMove(int fromRow, int fromCol, int toRow, int toCol,
    Piece* board[8][8]) {
    // Must move in a straight line (same row OR same column)
    if (fromRow != toRow && fromCol != toCol) return false;

    // Check there are no pieces in between (path must be clear)
    int rowStep = 0, colStep = 0;
    if (toRow > fromRow) rowStep = 1;
    if (toRow < fromRow) rowStep = -1;
    if (toCol > fromCol) colStep = 1;
    if (toCol < fromCol) colStep = -1;

    int r = fromRow + rowStep;
    int c = fromCol + colStep;
    // Walk along the path (exclude destination)
    while (r != toRow || c != toCol) {
        if (board[r][c] != nullptr) return false; // Something is in the way
        r += rowStep;
        c += colStep;
    }

    // Destination must be empty OR occupied by enemy
    if (board[toRow][toCol] == nullptr) return true;
    if (board[toRow][toCol]->getColor() != color) return true;

    return false;
}

// ============================================================
//  KNIGHT IMPLEMENTATION
//  - Moves in an L-shape: 2+1 or 1+2 squares
//  - CAN jump over other pieces
// ============================================================
Knight::Knight(char color) : Piece(color, 'N') {}

bool Knight::isValidMove(int fromRow, int fromCol, int toRow, int toCol,
    Piece* board[8][8]) {
    int rowDiff = abs(toRow - fromRow);
    int colDiff = abs(toCol - fromCol);

    // L-shape: one side is 2, other side is 1
    bool isLShape = (rowDiff == 2 && colDiff == 1) ||
        (rowDiff == 1 && colDiff == 2);
    if (!isLShape) return false;

    // Destination must be empty or enemy
    if (board[toRow][toCol] == nullptr) return true;
    if (board[toRow][toCol]->getColor() != color) return true;

    return false;
}

// ============================================================
//  BISHOP IMPLEMENTATION
//  - Moves diagonally any number of squares
// ============================================================
Bishop::Bishop(char color) : Piece(color, 'B') {}

bool Bishop::isValidMove(int fromRow, int fromCol, int toRow, int toCol,
    Piece* board[8][8]) {
    int rowDiff = abs(toRow - fromRow);
    int colDiff = abs(toCol - fromCol);

    // Must move diagonally: row change equals column change
    if (rowDiff != colDiff) return false;

    // Check path is clear
    int rowStep = (toRow > fromRow) ? 1 : -1;
    int colStep = (toCol > fromCol) ? 1 : -1;

    int r = fromRow + rowStep;
    int c = fromCol + colStep;
    while (r != toRow || c != toCol) {
        if (board[r][c] != nullptr) return false;
        r += rowStep;
        c += colStep;
    }

    // Destination: empty or enemy
    if (board[toRow][toCol] == nullptr) return true;
    if (board[toRow][toCol]->getColor() != color) return true;

    return false;
}

// ============================================================
//  QUEEN IMPLEMENTATION
//  - Combines Rook + Bishop movement
// ============================================================
Queen::Queen(char color) : Piece(color, 'Q') {}

bool Queen::isValidMove(int fromRow, int fromCol, int toRow, int toCol,
    Piece* board[8][8]) {
    // Queen = Rook movement OR Bishop movement
    // We reuse the logic by creating temporary objects
    Rook   tempRook(color);
    Bishop tempBishop(color);

    return tempRook.isValidMove(fromRow, fromCol, toRow, toCol, board) ||
        tempBishop.isValidMove(fromRow, fromCol, toRow, toCol, board);
}

// ============================================================
//  KING IMPLEMENTATION
//  - Moves exactly 1 square in any direction
// ============================================================
King::King(char color) : Piece(color, 'K') {}

bool King::isValidMove(int fromRow, int fromCol, int toRow, int toCol,
    Piece* board[8][8]) {
    int rowDiff = abs(toRow - fromRow);
    int colDiff = abs(toCol - fromCol);

    // King moves at most 1 square in any direction
    if (rowDiff > 1 || colDiff > 1) return false;

    // Destination must be empty or enemy
    if (board[toRow][toCol] == nullptr) return true;
    if (board[toRow][toCol]->getColor() != color) return true;

    return false;
}

// ============================================================
//  BOARD CLASS IMPLEMENTATION
// ============================================================

// Constructor: initialize grid to null and setup pieces
Board::Board() {
    // Set all squares to nullptr (empty)
    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 8; c++)
            grid[r][c] = nullptr;

    hasLastMove = false;
    lastFromRow = lastFromCol = lastToRow = lastToCol = -1;

    setup(); // Place all pieces in starting positions
}

// Destructor: free all dynamically allocated pieces
Board::~Board() {
    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 8; c++)
            if (grid[r][c] != nullptr) {
                delete grid[r][c];
                grid[r][c] = nullptr;
            }
}

// Place all 32 pieces in their standard starting positions
void Board::setup() {
    // --- BLACK pieces (top of board, rows 0-1) ---
    grid[0][0] = new Rook('B');
    grid[0][1] = new Knight('B');
    grid[0][2] = new Bishop('B');
    grid[0][3] = new Queen('B');
    grid[0][4] = new King('B');
    grid[0][5] = new Bishop('B');
    grid[0][6] = new Knight('B');
    grid[0][7] = new Rook('B');
    for (int c = 0; c < 8; c++)
        grid[1][c] = new Pawn('B');

    // --- WHITE pieces (bottom of board, rows 6-7) ---
    for (int c = 0; c < 8; c++)
        grid[6][c] = new Pawn('W');
    grid[7][0] = new Rook('W');
    grid[7][1] = new Knight('W');
    grid[7][2] = new Bishop('W');
    grid[7][3] = new Queen('W');
    grid[7][4] = new King('W');
    grid[7][5] = new Bishop('W');
    grid[7][6] = new Knight('W');
    grid[7][7] = new Rook('W');
}

// Display the board with ANSI colors (Bonus Graphical UI)
void Board::display() const {
    cout << "\n";
    // Column labels
    cout << "    ";
    for (int c = 0; c < 8; c++) {
        cout << COLOR_INFO << "  " << (char)('a' + c) << " " << RESET;
    }
    cout << "\n";
    cout << "    ";
    for (int c = 0; c < 8; c++) cout << "----";
    cout << "\n";

    for (int r = 0; r < 8; r++) {
        // Row number label (8 at top, 1 at bottom from white's view)
        cout << COLOR_INFO << " " << (8 - r) << " |" << RESET;

        for (int c = 0; c < 8; c++) {
            // Choose square background color
            bool isLastMove = hasLastMove &&
                ((r == lastFromRow && c == lastFromCol) ||
                    (r == lastToRow && c == lastToCol));

            string bg;
            if (isLastMove)
                bg = BG_HIGHLIGHT;         // Highlight last moved square
            else if ((r + c) % 2 == 0)
                bg = BG_LIGHT;             // Light square
            else
                bg = BG_DARK;              // Dark square

            cout << bg;

            if (grid[r][c] == nullptr) {
                // Empty square
                cout << "    " << RESET;
            }
            else {
                // Print piece with its color
                char sym = grid[r][c]->getSymbol();
                if (grid[r][c]->getColor() == 'W')
                    cout << WHITE_PIECE << " " << sym << "  " << RESET;
                else
                    cout << BLACK_PIECE << " " << sym << "  " << RESET;
            }
        }

        cout << COLOR_INFO << "| " << (8 - r) << RESET << "\n";
    }

    cout << "    ";
    for (int c = 0; c < 8; c++) cout << "----";
    cout << "\n";
    cout << "    ";
    for (int c = 0; c < 8; c++) {
        cout << COLOR_INFO << "  " << (char)('a' + c) << " " << RESET;
    }
    cout << "\n\n";
}

// Get piece at given position
Piece* Board::getPiece(int row, int col) const {
    return grid[row][col];
}

// Attempt to move piece from (fromRow,fromCol) to (toRow,toCol)
// Returns true if the move was made successfully
bool Board::movePiece(int fromRow, int fromCol, int toRow, int toCol) {
    Piece* piece = grid[fromRow][fromCol];

    // 1. Is there a piece at the source?
    if (piece == nullptr) return false;

    // 2. Is the move valid according to piece rules?
    if (!piece->isValidMove(fromRow, fromCol, toRow, toCol, grid))
        return false;

    // 3. Simulate the move and check if own King ends up in check
    //    (You can't make a move that puts your own King in check)
    Piece* captured = grid[toRow][toCol]; // Save captured piece (could be null)
    grid[toRow][toCol] = piece;
    grid[fromRow][fromCol] = nullptr;

    if (isInCheck(piece->getColor())) {
        // Undo the move - it leaves our King in check
        grid[fromRow][fromCol] = piece;
        grid[toRow][toCol] = captured;
        return false;
    }

    // 4. Move is valid - update last move for highlighting
    lastFromRow = fromRow; lastFromCol = fromCol;
    lastToRow = toRow;   lastToCol = toCol;
    hasLastMove = true;

    // 5. Free captured piece from memory
    if (captured != nullptr) delete captured;

    // 6. Mark piece as having moved
    piece->setHasMoved(true);

    // 7. Pawn promotion: if pawn reaches the other end, promote to Queen
    if (piece->getSymbol() == 'P') {
        if ((piece->getColor() == 'W' && toRow == 0) ||
            (piece->getColor() == 'B' && toRow == 7)) {
            char col = piece->getColor();
            delete grid[toRow][toCol];
            grid[toRow][toCol] = new Queen(col);
            cout << COLOR_MSG << "  *** Pawn promoted to Queen! ***\n" << RESET;
        }
    }

    return true;
}

// Find position of the King of the given color
void Board::findKing(char color, int& row, int& col) {
    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 8; c++)
            if (grid[r][c] != nullptr &&
                grid[r][c]->getColor() == color &&
                grid[r][c]->getSymbol() == 'K') {
                row = r; col = c;
                return;
            }
    row = -1; col = -1; // King not found (shouldn't happen in a real game)
}

// Check if the given color's King is currently in check
bool Board::isInCheck(char color) {
    // Find King's position
    int kingRow, kingCol;
    findKing(color, kingRow, kingCol);
    if (kingRow == -1) return false;

    // Check if any enemy piece can attack the King's position
    char enemyColor = (color == 'W') ? 'B' : 'W';
    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 8; c++)
            if (grid[r][c] != nullptr &&
                grid[r][c]->getColor() == enemyColor) {
                if (grid[r][c]->isValidMove(r, c, kingRow, kingCol, grid))
                    return true;
            }
    return false;
}

// Check if the given color has at least one valid legal move
bool Board::hasAnyValidMove(char color) {
    for (int fr = 0; fr < 8; fr++)
        for (int fc = 0; fc < 8; fc++) {
            if (grid[fr][fc] == nullptr) continue;
            if (grid[fr][fc]->getColor() != color) continue;

            // Try every possible destination
            for (int tr = 0; tr < 8; tr++)
                for (int tc = 0; tc < 8; tc++) {
                    if (!grid[fr][fc]->isValidMove(fr, fc, tr, tc, grid)) continue;

                    // Simulate the move
                    Piece* saved = grid[tr][tc];
                    grid[tr][tc] = grid[fr][fc];
                    grid[fr][fc] = nullptr;

                    bool stillInCheck = isInCheck(color);

                    // Undo simulation
                    grid[fr][fc] = grid[tr][tc];
                    grid[tr][tc] = saved;

                    if (!stillInCheck) return true; // Found at least one valid move
                }
        }
    return false; // No valid move found
}

// Checkmate: King is in check AND no valid moves exist
bool Board::isCheckmate(char color) {
    return isInCheck(color) && !hasAnyValidMove(color);
}

// Stalemate: King is NOT in check BUT no valid moves exist
bool Board::isStalemate(char color) {
    return !isInCheck(color) && !hasAnyValidMove(color);
}
