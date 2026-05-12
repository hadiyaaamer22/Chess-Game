// main.cpp
// Contains Game class implementation and program entry point

#include "chess.h"
#include <sstream>   // for string stream parsing
#include <limits>    // for numeric_limits

// ============================================================
//  GAME CLASS IMPLEMENTATION
// ============================================================

Game::Game() {
    currentTurn = 'W'; // White always goes first
    moveCount = 0;
    gameOver = false;
}

// Print a colorful title banner
void Game::printTitle() const {
    cout << "\n";
    cout << COLOR_TITLE << "  ║          CHESS GAME IN C++           ║\n" << RESET;
    cout << COLOR_TITLE << "  ║            OOP Lab Semester Project   ║\n" << RESET;
    cout << "\n";
    cout << COLOR_INFO << "  HOW TO PLAY:\n" << RESET;
    cout << "  Enter moves like:  " << BOLD << "e2 e4" << RESET
        << "  (column+row  column+row)\n";
    cout << "  Type " << BOLD << "quit" << RESET << " to exit the game.\n";
    cout << "  Type " << BOLD << "help" << RESET << " to see this guide again.\n";
    cout << "\n";
    cout << COLOR_INFO << "  PIECE SYMBOLS:\n" << RESET;
    cout << "  K=King  Q=Queen  R=Rook  B=Bishop  N=Knight  P=Pawn\n";
    cout << "  " << WHITE_PIECE << "UPPER" << RESET << " = White pieces    "
        << BLACK_PIECE << "LOWER" << RESET << " = Black pieces (shown same letter)\n\n";

    // Legend for the board colors
    cout << "  Board: " << BG_HIGHLIGHT << "   " << RESET << " = last move    "
        << BG_LIGHT << "   " << RESET << " = light square    "
        << BG_DARK << "   " << RESET << " = dark square\n\n";
}

// Print info about whose turn it is
void Game::printTurnInfo() const {
    string playerName = (currentTurn == 'W') ? "WHITE" : "BLACK";
    string colorCode = (currentTurn == 'W') ? WHITE_PIECE : BLACK_PIECE;

    cout << colorCode << BOLD
        << "  Move #" << (moveCount + 1)
        << " - " << playerName << "'s Turn"
        << RESET << "\n";
    cout << "  Enter move (e.g. e2 e4): ";
}

// Convert column letter 'a'-'h' to index 0-7
int Game::colToIndex(char c) const {
    return c - 'a'; // 'a'=0, 'b'=1, ... 'h'=7
}

// Convert row character '1'-'8' to array index 0-7
int Game::rowToIndex(char c) const {
    return 8 - (c - '0'); // '8'=0 (top), '1'=7 (bottom)
}

// Parse input string like "e2 e4" into four integers
bool Game::parseInput(const string& input,
    int& fromRow, int& fromCol,
    int& toRow, int& toCol) const {
    // Expected format: "c1 c2" e.g. "e2 e4"
    if (input.length() < 5) return false;

    char fc = input[0]; // from column letter
    char fr = input[1]; // from row number
    char tc = input[3]; // to column letter
    char tr = input[4]; // to row number

    // Validate column: must be a-h
    if (fc < 'a' || fc > 'h') return false;
    if (tc < 'a' || tc > 'h') return false;
    // Validate row: must be 1-8
    if (fr < '1' || fr > '8') return false;
    if (tr < '1' || tr > '8') return false;

    fromCol = colToIndex(fc);
    fromRow = rowToIndex(fr);
    toCol = colToIndex(tc);
    toRow = rowToIndex(tr);

    return true;
}

// Main game loop
void Game::run() {
    printTitle();

    while (!gameOver) {
        // Display current board state
        board.display();

        // Check for checkmate or stalemate BEFORE the player moves
        if (board.isCheckmate(currentTurn)) {
            char winner = (currentTurn == 'W') ? 'B' : 'W';
            string winnerName = (winner == 'W') ? "WHITE" : "BLACK";
            cout << COLOR_WIN << BOLD
                << "\n  *** CHECKMATE! " << winnerName << " WINS! ***\n"
                << RESET;
            gameOver = true;
            break;
        }
        if (board.isStalemate(currentTurn)) {
            cout << COLOR_WIN << BOLD
                << "\n  *** STALEMATE! It's a DRAW! ***\n"
                << RESET;
            gameOver = true;
            break;
        }

        // Warn if current player is in check
        if (board.isInCheck(currentTurn)) {
            cout << COLOR_ERROR << BOLD
                << "  *** CHECK! Your King is under attack! ***\n"
                << RESET;
        }

        // Show whose turn it is and get input
        printTurnInfo();

        string input;
        getline(cin, input);

        // Handle special commands
        if (input == "quit" || input == "exit") {
            cout << COLOR_MSG << "\n  Thanks for playing! Goodbye!\n" << RESET;
            break;
        }
        if (input == "help") {
            printTitle();
            continue;
        }

        // Parse the move input
        int fromRow, fromCol, toRow, toCol;
        if (!parseInput(input, fromRow, fromCol, toRow, toCol)) {
            cout << COLOR_ERROR
                << "  Invalid input format! Use format like: e2 e4\n"
                << RESET;
            continue;
        }

        // Check source square has a piece
        Piece* piece = board.getPiece(fromRow, fromCol);
        if (piece == nullptr) {
            cout << COLOR_ERROR << "  No piece at that position!\n" << RESET;
            continue;
        }

        // Check it's the correct player's piece
        if (piece->getColor() != currentTurn) {
            string playerName = (currentTurn == 'W') ? "White" : "Black";
            cout << COLOR_ERROR
                << "  That is not your piece! " << playerName << " must move.\n"
                << RESET;
            continue;
        }

        // Attempt the move
        bool moved = board.movePiece(fromRow, fromCol, toRow, toCol);

        if (!moved) {
            cout << COLOR_ERROR
                << "  Invalid move! That move is not allowed.\n"
                << RESET;
            continue;
        }

        // Move was successful - switch turns
        moveCount++;
        currentTurn = (currentTurn == 'W') ? 'B' : 'W';
    }

    // Final board display
    board.display();
    cout << COLOR_TITLE << "  Game Over! Total moves: " << moveCount << "\n\n" << RESET;
}

// ============================================================
//  ENTRY POINT
// ============================================================
int main() {
    Game game; // Create a game object
    game.run(); // Start the game
    return 0;
}
