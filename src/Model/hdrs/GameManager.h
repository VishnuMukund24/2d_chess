#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H
#include "Board.h"
#include "MoveValidator.h"

class GameManager {
    private:
        Board board;
        Color currentTurn;
        bool gameOver;

    public:
        GameManager();

        // The main entry point for an attempted move
        bool makeMove(Position from, Position to);

        // Getters for the UI (or main.cpp) to see what's happening
        const Board& getBoard() const { return board; }
        Color getCurrentTurn() const { return currentTurn; }
        bool isGameOver() const { return gameOver; }

    private:
        void switchTurn();
};
#endif
