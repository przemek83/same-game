#include "gtest/gtest.h"

#include <cstring>
#include <fstream>

#include "../SameGame.h"

using namespace SameGame;

class ImpaceGravityTests
    : public ::testing::TestWithParam<
          std::tuple<std::vector<std::vector<int>>,
                     std::vector<std::vector<int>>, std::vector<Point>>>
{
};

TEST_P(ImpaceGravityTests, impactGravity)
{
    std::vector<std::vector<int>> board{std::get<0>(GetParam())};
    const std::vector<std::vector<int>> expected{std::get<1>(GetParam())};
    const std::vector<Point> impactedColumnsPoints{std::get<2>(GetParam())};

    int impactedColumns[MAX_W];
    for (const auto point : impactedColumnsPoints)
        impactedColumns[point.column] = point.row;

    impactGravity(board, impactedColumns);
    EXPECT_EQ(board, expected);
}

INSTANTIATE_TEST_SUITE_P(
    SameGameTest, ImpaceGravityTests,
    ::testing::Values(
        std::make_tuple(std::vector<std::vector<int>>{{1, 3, 4, -1, 5}},
                        std::vector<std::vector<int>>{{-1, 1, 3, 4, 5}},
                        std::vector<Point>{{0, 3}}),
        std::make_tuple(std::vector<std::vector<int>>{{1, 3, -1, -1, -1}},
                        std::vector<std::vector<int>>{{-1, -1, -1, 1, 3}},
                        std::vector<Point>{{0, 4}}),
        std::make_tuple(std::vector<std::vector<int>>{{-1, -1, -1, -1, -1}},
                        std::vector<std::vector<int>>{{-1, -1, -1, -1, -1}},
                        std::vector<Point>{{0, EMPTY}}),
        std::make_tuple(std::vector<std::vector<int>>{{1, 2, 3, 4, 5}},
                        std::vector<std::vector<int>>{{1, 2, 3, 4, 5}},
                        std::vector<Point>{{0, EMPTY}}),
        std::make_tuple(std::vector<std::vector<int>>{{1, -1, -1, -1, -1},
                                                      {1, 2, -1, -1, -1},
                                                      {1, -1, -1, 1, -1},
                                                      {1, -1, -1, -1, 2},
                                                      {-1, -1, -1, 1, -1},
                                                      {1, -1, 1, -1, 1}},
                        std::vector<std::vector<int>>{{-1, -1, -1, -1, 1},
                                                      {-1, -1, -1, 1, 2},
                                                      {-1, -1, -1, 1, 1},
                                                      {-1, -1, -1, 1, 2},
                                                      {-1, -1, -1, -1, 1},
                                                      {-1, -1, 1, 1, 1}},
                        std::vector<Point>{
                            {0, 4}, {1, 4}, {2, 4}, {3, 3}, {4, 4}, {5, 3}})));

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

    std::vector<Point> checkedPoints{{1, 3}, {2, 3}, {2, 4}, {3, 3}};
    bool checked[MAX_W][MAX_H]{};
    for (const auto point : checkedPoints)
        checked[point.column][point.row] = true;

    makeMove(board, checked, impactedColumns, {2, 3});
    EXPECT_EQ(board, expected);

    for (const auto point : checkedPoints)
        EXPECT_FALSE(checked[point.column][point.row]);

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

    std::vector<Point> checkedPoints{{0, 1}, {1, 1}, {1, 2}, {1, 3}, {2, 3}};
    bool checked[MAX_W][MAX_H]{};
    for (const auto point : checkedPoints)
        checked[point.column][point.row] = true;

    int impactedColumns[MAX_W]{};
    std::memset(impactedColumns, EMPTY, sizeof(impactedColumns));

    makeMove(board, checked, impactedColumns, {2, 3});
    EXPECT_EQ(board, expected);

    for (const auto point : checkedPoints)
        EXPECT_FALSE(checked[point.column][point.row]);

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

TEST(SameGameTest, loadBoard)
{
    std::ifstream in("4x4_3_colors.txt", std::ifstream::in);
    std::vector<std::vector<int>> currentBoard{loadBoard(4, 4, in)};
    std::vector<std::vector<int>> expectedBoard{
        {0, 1, 0, 0}, {0, 1, 1, 1}, {1, 2, 2, 1}, {1, 2, 0, 2}};
    EXPECT_EQ(currentBoard, expectedBoard);
}

static std::vector<std::vector<int>> preapreBoard(std::string fileName)
{
    std::ifstream in(fileName, std::ifstream::in);
    return loadBoard(50, 50, in);
}

static std::vector<std::vector<int>> bigBoard{
    preapreBoard("50x50_11_colors.txt")};

TEST(SameGameTest, performanceTest)
{
    srand(0);
    playGame(50, 50, bigBoard);
}
