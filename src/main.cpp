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

void testPawnPromotion(){
    // Testing Pawn promotion
    GameManager game;
    Board board = game.getBoard();
    board.setSquare({0, 6}, {PieceType::Pawn, Color::White});
    
}

void runPromotionTest(GameManager& game) {
    std::cout << "\n--- STARTING PROMOTION TEST ---" << std::endl;

    // 1. Manually set up a "Promotion Scenario"
    // We clear the board and place a White Pawn on the 7th rank (index 6)
    // and a Black Pawn on the 2nd rank (index 1) for two-way testing.
    Board& board = const_cast<Board&>(game.getBoard()); 
    
    // Clear a path
    Position whitePawnStart = {4, 6}; // E7
    Position whitePawnEnd   = {4, 7}; // E8
    board.setSquare(whitePawnStart, {PieceType::Pawn, Color::White});

    // 2. Execute the promotion move
    std::cout << "Moving White Pawn from E7 to E8..." << std::endl;
    if (game.makeMove(whitePawnStart, whitePawnEnd)) {
        Square result = game.getBoard().getSquare(whitePawnEnd);
        if (result.type == PieceType::Queen) {
            std::cout << "SUCCESS: Pawn promoted to Queen at E8." << std::endl;
        } else {
            std::cout << "FAILURE: Piece at E8 is still a " << (int)result.type << std::endl;
        }
    } else {
        std::cout << "ERROR: Move was rejected by GameManager!" << std::endl;
    }

    // 3. Test the Undo
    std::cout << "Undoing promotion..." << std::endl;
    game.undoMove();
    
    Square backToPawn = game.getBoard().getSquare(whitePawnStart);
    if (backToPawn.type == PieceType::Pawn && backToPawn.color == Color::White) {
        std::cout << "SUCCESS: Undo restored the White Pawn to E7." << std::endl;
    } else {
        std::cout << "FAILURE: Undo failed to restore the Pawn." << std::endl;
    }
    
    std::cout << "--- PROMOTION TEST COMPLETE ---\n" << std::endl;
}

void runExhaustiveCastlingTest(GameManager& game) {
    std::cout << "--- EXHAUSTIVE CASTLING SUITE ---\n";
    Board& board = const_cast<Board&>(game.getBoard());

    struct CastleTest {
        std::string name;
        Color turn;
        Position start;
        Position end;
        Position rStart;
        Position rEnd;
        std::vector<Position> clearPath;
    };

    std::vector<CastleTest> tests = {
        {"White Kingside", Color::White, {4, 0}, {6, 0}, {7, 0}, {5, 0}, {{5, 0}, {6, 0}}},
        {"White Queenside", Color::White, {4, 0}, {2, 0}, {0, 0}, {3, 0}, {{1, 0}, {2, 0}, {3, 0}}},
        {"Black Kingside", Color::Black, {4, 7}, {6, 7}, {7, 7}, {5, 7}, {{5, 7}, {6, 7}}},
        {"Black Queenside", Color::Black, {4, 7}, {2, 7}, {0, 7}, {3, 7}, {{1, 7}, {2, 7}, {3, 7}}}
    };

    for (const auto& t : tests) {
        std::cout << "Testing: " << t.name << "..." << std::endl;

        // 1. Setup Board for this specific test
        // Clear board first to ensure hasMoved flags are clean (fresh GameManager recommended)
        board.initializeStartingPosition(); 
        for (auto p : t.clearPath) board.setSquare(p, {PieceType::None, Color::None});
        
        // Ensure turn is correct in game manager (might need a setter or manual toggle)
        // For this test, we assume game.makeMove will handle turn validation
        
        // 2. Execute Move
        if (game.makeMove(t.start, t.end)) {
            Square k = board.getSquare(t.end);
            Square r = board.getSquare(t.rEnd);
            
            if (k.type == PieceType::King && r.type == PieceType::Rook && k.hasMoved && r.hasMoved) {
                std::cout << "  [PASS] Movement and Flags valid.\n";
            } else {
                std::cout << "  [FAIL] Piece/Flag mismatch.\n";
            }
            printBoard(board);
            
            // 3. Undo
            game.undoMove();
            Square kOld = board.getSquare(t.start);
            Square rOld = board.getSquare(t.rStart);
            
            if (kOld.type == PieceType::King && !kOld.hasMoved && rOld.type == PieceType::Rook) {
                std::cout << "  [PASS] Undo restored state.\n";
            } else {
                std::cout << "  [FAIL] Undo failed.\n";
            }
        } else {
            std::cout << "  [ERROR] Move rejected - check turn or MoveValidator.\n";
        }
    }
}

void twoDChess(GameManager& game) {
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
}

int main() {
    GameManager game;
    // twoDChess(game);
    // runPromotionTest(game);
    runExhaustiveCastlingTest(game);
    return 0;
}