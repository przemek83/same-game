#include "gtest/gtest.h"

#include "../Board.h"
#include "TestTools.h"

TEST(BoardTest, loadBoard)
{
    Board currentBoard{TestTools::prepareBoard(4, 4, "4x4_3_colors.txt")};
    std::vector<std::vector<int>> expectedBoard{
        {0, 1, 0, 0}, {0, 1, 1, 1}, {1, 2, 2, 1}, {1, 2, 0, 2}};
    EXPECT_EQ(currentBoard, TestTools::createBoard(expectedBoard));
}
