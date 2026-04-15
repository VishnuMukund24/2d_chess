#include "hdrs/GameManager.h"
#include <iostream>

GameManager::GameManager() : currentTurn(Color::White), gameOver(false) {
    board.initializeStartingPosition();
}

bool GameManager::makeMove(Position from, Position to) {
    if (gameOver) return false;

    Square piece = board.getSquare(from);

    // 1. Validate Turn: Is the player moving their own color?
    if (piece.color != currentTurn) {
    std::cout << "It is not your turn!" << std::endl;
    return false;
    }

    // 2. Validate Rules: Is the move geometrically legal?
    Move move = {from, to};
    if (MoveValidator::isValidMove(board, move)) {

        // Check for simple win condition: Is a King being captured?
        if (board.getSquare(to).type == PieceType::King) {
            gameOver = true;
            std::cout << "Checkmate! " << (currentTurn == Color::White ? "White" : "Black")
                      << " wins!" << std::endl;
        }

        // 3. Execute Move: Update the board
        board.setSquare(to, piece);
        board.setSquare(from, {PieceType::None, Color::None});

        switchTurn();
        return true;
    }
    return false;
}

void GameManager::switchTurn() {
    currentTurn = (currentTurn == Color::White) ? Color::Black : Color::White;
}
