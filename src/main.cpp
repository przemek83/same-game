#include <chrono>
#include <cstdlib>
#include <iostream>

#include "NumbersGenerator.h"
#include "SameGame.h"

int main()
{
    int rowCount{0};
    int columnCount{0};
    int colorCount{0};
    std::cin >> rowCount >> columnCount >> colorCount;

    if ((rowCount > 0) && (columnCount > 0) &&
        (!Board::isDescriptionValid(colorCount)))
        return EXIT_FAILURE;

    Board board(columnCount, rowCount, std::cin);
    NumbersGenerator generator{
        std::chrono::system_clock::now().time_since_epoch().count()};
    std::vector<Point> points{SameGame::playGame(std::move(board), generator)};

    for (auto point : points)
        std::cout << point.row << " " << point.column << std::endl;

    return EXIT_SUCCESS;
}
