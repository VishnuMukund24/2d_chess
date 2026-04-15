#pragma once

enum class Color {
    None,
    White,
    Black
};

enum class PieceType {
    None,
    Pawn,
    Knight,
    Bishop,
    Rook,
    Queen,
    King
};

struct Position {
    int x, y;

    // Helper to compare positions
    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
};

struct Square {
    PieceType type = PieceType::None;
    Color color    = Color::None;
};

