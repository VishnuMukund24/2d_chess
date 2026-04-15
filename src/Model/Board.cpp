#include "hdrs/Board.h"

Board::Board() {
    grid.fill({PieceType::None, Color::None});
}

void Board::initializeStartingPosition() {
    // Helper to set rows (Lambda function)
    auto setupRow = [&](int y, Color color) {
        PieceType powerPieces[] = {
            PieceType::Rook, PieceType::Knight, PieceType::Bishop, PieceType::Queen,
            PieceType::King, PieceType::Bishop, PieceType::Knight, PieceType::Rook
        };
        for (int x = 0; x < 8; ++x) {
            setSquare({x, y}, {powerPieces[x], color});
            // Set pawns in the next/previous row
            int pawnY = (color == Color::White) ? y + 1: y - 1;
            setSquare({x, pawnY}, {PieceType::Pawn, color});
        }
    };

    setupRow(0, Color::White); // White power pieces at y=0, pawns at y=1
    setupRow(7, Color::Black); // Black power pieces at y=7, pawns at y=6
}
