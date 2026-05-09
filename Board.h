#pragma once

#include "Piece.h"
#include <string>
using namespace std;

// Board class (Composition: Board HAS Piece objects)
class Board {
private:
    Piece* grid[8][8];

public:
    Board();
    ~Board();

    void setupPieces();
    void display() const;

    // Returns true if move was made, false if illegal
    bool movePiece(int fr, int fc, int tr, int tc, char currentTurn);
};
