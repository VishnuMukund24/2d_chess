#ifndef MOVE_COMMAND_H
#define MOVE_COMMAND_H

#include "Types.h"

struct MoveCommand {
    Position from;
    Position to;
    Square movedPiece;
    Square capturedPiece;
    PieceType promotedTo; // None if no promotion

    MoveCommand(Position f, Position t, Square m, Square c, PieceType p = PieceType::None)
        : from(f), to(t), movedPiece(m), capturedPiece(c), promotedTo(p) {}
};

#endif