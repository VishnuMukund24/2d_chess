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
        bool wasCastle          = false;

        // Create a copy of the piece to modify for the board update
        Square pieceToPlace = originalPiece;

        // CASTLING LOGIC START
        if (originalPiece.type == PieceType::King && std::abs(to.x - from.x) == 2) {
            wasCastle = true;
            int rookFromX = (to.x > from.x) ? 7 : 0;
            int rookToX   = (to.x > from.x) ? 5 : 3;

            Square rook   = board.getSquare({rookFromX, from.y});

            // Move the Rook manually
            board.setSquare({rookToX, from.y}, rook);
            board.setSquare({rookFromX, from.y}, {PieceType::None, Color::None});
        }
        // CASTLING LOGIC END

        // Check for Pawn Promotion
        if (originalPiece.type == PieceType::Pawn) {
            int promotionRank = (originalPiece.color == Color::White) ? 7 : 0;
            if (to.y == promotionRank) {
                promotionType     = PieceType::Queen; // Default to now
                pieceToPlace.type = PieceType::Queen; // Change the active piece type
            }
        }

        // 2. Record the Original piece (the Pawn) into history
        // Record the command before changing the board
        history.push_back(MoveCommand(from, to, originalPiece, targetSquare, promotionType, wasCastle));

        // Check for simple win condition: Is a King being captured?
        if (targetSquare.type == PieceType::King) {
            gameOver = true;
            std::cout << "Checkmate! " << (currentTurn == Color::White ? "White" : "Black")
                      << " wins!" << std::endl;
        }

        // 3. Execute Move: Update the board
        board.setSquare(to, pieceToPlace);
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

    // Reverse the board state (Basic)
    board.setSquare(lastMove.from, lastMove.movedPiece);
    board.setSquare(lastMove.to, lastMove.capturedPiece);
 
    // Spectial Castling reversal
    if (lastMove.isCastle) {
        int rookFromX = (lastMove.to.x > lastMove.from.x) ? 7 : 0;
        int rookToX   = (lastMove.to.x > lastMove.from.x) ? 5 : 3;

        // Find the Rook at its castled postion and move it back to the corner
        Square rook   = board.getSquare({rookToX, lastMove.from.y});
        board.setSquare({rookFromX, lastMove.from.y}, rook);
        board.setSquare({rookToX, lastMove.from.y}, {PieceType::None, Color::None});
    }
    // Reset game state if it was over
    gameOver = false;

    // Crucial: Switch the turn back!
    switchTurn();
}
