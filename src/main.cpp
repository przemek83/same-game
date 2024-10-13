#include <cstdlib>
#include <ctime>
#include <iostream>

#include "SameGame.h"

int main()
{
    srand(static_cast<unsigned int>(time(nullptr)));

    int rowCount{0};
    int columnCount{0};
    int colorCount{0};
    std::cin >> rowCount >> columnCount >> colorCount;

    if ((rowCount > 0) && (columnCount > 0) &&
        (!Board::isDescriptionValid(colorCount)))
        return EXIT_FAILURE;

    Board board(columnCount, rowCount, std::cin);
    std::vector<Point> points{SameGame::playGame(std::move(board))};

    for (auto point : points)
        std::cout << point.row << " " << point.column << std::endl;

    return EXIT_SUCCESS;
}
