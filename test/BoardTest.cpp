#include "gtest/gtest.h"

#include "../Board.h"
#include "TestTools.h"

TEST(BoardTest, createBoard)
{
    Board currentBoard{TestTools::prepareBoard(4, 4, "4x4_3_colors.txt")};
    std::vector<std::vector<int>> expectedBoard{
        {0, 1, 0, 0}, {0, 1, 1, 1}, {1, 2, 2, 1}, {1, 2, 0, 2}};
    EXPECT_EQ(currentBoard, TestTools::createBoard(expectedBoard));
}

TEST(BoardTest, getRowCount)
{
    const int expectedRowCount{2};
    std::istringstream stringStream;
    Board board(5, expectedRowCount, stringStream);
    EXPECT_EQ(board.getRowCount(), expectedRowCount);
}

TEST(BoardTest, getColumnCount)
{
    const int expectedColumnCount{5};
    std::istringstream stringStream;
    Board board(expectedColumnCount, 2, stringStream);
    EXPECT_EQ(board.getColumnCount(), expectedColumnCount);
}

TEST(BoardTest, comparisonOperatorPositive)
{
    std::vector<std::vector<int>> data{{{1, 2, 3, 4}}};
    Board firstBoard{TestTools::createBoard(data)};
    Board secondBoard{TestTools::createBoard(data)};
    EXPECT_TRUE(firstBoard == secondBoard);
}
