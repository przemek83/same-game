#include "gtest/gtest.h"

#include <cstring>
#include <fstream>

#include "../Board.h"
//#include "../Point.h"
//#include "../SameGame.h"

static Board createBoard(const std::vector<std::vector<int>>& data)
{
    const unsigned int columnCount{static_cast<unsigned int>(data.size())};
    const unsigned int rowCount{static_cast<unsigned int>(data[0].size())};

    std::istringstream stringStream;
    Board board(columnCount, rowCount, stringStream);
    for (unsigned int row = 0; row < rowCount; ++row)
        for (unsigned int column = 0; column < columnCount; ++column)
            board.setColor({static_cast<int>(column), static_cast<int>(row)},
                           data[column][row]);
    return board;
}

static Board prepareBoard(unsigned int columnsCount, unsigned int rowsCount,
                          std::string fileName)
{
    std::ifstream in(fileName, std::ifstream::in);
    return Board(columnsCount, rowsCount, in);
}

TEST(BoardTest, loadBoard)
{
    Board currentBoard{prepareBoard(4, 4, "4x4_3_colors.txt")};
    std::vector<std::vector<int>> expectedBoard{
        {0, 1, 0, 0}, {0, 1, 1, 1}, {1, 2, 2, 1}, {1, 2, 0, 2}};
    EXPECT_EQ(currentBoard, createBoard(expectedBoard));
}
