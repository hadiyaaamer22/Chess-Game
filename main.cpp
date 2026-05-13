// Source.cpp
#include "Piece.h"

int main() {
    // Window setup
    Vector2u windowSize(TILE_SIZE * BOARD_SIZE, TILE_SIZE * BOARD_SIZE);
    RenderWindow chessGUI(VideoMode(windowSize), "Chess Game");
    chessGUI.setFramerateLimit(60);

    // Board
    chessBoard board;
    board.initPieces();

    //try {
    //    board.preloadTextures();
    //}
    //catch (const char* e) {
    //    cerr << e << endl;
    //    chessGUI.close();
    //    return EXIT_FAILURE;  // game loop never starts
    //}

    // Game loop
    while (chessGUI.isOpen()) {
        while (const auto event = chessGUI.pollEvent()) {

            //check event
            if (event->is<Event::Closed>())
                chessGUI.close();

            if (const auto* mousePressed = event->getIf<Event::MouseButtonPressed>()) {
                if (mousePressed->button == Mouse::Button::Left) {
                    int mx = mousePressed->position.x;
                    int my = mousePressed->position.y;

                    board.handleClick(mx, my);
                }
            }
        }
        // game over
        if (board.isGameOver()) {
            int winner = board.getWinner();
            string Winner = winner ? "Black Won" : "White Won";
            chessGUI.setTitle("Game Over " + Winner);
        }
        // Render
        chessGUI.clear(Color(30, 30, 30));
        board.drawBoard(chessGUI);

        chessGUI.display();
    }
    return EXIT_SUCCESS;
}
