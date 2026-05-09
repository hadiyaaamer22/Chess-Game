#pragma once
#include <cstdlib>

// Base class: Piece
class Piece {
protected:
    char color;   // 'W' = White, 'B' = Black
    char symbol;  // display character

public:
    Piece(char c, char s) : color(c), symbol(s) {}
    virtual ~Piece() {}

    char getColor()  const { return color; }
    char getSymbol() const { return symbol; }

    // Pure virtual - every piece defines its own move rules
    virtual bool isValidMove(int fromRow, int fromCol,
        int toRow, int toCol,
        Piece* board[8][8]) const = 0;
};

// PAWN
class Pawn : public Piece {
public:
    Pawn(char c) : Piece(c, c == 'W' ? 'P' : 'p') {}

    bool isValidMove(int fr, int fc, int tr, int tc,
        Piece* board[8][8]) const override {
        int dir = (color == 'W') ? -1 : 1;
        int startRow = (color == 'W') ? 6 : 1;

        // One step forward
        if (tc == fc && tr == fr + dir && board[tr][tc] == nullptr)
            return true;

        // Two steps from starting row
        if (tc == fc && fr == startRow &&
            tr == fr + 2 * dir &&
            board[fr + dir][fc] == nullptr &&
            board[tr][tc] == nullptr)
            return true;

        // Diagonal capture
        if (abs(tc - fc) == 1 && tr == fr + dir &&
            board[tr][tc] != nullptr &&
            board[tr][tc]->getColor() != color)
            return true;

        return false;
    }
};

// ROOK
class Rook : public Piece {
public:
    Rook(char c) : Piece(c, c == 'W' ? 'R' : 'r') {}

    bool isValidMove(int fr, int fc, int tr, int tc,
        Piece* board[8][8]) const override {
        if (fr != tr && fc != tc) return false;

        int rowStep = (tr > fr) ? 1 : (tr < fr) ? -1 : 0;
        int colStep = (tc > fc) ? 1 : (tc < fc) ? -1 : 0;
        int r = fr + rowStep, c = fc + colStep;
        while (r != tr || c != tc) {
            if (board[r][c] != nullptr) return false;
            r += rowStep; c += colStep;
        }
        return (board[tr][tc] == nullptr || board[tr][tc]->getColor() != color);
    }
};

// KNIGHT
class Knight : public Piece {
public:
    Knight(char c) : Piece(c, c == 'W' ? 'N' : 'n') {}

    bool isValidMove(int fr, int fc, int tr, int tc,
        Piece* board[8][8]) const override {
        int dr = abs(tr - fr), dc = abs(tc - fc);
        if (!((dr == 2 && dc == 1) || (dr == 1 && dc == 2))) return false;
        return (board[tr][tc] == nullptr || board[tr][tc]->getColor() != color);
    }
};

// BISHOP
class Bishop : public Piece {
public:
    Bishop(char c) : Piece(c, c == 'W' ? 'B' : 'b') {}

    bool isValidMove(int fr, int fc, int tr, int tc,
        Piece* board[8][8]) const override {
        if (abs(tr - fr) != abs(tc - fc)) return false;

        int rowStep = (tr > fr) ? 1 : -1;
        int colStep = (tc > fc) ? 1 : -1;
        int r = fr + rowStep, c = fc + colStep;
        while (r != tr || c != tc) {
            if (board[r][c] != nullptr) return false;
            r += rowStep; c += colStep;
        }
        return (board[tr][tc] == nullptr || board[tr][tc]->getColor() != color);
    }
};

// QUEEN
class Queen : public Piece {
public:
    Queen(char c) : Piece(c, c == 'W' ? 'Q' : 'q') {}

    bool isValidMove(int fr, int fc, int tr, int tc,
        Piece* board[8][8]) const override {
        bool straight = (fr == tr || fc == tc);
        bool diagonal = (abs(tr - fr) == abs(tc - fc));
        if (!straight && !diagonal) return false;

        int rowStep = (tr > fr) ? 1 : (tr < fr) ? -1 : 0;
        int colStep = (tc > fc) ? 1 : (tc < fc) ? -1 : 0;
        int r = fr + rowStep, c = fc + colStep;
        while (r != tr || c != tc) {
            if (board[r][c] != nullptr) return false;
            r += rowStep; c += colStep;
        }
        return (board[tr][tc] == nullptr || board[tr][tc]->getColor() != color);
    }
};

// KING
class King : public Piece {
public:
    King(char c) : Piece(c, c == 'W' ? 'K' : 'k') {}

    bool isValidMove(int fr, int fc, int tr, int tc,
        Piece* board[8][8]) const override {
        if (abs(tr - fr) > 1 || abs(tc - fc) > 1) return false;
        return (board[tr][tc] == nullptr || board[tr][tc]->getColor() != color);
    }
};