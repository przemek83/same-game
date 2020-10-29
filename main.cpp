#include <iostream>

#include "SameGame.h"

int main()
{
    using namespace SameGame;

    srand(time(NULL));

    unsigned int rowCount{0};
    unsigned int columnCount{0};
    unsigned int colorCount{0};
    std::cin >> rowCount >> columnCount >> colorCount;

    if (!Board::isDescriptionValid(rowCount, columnCount, colorCount))
        return 1;

    Board board(columnCount, rowCount, std::cin);
    std::vector<Point> points{playGame(std::move(board))};

    for (auto point : points)
        std::cout << point.row << " " << point.column << std::endl;

    return 0;
}
