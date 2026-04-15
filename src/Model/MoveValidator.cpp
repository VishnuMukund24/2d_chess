#include "hdrs/MoveValidator.h"
#include <cmath>

bool MoveValidator::isValidMove(const Board& board, Move move) {
    Square source = board.getSquare(move.from);
    Square target = board.getSquare(move.to);

    // 1. Basic checks
    if (source.type == PieceType::None) return false;
    if (move.from == move.to) return false;
    if (source.color == target.color) return false; // Can't capture own piece

    // 2. Route to specific logic
    switch (source.type) {
        case PieceType::Rook:
            return (move.from.x == move.to.x || move.from.y == move.to.y) && isPathClear(board, move);

        case PieceType::Bishop:
            return (std::abs(move.from.x - move.to.x) == std::abs(move.from.y - move.to.y)) && isPathClear(board, move);

        case PieceType::Queen: {
            // Queen is just Rook + Bishop logic
            bool isStraight = (move.from.x == move.to.x || move.from.y == move.to.y);
            bool isDiagonal = (std::abs(move.from.x - move.to.x) == std::abs(move.from.y - move.to.y));
            return (isStraight || isDiagonal) && isPathClear(board, move);
        }

        case PieceType::Pawn:
            return validatePawn(board, move, source.color);

        case PieceType::Knight:
            return validateKnight(board, move);

        case PieceType::King: {
            int dx = std::abs(move.to.x - move.from.x);
            int dy = std::abs(move.to.y - move.from.y);

            if (dx == 2 && dy == 0) {
                // Determine if it's Kingside (right) or Queenside (left)
                int rookX   = (move.to.x > move.from.x) ? 7 : 0;
                Square rook = board.getSquare({rookX, move.from.y});

                // 1. Check if King and Rook have moved
                if (source.hasMoved || rook.type != PieceType::Rook || rook.hasMoved) return false;

                // 2. Check if path is clear
                return isPathClear(board, {move.from, {rookX, move.from.y}});
            }

            // King moves exactly 1 square in any direction (horizontal, vertical, or diagonal)
            return (dx <= 1 && dy <= 1);
        }
    }

    return false;
}

bool MoveValidator::isPathClear(const Board& board, Move move) {
    int dx = (move.to.x > move.from.x) ? 1 : (move.to.x < move.from.x ? -1 : 0);
    int dy = (move.to.y > move.from.y) ? 1 : (move.to.y < move.from.y ? -1 : 0);

    Position current = { move.from.x + dx, move.from.y + dy };

    // Check every square between start and end
    while (!(current == move.to)) {
        if (board.getSquare(current).type != PieceType::None) {
            return false; // Path blocked
        }
        current.x += dx;
        current.y += dy;
    }
    return true;
}

bool MoveValidator::validatePawn(const Board& board, Move move, Color color) {
    int direction = (color == Color::White) ? 1 : -1;
    int startRank = (color == Color::White) ? 1 : 6;

    int dx = move.to.x - move.from.x;
    int dy = move.to.y - move.from.y;

    Square target = board.getSquare(move.to);

    // 1. Forward Movement (No capturing)
    if (dx == 0) {
        // Single step
        if (dy == direction && target.type == PieceType::None) {
            return true;
        }
        // Double step from starting position
        if (move.from.y == startRank && dy == 2 * direction) {
            Position intermediate = { move.from.x, move.from.y + direction };
            if (target.type == PieceType::None && board.getSquare(intermediate).type == PieceType::None) {
                return true;
            }
        }
    }
    // 2. Diagonal Capture
    else if (std::abs(dx) == 1 && dy == direction) {
        if (target.type != PieceType::None && target.color != color) {
            return true;
        }
    }

    return false;
}

bool MoveValidator::validateKnight(const Board& board, Move move) {
    int dx = std::abs(move.to.x - move.from.x);
    int dy = std::abs(move.to.y - move.from.y);
    return (dx == 1 && dy == 2) || (dx == 2 && dy == 1);
}