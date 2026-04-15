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
    if (MoveValidator::isValidMove(board, {from, to})) {
        Square targetSquare = board.getSquare(to);

        // Record the command before changing the board
        history.push_back(MoveCommand(from, to, piece, targetSquare));

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

void GameManager::undoMove() {
    if (history.empty()) return;

    // Get the last move
    MoveCommand lastMove = history.back();
    history.pop_back();

    // Reverse the board state
    board.setSquare(lastMove.from, lastMove.movedPiece);
    board.setSquare(lastMove.to, lastMove.capturedPiece);

    // Reset game state if it was over
    gameOver = false;

    // Crucial: Switch the turn back!
    switchTurn();
}
