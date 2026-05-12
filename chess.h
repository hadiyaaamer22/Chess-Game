#pragma once
#include <iostream>
#include <string>
using namespace std;

// ============================================================
//  ANSI color codes for Bonus Graphical UI (colored console)
// ============================================================
#define RESET       "\033[0m"
#define BOLD        "\033[1m"
#define BG_DARK     "\033[48;5;94m"   // dark brown square
#define BG_LIGHT    "\033[48;5;229m"  // light beige square
#define BG_HIGHLIGHT "\033[48;5;226m" // yellow highlight for last move
#define WHITE_PIECE "\033[1;97m"      // bright white for white pieces
#define BLACK_PIECE "\033[1;30m"      // dark black for black pieces
#define COLOR_TITLE "\033[1;33m"      // yellow for title
#define COLOR_MSG   "\033[1;36m"      // cyan for messages
#define COLOR_ERROR "\033[1;31m"      // red for errors
#define COLOR_WIN   "\033[1;32m"      // green for win message
#define COLOR_INFO  "\033[1;35m"      // purple for info

// ============================================================
//  BASE CLASS: Piece
//  - All chess pieces inherit from this class
//  - Demonstrates: Encapsulation, Inheritance, Polymorphism
// ============================================================
class Piece {
protected:
    char color;     // 'W' for White, 'B' for Black
    char symbol;    // The symbol shown on the board (e.g. 'K', 'Q')
    bool hasMoved;  // Tracks if piece has moved (used for pawn's first move)

public:
    // Constructor
    Piece(char color, char symbol);

    // Virtual destructor (good OOP practice with inheritance)
    virtual ~Piece() {}

    // Pure virtual function - each piece must define its own move rules
    // This is POLYMORPHISM - same function, different behavior per class
    virtual bool isValidMove(int fromRow, int fromCol, int toRow, int toCol,
        Piece* board[8][8]) = 0;

    // Getters (Encapsulation - accessing private/protected data via functions)
    char getColor()  const { return color; }
    char getSymbol() const { return symbol; }
    bool getHasMoved() const { return hasMoved; }

    // Setter
    void setHasMoved(bool val) { hasMoved = val; }
};

// ============================================================
//  DERIVED CLASS: Pawn
// ============================================================
class Pawn : public Piece {
public:
    Pawn(char color);
    // Override the pure virtual function
    bool isValidMove(int fromRow, int fromCol, int toRow, int toCol,
        Piece* board[8][8]) override;
};

// ============================================================
//  DERIVED CLASS: Rook
// ============================================================
class Rook : public Piece {
public:
    Rook(char color);
    bool isValidMove(int fromRow, int fromCol, int toRow, int toCol,
        Piece* board[8][8]) override;
};

// ============================================================
//  DERIVED CLASS: Knight
// ============================================================
class Knight : public Piece {
public:
    Knight(char color);
    bool isValidMove(int fromRow, int fromCol, int toRow, int toCol,
        Piece* board[8][8]) override;
};

// ============================================================
//  DERIVED CLASS: Bishop
// ============================================================
class Bishop : public Piece {
public:
    Bishop(char color);
    bool isValidMove(int fromRow, int fromCol, int toRow, int toCol,
        Piece* board[8][8]) override;
};

// ============================================================
//  DERIVED CLASS: Queen
// ============================================================
class Queen : public Piece {
public:
    Queen(char color);
    bool isValidMove(int fromRow, int fromCol, int toRow, int toCol,
        Piece* board[8][8]) override;
};

// ============================================================
//  DERIVED CLASS: King
// ============================================================
class King : public Piece {
public:
    King(char color);
    bool isValidMove(int fromRow, int fromCol, int toRow, int toCol,
        Piece* board[8][8]) override;
};

// ============================================================
//  BOARD CLASS (Composition)
//  - The Board "has-a" collection of Piece objects
//  - Demonstrates: Composition
// ============================================================
class Board {
private:
    Piece* grid[8][8];  // 8x8 grid holding pointers to Piece objects

    // Stores last move for highlight (Bonus UI feature)
    int lastFromRow, lastFromCol, lastToRow, lastToCol;
    bool hasLastMove;

public:
    Board();    // Constructor - sets up the board
    ~Board();   // Destructor - cleans up memory

    // Initialize all pieces to their starting positions
    void setup();

    // Display the board with colors (Bonus Graphical UI)
    void display() const;

    // Get piece at a specific position
    Piece* getPiece(int row, int col) const;

    // Move a piece from one position to another
    // Returns true if move was successful
    bool movePiece(int fromRow, int fromCol, int toRow, int toCol);

    // Check if the given color's King is in check
    bool isInCheck(char color);

    // Find position of King of given color
    void findKing(char color, int& row, int& col);

    // Check if given color has any valid moves left (for checkmate/stalemate)
    bool hasAnyValidMove(char color);

    // Check if it's checkmate for the given color
    bool isCheckmate(char color);

    // Check if it's stalemate for the given color
    bool isStalemate(char color);
};

// ============================================================
//  GAME CLASS
//  - Controls the flow of the game
// ============================================================
class Game {
private:
    Board board;         // Composition: Game has-a Board
    char currentTurn;    // 'W' or 'B'
    int  moveCount;      // Total moves made
    bool gameOver;       // Is the game finished?

    // Print the title screen
    void printTitle() const;

    // Print current player's turn info
    void printTurnInfo() const;

    // Parse input like "e2 e4" into row/col numbers
    // Returns false if input format is wrong
    bool parseInput(const string& input,
        int& fromRow, int& fromCol,
        int& toRow, int& toCol) const;

    // Convert column letter (a-h) to index (0-7)
    int  colToIndex(char c) const;

    // Convert row number (1-8) to array index (0-7)
    int  rowToIndex(char c) const;

public:
    Game();

    // Start and run the game loop
    void run();
};

// CHESS_H#pragma once
