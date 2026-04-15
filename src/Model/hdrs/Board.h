#pragma once
#include "Types.h"
#include <array>

class Board {
    private:
        // index = row * 8 + column
        std::array<Square, 64> grid;

    public:
        Board();

        // Helper to convert (x, y) to 0-63
        int getIndex(Position pos) const {
            return pos.y * 8 + pos.x;
        }

        // Core data access
        Square getSquare(Position pos) const {
            if (isOutOfBounds(pos)) return {PieceType::None, Color::None};
            return grid[getIndex(pos)];
        }

        void setSquare(Position pos, Square piece) {
            if (!isOutOfBounds(pos)) grid[getIndex(pos)] = piece;
        }

        bool isOutOfBounds(Position pos) const {
            return pos.x < 0 || pos.x > 7 || pos.y < 0 || pos.y > 7;
        }

        // Setup
        void initializeStartingPosition();
};
