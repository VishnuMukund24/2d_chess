# Variables
CXX = g++
CXXFLAGS = -Isrc -Isrc/Model/hdrs -std=c++17 -Wall

# Files
SRCS = src/main.cpp src/Model/Board.cpp src/Model/MoveValidator.cpp src/Model/GameManager.cpp
OBJ  = chess_test

# Build rule
all:
		$(CXX) $(CXXFLAGS) $(SRCS) -o $(OBJ)

# Clean rule to remove the executable
clean:
		rm -f $(OBJ)
