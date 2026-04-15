#include "hdrs/GameManager.h"
#include <iostream>

GameManager::GameManager() : currentTurn(Color::White), gameOver(false) {
    board.initializeStartingPosition();
}

bool GameManager::makeMove(Position from, Position to) {
    if (gameOver) return false;

    // 1. Capture the Original state immediately
    Square originalPiece = board.getSquare(from);

    // 1. Validate Turn: Is the player moving their own color?
    if (originalPiece.color != currentTurn) {
    std::cout << "It is not your turn!" << std::endl;
    return false;
    }

    // 2. Validate Rules: Is the move geometrically legal?
    if (MoveValidator::isValidMove(board, {from, to})) {
        Square targetSquare     = board.getSquare(to);
        PieceType promotionType = PieceType::None;

        // Create a copy of the piece to modify for the board update
        Square piecToPlace = originalPiece;

        // Check for Pawn Promotion
        if (originalPiece.type == PieceType::Pawn) {
            int promotionRank = (originalPiece.color == Color::White) ? 7 : 0;
            if (to.y == promotionRank) {
                promotionType    = PieceType::Queen; // Default to now
                piecToPlace.type = PieceType::Queen; // Change the active piece type
            }
        }

        // 2. Record the Original piece (the Pawn) into history
        // Record the command before changing the board
        history.push_back(MoveCommand(from, to, originalPiece, targetSquare, promotionType));

        // Check for simple win condition: Is a King being captured?
        if (targetSquare.type == PieceType::King) {
            gameOver = true;
            std::cout << "Checkmate! " << (currentTurn == Color::White ? "White" : "Black")
                      << " wins!" << std::endl;
        }

        // 3. Execute Move: Update the board
        board.setSquare(to, piecToPlace);
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
