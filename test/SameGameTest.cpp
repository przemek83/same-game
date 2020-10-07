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
    EXPECT_EQ(board, expected);
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

TEST(SameGameTest, impactGravity)
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
}

TEST(SameGameTest, makeMove6x5)
{
    const std::vector<std::vector<int>> expected{
        {-1, -1, -1, -1, 1}, {-1, -1, -1, -1, 2}, {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, 2}, {-1, -1, -1, -1, 1}, {-1, -1, 1, 1, 1}};
    std::vector<std::vector<int>> board{{-1, -1, -1, -1, 1}, {-1, -1, -1, 1, 2},
                                        {-1, -1, -1, 1, 1},  {-1, -1, -1, 1, 2},
                                        {-1, -1, -1, -1, 1}, {-1, -1, 1, 1, 1}};
    int impactedColumns[MAX_W]{};
    std::memset(impactedColumns, EMPTY, sizeof(impactedColumns));

    bool checked[MAX_W][MAX_H]{};
    checked[1][3] = true;
    checked[2][3] = true;
    checked[2][4] = true;
    checked[3][3] = true;

    makeMove(board, checked, impactedColumns, {2, 3});
    EXPECT_EQ(board, expected);

    EXPECT_FALSE(checked[1][3]);
    EXPECT_FALSE(checked[2][3]);
    EXPECT_FALSE(checked[2][4]);
    EXPECT_FALSE(checked[3][3]);

    EXPECT_EQ(impactedColumns[0], EMPTY);
    EXPECT_EQ(impactedColumns[1], 3);
    EXPECT_EQ(impactedColumns[2], 4);
    EXPECT_EQ(impactedColumns[3], 3);
    EXPECT_EQ(impactedColumns[4], EMPTY);
}

TEST(SameGameTest, makeMove4x4)
{
    std::vector<std::vector<int>> board{
        {0, 1, 0, 0}, {0, 1, 1, 1}, {1, 2, 2, 1}, {1, 2, 0, 2}};
    const std::vector<std::vector<int>> expected{
        {0, -1, 0, 0}, {0, -1, -1, -1}, {1, 2, 2, -1}, {1, 2, 0, 2}};
    bool checked[MAX_W][MAX_H]{};
    checked[0][1] = true;
    checked[1][1] = true;
    checked[1][2] = true;
    checked[1][3] = true;
    checked[2][3] = true;

    int impactedColumns[MAX_W]{};
    std::memset(impactedColumns, EMPTY, sizeof(impactedColumns));
    makeMove(board, checked, impactedColumns, {2, 3});
    EXPECT_EQ(board, expected);

    EXPECT_FALSE(checked[0][1]);
    EXPECT_FALSE(checked[1][1]);
    EXPECT_FALSE(checked[1][2]);
    EXPECT_FALSE(checked[1][3]);
    EXPECT_FALSE(checked[2][3]);

    EXPECT_EQ(impactedColumns[0], 1);
    EXPECT_EQ(impactedColumns[1], 3);
    EXPECT_EQ(impactedColumns[2], 3);
    EXPECT_EQ(impactedColumns[3], EMPTY);
}

class GetClusterTests
    : public ::testing::TestWithParam<
          std::tuple<std::vector<std::vector<int>>, int, Point>>
{
};

TEST_P(GetClusterTests, GetCluster)
{
    const std::vector<std::vector<int>> board{std::get<0>(GetParam())};
    const int expectedClusterSize{std::get<1>(GetParam())};
    const Point point{std::get<2>(GetParam())};

    bool checked[MAX_W][MAX_H]{};
    std::memset(checked, false, sizeof(checked));
    unsigned int currentClusterSize{
        getClusterSize(board, point, checked, board.size(), board[0].size())};
    EXPECT_EQ(currentClusterSize, expectedClusterSize);
}

static std::vector<std::vector<int>> symmetricalBoard{
    {0, 1, 0, 0}, {0, 1, 1, 1}, {1, 2, 2, 1}, {1, 2, 0, 2}};
static std::vector<std::vector<int>> singleRowBoard{{-1, 1, 0, 0}};

INSTANTIATE_TEST_SUITE_P(
    SameGameTest, GetClusterTests,
    ::testing::Values(std::make_tuple(symmetricalBoard, 2, Point{0, 0}),
                      std::make_tuple(symmetricalBoard, 5, Point{0, 1}),
                      std::make_tuple(symmetricalBoard, 1, Point{3, 2}),
                      std::make_tuple(symmetricalBoard, 3, Point{2, 1}),
                      std::make_tuple(singleRowBoard, 0, Point{0, 0}),
                      std::make_tuple(singleRowBoard, 1, Point{0, 1}),
                      std::make_tuple(singleRowBoard, 2, Point{0, 3})));
