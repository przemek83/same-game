#include "gtest/gtest.h"

#include <cstring>
#include <fstream>

#include "../Board.h"
#include "../Point.h"
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

    std::vector<int> impactedColumns(board.size());
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
                        std::vector<Point>{{0, Point::EMPTY}}),
        std::make_tuple(std::vector<std::vector<int>>{{1, 2, 3, 4, 5}},
                        std::vector<std::vector<int>>{{1, 2, 3, 4, 5}},
                        std::vector<Point>{{0, Point::EMPTY}}),
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
    std::vector<int> impactedColumns(board.size(), Point::EMPTY);

    makeMove(board, impactedColumns, {2, 3});
    EXPECT_EQ(board, expected);

    EXPECT_EQ(impactedColumns[0], Point::EMPTY);
    EXPECT_EQ(impactedColumns[1], 3);
    EXPECT_EQ(impactedColumns[2], 4);
    EXPECT_EQ(impactedColumns[3], 3);
    EXPECT_EQ(impactedColumns[4], Point::EMPTY);
}

TEST(SameGameTest, makeMove4x4)
{
    std::vector<std::vector<int>> board{
        {0, 1, 0, 0}, {0, 1, 1, 1}, {1, 2, 2, 1}, {1, 2, 0, 2}};
    const std::vector<std::vector<int>> expected{
        {0, -1, 0, 0}, {0, -1, -1, -1}, {1, 2, 2, -1}, {1, 2, 0, 2}};

    std::vector<int> impactedColumns(board.size(), Point::EMPTY);

    makeMove(board, impactedColumns, {2, 3});
    EXPECT_EQ(board, expected);

    EXPECT_EQ(impactedColumns[0], 1);
    EXPECT_EQ(impactedColumns[1], 3);
    EXPECT_EQ(impactedColumns[2], 3);
    EXPECT_EQ(impactedColumns[3], Point::EMPTY);
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

    bool checked[Board::MAX_W][Board::MAX_H]{};
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

static std::vector<std::vector<int>> preapreBoard(unsigned int columnsCount,
                                                  unsigned int rowsCount,
                                                  std::string fileName)
{
    std::ifstream in(fileName, std::ifstream::in);
    return Board::loadBoard(columnsCount, rowsCount, in);
}

TEST(SameGameTest, loadBoard)
{
    std::vector<std::vector<int>> currentBoard{
        preapreBoard(4, 4, "4x4_3_colors.txt")};
    std::vector<std::vector<int>> expectedBoard{
        {0, 1, 0, 0}, {0, 1, 1, 1}, {1, 2, 2, 1}, {1, 2, 0, 2}};
    EXPECT_EQ(currentBoard, expectedBoard);
}

static std::vector<std::vector<int>> board50x50x11Colors{
    preapreBoard(50, 50, "50x50_11_colors.txt")};
static std::vector<std::vector<int>> board200x200x3Colors{
    preapreBoard(200, 200, "200x200_3_colors.txt")};
static std::vector<std::vector<int>> board200x200x20Colors{
    preapreBoard(200, 200, "200x200_20_colors.txt")};
static std::vector<std::vector<int>> board500x500x20Colors{
    preapreBoard(500, 500, "500x500_20_colors.txt")};

class PerformanceTests
    : public ::testing::TestWithParam<std::tuple<std::vector<std::vector<int>>>>
{
};

// Test Name             | Release (ms) | Debug (ms)
// board50x50x11Colors   | 23           | 79
// board200x200x3Colors  | 1268         | 4739
// board200x200x20Colors | 4875         | 18070
// board500x500x20Colors | 236104       | 816494

INSTANTIATE_TEST_SUITE_P(
    SameGameTest, PerformanceTests,
    ::testing::Values(board50x50x11Colors,   // 23 | 79 ms
                      board200x200x3Colors,  // 1268 | 4739 ms
                      board200x200x20Colors  // 4875 || 18070 ms
                      //,board500x500x20Colors  // 816494 ms
                      ));

TEST_P(PerformanceTests, playGame)
{
    const std::vector<std::vector<int>> board{std::get<0>(GetParam())};
    srand(1);
    playGame(board);
}
