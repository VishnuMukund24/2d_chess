#pragma once
#include "Board.h"

struct Move {
    Position from;
    Position to;
};

class MoveValidator {
    public:
        // THe master function
        static bool isValidMove(const Board& board, Move move);

    private:
        // Piece-specific rules
        static bool validatePawn(const Board& board, Move move, Color color);
        static bool validateKnight(const Board& board, Move move);
        static bool validateSlidingPiece(const Board& board, Move move); // Rook, Bishop, Queen

        // Path checking (ensure no pieces are in the way)
        static bool isPathClear(const Board& board, Move move);
};
