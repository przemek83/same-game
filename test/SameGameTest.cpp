#include "gtest/gtest.h"

#include <cstring>

#include "../SameGame.h"

using namespace SameGame;

void performImpactGravityTest(std::vector<std::vector<int>> board,
                              int (&impactedColumns)[MAX_W],
                              std::vector<std::vector<int>> expected,
                              std::string testName)
{
    impactGravity(board, impactedColumns);
    if (board != expected)
    {
        std::cout << "FAIL: impactGravity " + testName << std::endl;
        std::cout << "Got:" << std::endl;
        printBoard(board);
        std::cout << "Expected:" << std::endl;
        printBoard(expected);
    }
    else
        std::cout << "SUCCESS: impactGravity " + testName << std::endl;
}

void performMakeMoveTest(std::vector<std::vector<int>> board,
                         bool (&checked)[MAX_W][MAX_H],
                         int (&impactedColumns)[MAX_W], const Point& point,
                         std::vector<std::vector<int>> expected,
                         std::string testName)
{
    makeMove(board, checked, impactedColumns, point);
    if (board != expected)
    {
        std::cout << "FAIL: makeMove " + testName << std::endl;
        std::cout << "Got:" << std::endl;
        printBoard(board);
        std::cout << "Expected:" << std::endl;
        printBoard(expected);
    }
    else
        std::cout << "SUCCESS: makeMove " + testName << std::endl;
}

void performGetClusterTests(const std::vector<std::vector<int>>& board,
                            unsigned int expectedClusterSize, Point point,
                            std::string testName)
{
    bool checked[MAX_W][MAX_H] = {};
    std::memset(checked, false, sizeof(checked));
    unsigned int currentClusterSize{
        getClusterSize(board, point, checked, board.size(), board[0].size())};
    if (currentClusterSize != expectedClusterSize)
    {
        std::cout << "FAIL: getCluster " + testName << std::endl;
        std::cout << "Got:" << currentClusterSize << ", expected "
                  << expectedClusterSize << std::endl;
    }
    else
        std::cout << "SUCCESS: getCluster " + testName << std::endl;
}

void test()
{
    std::vector<std::vector<int>> board{{1, 3, 4, -1, 5}};
    std::vector<std::vector<int>> expected{{-1, 1, 3, 4, 5}};
    int impactedColumns[MAX_W];
    impactedColumns[0] = 3;
    performImpactGravityTest(board, impactedColumns, expected, "1");
    board = {{1, 3, -1, -1, -1}};
    expected = {{-1, -1, -1, 1, 3}};
    impactedColumns[0] = 4;
    performImpactGravityTest(board, impactedColumns, expected, "2");
    board = {{-1, -1, -1, -1, -1}};
    expected = {{-1, -1, -1, -1, -1}};
    impactedColumns[0] = EMPTY;
    performImpactGravityTest(board, impactedColumns, expected, "3");
    board = {{1, 2, 3, 4, 5}};
    expected = {{1, 2, 3, 4, 5}};
    impactedColumns[0] = EMPTY;
    performImpactGravityTest(board, impactedColumns, expected, "4");
    board = {{1, -1, -1, -1, -1}, {1, 2, -1, -1, -1},  {1, -1, -1, 1, -1},
             {1, -1, -1, -1, 2},  {-1, -1, -1, 1, -1}, {1, -1, 1, -1, 1}};
    expected = {{-1, -1, -1, -1, 1}, {-1, -1, -1, 1, 2},  {-1, -1, -1, 1, 1},
                {-1, -1, -1, 1, 2},  {-1, -1, -1, -1, 1}, {-1, -1, 1, 1, 1}};
    impactedColumns[0] = 4;
    impactedColumns[1] = 4;
    impactedColumns[2] = 4;
    impactedColumns[3] = 3;
    impactedColumns[4] = 4;
    impactedColumns[5] = 3;
    performImpactGravityTest(board, impactedColumns, expected, "5");

    bool checked[MAX_W][MAX_H] = {};
    checked[2][3] = true;
    checked[2][4] = true;
    checked[3][3] = true;
    std::memset(impactedColumns, EMPTY, sizeof(impactedColumns));

    performMakeMoveTest(expected, checked, impactedColumns, {2, 3},
                        {{-1, -1, -1, -1, 1},
                         {-1, -1, -1, -1, 2},
                         {-1, -1, -1, -1, -1},
                         {-1, -1, -1, -1, 2},
                         {-1, -1, -1, -1, 1},
                         {-1, -1, 1, 1, 1}},
                        "1");

    if (checked[2][3] || checked[2][4] || checked[3][3])
        std::cout << "FAIL: makeMove checked" << std::endl;

    board = {{0, 1, 0, 0}, {0, 1, 1, 1}, {1, 2, 2, 1}, {1, 2, 0, 2}};
    expected = {{0, -1, 0, 0}, {0, -1, -1, -1}, {1, 2, 2, -1}, {1, 2, 0, 2}};
    performMakeMoveTest(board, checked, impactedColumns, {2, 3}, expected, "2");

    board = {{0, 1, 0, 0}, {0, 1, 1, 1}, {1, 2, 2, 1}, {1, 2, 0, 2}};
    performGetClusterTests(board, 2, {0, 0}, "1");
    performGetClusterTests(board, 5, {0, 1}, "2");
    performGetClusterTests(board, 1, {3, 2}, "3");
    performGetClusterTests(board, 3, {2, 1}, "4");

    board = {{-1, 1, 0, 0}};
    performGetClusterTests(board, 0, {0, 0}, "5");
    performGetClusterTests(board, 1, {0, 1}, "6");
    performGetClusterTests(board, 2, {0, 3}, "7");
    std::cout << "========== END TESTS ===========" << std::endl;
}

TEST(SameGameTest, test)
{
    test();
    EXPECT_EQ(true, true);
}
