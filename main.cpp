#include <iostream>
#include <string>
#include "Board.h"
using namespace std;

// Convert "e2" -> row, col
bool parseSquare(const string& s, int& row, int& col) {
    if (s.size() != 2) return false;

    char file = s[0];  // a-h
    char rank = s[1];  // 1-8

    if (file < 'a' || file > 'h') return false;
    if (rank < '1' || rank > '8') return false;

    col = file - 'a';
    row = 8 - (rank - '0');

    return true;
}

// Entry point
int main() {
    cout << "========================================\n";
    cout << "        CHESS GAME         \n";
    cout << "========================================\n";
    cout << "Pieces: K/k=King  Q/q=Queen  R/r=Rook\n";
    cout << "        B/b=Bishop  N/n=Knight  P/p=Pawn\n";
    cout << "UPPERCASE = White     lowercase = Black\n";
    cout << "Enter moves like: e2 e4\n";
    cout << "Type 'quit' to exit.\n";

    Board board;
    char turn = 'W';   // White goes first

    while (true) {
        board.display();

        cout << (turn == 'W' ? "White" : "Black") << "'s turn.\n";
        cout << "Enter move (from to): ";

        string from, to;
        cin >> from;

        if (from == "quit") {
            cout << "Game ended by player.\n";
            break;
        }

        cin >> to;

        int fr, fc, tr, tc;

        if (!parseSquare(from, fr, fc) || !parseSquare(to, tr, tc)) {
            cout << "Bad input! Use format like 'e2 e4'.\n";
            continue;
        }

        if (board.movePiece(fr, fc, tr, tc, turn)) {
            turn = (turn == 'W') ? 'B' : 'W';  // switch turn
        }
    }

    return 0;
}