#include <iostream>
#include <string>

#include "Model/hdrs/GameManager.h"

// Helper to print the board to the console
void printBoard(const Board& board) {
    std::cout << "\n 0 1 2 3 4 5 6 7 (X)\n";
    for (int y = 7; y >= 0; --y) {
        std::cout << y << " ";
        for (int x = 0; x < 8; ++x) {
            Square s = board.getSquare({x, y});
            if (s.type == PieceType::None) {
                std::cout << ". ";
            } else {
                // Simplified display: Upper for White, Lower for Black
                char c = ' ';
                switch (s.type) {
                    case PieceType::Pawn:   c = 'P'; break;
                    case PieceType::Rook:   c = 'R'; break;
                    case PieceType::Knight: c = 'N'; break;
                    case PieceType::Bishop: c = 'B'; break;
                    case PieceType::Queen:  c = 'Q'; break;
                    case PieceType::King:   c = 'K'; break;
                }
                if (s.color == Color::Black) c = tolower(c);
                std::cout << c << " ";
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void testUndo(){}

int main() {
    GameManager game;
    int x1, y1, x2, y2;
    std::cout << "--- 2D C++ CHESS: CONSOLE EDITION ---\n";
    std::cout << "Enter moves as: startX startY endX endY (e.g., 0 1 0 3)\n";

    while (!game.isGameOver()) {
        printBoard(game.getBoard());

        std::cout << (game.getCurrentTurn() == Color::White ? "WHITE" : "BLACK")
                  << " to move: ";

        if (!(std::cin >> x1 >> y1 >> x2 >> y2)) {
            std::cout << "Invalid input format!\n";
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            continue;
        }

        if (x1 == -1) {
            game.undoMove();
            std::cout <<"Move undone.\n";
            continue;
        }

        if (game.makeMove({x1, y1}, {x2, y2})) {
            std::cout << "Move accepted.\n";
        } else {
            std::cout << "Move REJECTED! Try again. \n";
        }
    }

    std::cout << "Final Board State:\n";
    printBoard(game.getBoard());
    std::cout << "Thanks for playing!\n";

    return 0;
}