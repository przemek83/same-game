// Simple board game based on rules of Same Game (known also as JawBreaker and
// few other names). Played by basic algorithm.

// Rules:
// Program gets H x W board on input with C different numbers as cells.
// Assumption:
//  4 <= H <= 50 – board rows count
//  4 <= W <= 50 – board columns count
//  3 <= C <= 20 – color count
// Program can remove cells if there are minimum 1 neighbour in same color
// (above, below, left or right). Gravity fill the gaps if occur.
// Game ends when on boards there is no cells with neighbour in same color.

// Input:
// Number of games followed by line containing H W C (space separated) followed
// H lines with W numbers representing initial board.

// Output:
// In first line "Y" if want to proceed with game and "N" if not followed by
// coordinates of cells to pick. At end "-1 -1" meaning no more moves.

// Example:
// In
//    1
//    4 4 3
//    0 0 1 1
//    1 1 2 2
//    0 1 2 0
//    0 1 1 2

// Out
//    Y
//    1 0
//    1 0
//    3 2
//    -1 -1

//#include <chrono>
#include <cstring>
#include <iostream>
#include <sstream>
#include <vector>

#include "SameGame.h"

int main()
{
    using namespace SameGame;
    // auto start = std::chrono::high_resolution_clock::now();
    // test();
    // bigTestFile3Colors.txt bigTestFile.txt
    // testFileVeryBig
    // testFileVeryBig3Colors.txt
    // testFileVeryVeryBig.txt std::ifstream in("testFileVeryVeryBig.txt",
    // std::ifstream::in); std::cin.rdbuf(in.rdbuf());

    srand(time(NULL));

    unsigned int testCount;
    std::cin >> testCount;
    for (unsigned int i = 0; i < testCount; ++i)
    {
        unsigned int h;
        unsigned int w;
        unsigned int c;
        std::cin >> h >> w >> c;

        if (h < MIN_H || h > MAX_H || w < MIN_W || w > MAX_W || c < MIN_C ||
            c > MAX_C)
        {
            std::cout << "N" << std::endl;
            continue;
        }

        std::vector<std::vector<int>> board{loadBoard(w, h, std::cin)};

        std::ostringstream output;
        // printBoard(board);
        output << "Y" << std::endl;

        std::vector<Point> points{playGame(w, h, board)};
        for (auto point : points)
            printPoint(point, output);
        std::cout << output.str();
    }

    //    auto stop = std::chrono::high_resolution_clock::now();
    //    auto duration =
    //        std::chrono::duration_cast<std::chrono::milliseconds>(stop -
    //        start);
    //    std::cout << duration.count() << std::endl;
    return 0;
}
