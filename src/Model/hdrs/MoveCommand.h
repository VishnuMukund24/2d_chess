#ifndef MOVE_COMMAND_H
#define MOVE_COMMAND_H

#include "Types.h"

struct MoveCommand {
    Position from;
    Position to;
    Square movedPiece;
    Square capturedPiece;

    MoveCommand(Position f, Position t, Square m, Square c)
        : from(f), to(t), movedPiece(m), capturedPiece(c) {}
};

#endif