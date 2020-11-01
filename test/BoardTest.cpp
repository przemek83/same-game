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
    const Board board(expectedColumnCount, 2, stringStream);
    EXPECT_EQ(board.getColumnCount(), expectedColumnCount);
}

TEST(BoardTest, comparisonOperatorPositive)
{
    const std::vector<std::vector<int>> data{{{1, 2, 3, 4}}};
    const Board firstBoard{TestTools::createBoard(data)};
    const Board secondBoard{TestTools::createBoard(data)};
    EXPECT_TRUE(firstBoard == secondBoard);
}

TEST(BoardTest, comparisonOperatorDifferentRowCount)
{
    const Board firstBoard{TestTools::createBoard({{1, 2, 3, 4}})};
    const Board secondBoard{TestTools::createBoard({{1, 2, 3, 4, 2}})};
    EXPECT_FALSE(firstBoard == secondBoard);
}

TEST(BoardTest, comparisonOperatorDifferentColumnCount)
{
    const Board firstBoard{TestTools::createBoard({{1, 2, 3, 4}})};
    const Board secondBoard{
        TestTools::createBoard({{1, 2, 3, 4}, {1, 1, 1, 1}})};
    EXPECT_FALSE(firstBoard == secondBoard);
}

TEST(BoardTest, comparisonOperatorDifferentData)
{
    const Board firstBoard{TestTools::createBoard({{1, 2, 3, 4}})};
    const Board secondBoard{TestTools::createBoard({{1, 2, 3, 3}})};
    EXPECT_FALSE(firstBoard == secondBoard);
}

TEST(BoardTest, getColor)
{
    const Board board{TestTools::createBoard({{1, 2, 3, 4}})};
    EXPECT_EQ(board.getColor({0, 0}), 1);
    EXPECT_EQ(board.getColor({0, 1}), 2);
    EXPECT_EQ(board.getColor({0, 2}), 3);
    EXPECT_EQ(board.getColor({0, 3}), 4);
}

TEST(BoardTest, setColor)
{
    Board board{TestTools::createBoard({{1, 2, 3, 4}})};
    const Point point{0, 2};
    const int expectedColor{5};
    board.setColor(point, expectedColor);
    EXPECT_EQ(board.getColor(point), expectedColor);
}
