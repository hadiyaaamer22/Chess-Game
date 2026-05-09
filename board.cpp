#include "Board.h"
#include <iostream>
#include <cstdlib>
using namespace std;

// Constructor
Board::Board() {
    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 8; c++)
            grid[r][c] = nullptr;
    setupPieces();
}

// Destructor
Board::~Board() {
    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 8; c++)
            delete grid[r][c];
}

// Place all pieces in starting positions
void Board::setupPieces() {
    // Black back row (row 0)
    grid[0][0] = new Rook('B');   grid[0][7] = new Rook('B');
    grid[0][1] = new Knight('B'); grid[0][6] = new Knight('B');
    grid[0][2] = new Bishop('B'); grid[0][5] = new Bishop('B');
    grid[0][3] = new Queen('B');  grid[0][4] = new King('B');
    for (int c = 0; c < 8; c++) grid[1][c] = new Pawn('B');

    // White back row (row 7)
    grid[7][0] = new Rook('W');   grid[7][7] = new Rook('W');
    grid[7][1] = new Knight('W'); grid[7][6] = new Knight('W');
    grid[7][2] = new Bishop('W'); grid[7][5] = new Bishop('W');
    grid[7][3] = new Queen('W');  grid[7][4] = new King('W');
    for (int c = 0; c < 8; c++) grid[6][c] = new Pawn('W');
}

// Print the board to console
void Board::display() const {
    cout << "\n    a  b  c  d  e  f  g  h\n";
    cout << "  +------------------------+\n";
    for (int r = 0; r < 8; r++) {
        cout << (8 - r) << " | ";
        for (int c = 0; c < 8; c++) {
            if (grid[r][c] == nullptr)
                cout << ".  ";
            else
                cout << grid[r][c]->getSymbol() << "  ";
        }
        cout << "| " << (8 - r) << "\n";
    }
    cout << "  +------------------------+\n";
    cout << "    a  b  c  d  e  f  g  h\n\n";
}

// Validate and execute a move
bool Board::movePiece(int fr, int fc, int tr, int tc, char currentTurn) {
    // Bounds check
    if (fr < 0 || fr > 7 || fc < 0 || fc > 7 ||
        tr < 0 || tr > 7 || tc < 0 || tc > 7) {
        cout << "  Out of bounds!\n";
        return false;
    }

    Piece* src = grid[fr][fc];

    if (src == nullptr) {
        cout << "  No piece at that square!\n";
        return false;
    }

    if (src->getColor() != currentTurn) {
        cout << "  That is not your piece!\n";
        return false;
    }

    if (!src->isValidMove(fr, fc, tr, tc, grid)) {
        cout << "  Invalid move for this piece!\n";
        return false;
    }

    // Check if enemy King is captured -> game over
    if (grid[tr][tc] != nullptr) {
        char enemyKing = (currentTurn == 'W') ? 'k' : 'K';
        if (grid[tr][tc]->getSymbol() == enemyKing) {
            cout << "\n  *** "
                << (currentTurn == 'W' ? "White" : "Black")
                << " captured the King! GAME OVER! ***\n";
            delete grid[tr][tc];
            grid[tr][tc] = src;
            grid[fr][fc] = nullptr;
            display();
            exit(0);
        }
    }

    // Execute move
    delete grid[tr][tc];
    grid[tr][tc] = src;
    grid[fr][fc] = nullptr;
    return true;
}