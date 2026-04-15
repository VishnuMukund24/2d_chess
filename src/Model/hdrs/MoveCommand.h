#ifndef MOVE_COMMAND_H
#define MOVE_COMMAND_H

#include "Types.h"

struct MoveCommand {
    Position from;
    Position to;
    Square movedPiece;
    Square capturedPiece;
    PieceType promotedTo; // None if no promotion
    bool isCastle;        // Flag to signal the Rook also moved (2 piece movement)

    MoveCommand(
        Position f, Position t, Square m, Square c,
        PieceType p = PieceType::None, bool castle = false
    )
        : from(f), to(t), movedPiece(m), capturedPiece(c), promotedTo(p), isCastle(castle) {}
};

#endif