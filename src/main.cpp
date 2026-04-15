#include "Model/hdrs/Board.h"
#include "Model/hdrs/MoveValidator.h"
#include <iostream>

int main() {
    // Board board;
    // board.initializeStartingPosition();

    // Test 1: Moving a White Rook through a Pawn (Should be False)
    // Move illegalRookMove = { {0,0}, {0,3} };
    // if (!MoveValidator::isValidMove(board, illegalRookMove)) {
    //     std::cout << "invalid move" << std::endl;
    // }

    // Test 2: White Pawn Move Forward 2 (Should be True)
    // Move pawnDouble = { {0,1}, {0,3} };
    // if (MoveValidator::isValidMove(board, pawnDouble)) {
    //     std::cout << "Pawn double step: VALID" << std::endl;
    // }

    // Test 3: White Pawn Diagonal Move into Empty Square (Should be False)
    // Move pawnIllegalCapture = { {0,1}, {1,2} };
    // if (!MoveValidator::isValidMove(board, pawnIllegalCapture)) {
    //     std::cout << "Pawn illegal diagonal: BLOCKED" << std::endl;
    // }

    Board board;
    // We'll place a King manually for a clean test no initializeStartingPosition();
    Position kingPos = {4, 4}; // E5
    board.setSquare(kingPos, {PieceType::King, Color::White});

    // Test 4: Valid Diagonal Move (Should be True)
    Move validKingMove = { kingPos, {5, 5} };
    if (MoveValidator::isValidMove(board, validKingMove)) {
        std::cout << "King move E5 to F6: VALID" << std::endl;
    }

    // Test 5: Invalid "Teleport" Move (Should be False)
    Move illegalKingMove = { kingPos, {4, 6} };
    if (!MoveValidator::isValidMove(board, illegalKingMove)) {
        std::cout << "King move E5 to E7: BLOCKED (Too far)" << std::endl;
    }

    return 0;
}