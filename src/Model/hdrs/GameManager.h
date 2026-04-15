#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <vector>

#include "Board.h"
#include "MoveValidator.h"
#include "MoveCommand.h"

class GameManager {
    private:
        Board board;
        Color currentTurn;
        bool gameOver;
        std::vector<MoveCommand> history; // The undo stack

    public:
        GameManager();

        // The main entry point for an attempted move
        bool makeMove(Position from, Position to);

        void undoMove(); // New Method

        // Getters for the UI (or main.cpp) to see what's happening
        const Board& getBoard() const { return board; }
        Color getCurrentTurn() const { return currentTurn; }
        bool isGameOver() const { return gameOver; }

    private:
        void switchTurn();
};
#endif
