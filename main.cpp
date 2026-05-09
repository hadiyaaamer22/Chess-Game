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
