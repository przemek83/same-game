// Simple board game based on rules of Same Game (known also as JawBreaker and
// few other names). Played by basic algorithm.

// Rules:
// Program gets H x W board on input with C different numbers as cells.
// Assumption:
//  4 <= H <= 50 – board rows count
//  4 <= W <= 50 – board columns count
//  3 <= C <= 20 – colour count
// Program can remove cells if there are minimum 1 neighbour in same colour
// (above, below, left or right). Gravity fill the gaps if occur.
// Game ends when on boards there is no cells with neighbour in same colour.

// Input:
// Line containing H W C (space separated) followed by
// H lines with W numbers representing initial board.

// Output:
// Coordinates of cells to pick. At end "-1 -1" meaning no more moves.

// Example:
// In
//    4 4 3
//    0 0 1 1
//    1 1 2 2
//    0 1 2 0
//    0 1 1 2

// Out
//    1 0
//    1 0
//    3 2
//    -1 -1

#include <cstring>
#include <iostream>
#include <sstream>
#include <vector>

#include "SameGame.h"

int main()
{
    using namespace SameGame;

    srand(time(NULL));

    unsigned int rowCount{0};
    unsigned int columnCount{0};
    unsigned int colorCount{0};
    std::cin >> rowCount >> columnCount >> colorCount;

    if (rowCount < MIN_H || rowCount > MAX_H || columnCount < MIN_W ||
        columnCount > MAX_W || colorCount < MIN_C || colorCount > MAX_C)
        return 1;

    std::vector<std::vector<int>> board{
        loadBoard(columnCount, rowCount, std::cin)};

    std::vector<Point> points{playGame(std::move(board))};
    std::ostringstream output;
    for (auto point : points)
        printPoint(point, output);
    std::cout << output.str();

    return 0;
}
