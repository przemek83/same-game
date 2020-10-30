#include "gtest/gtest.h"

#include <cstring>
#include <fstream>

#include "../Board.h"
#include "../Point.h"
#include "../SameGame.h"

using namespace SameGame;

static Board createBoard(const std::vector<std::vector<int>>& data)
{
    const unsigned int columnCount{static_cast<unsigned int>(data.size())};
    const unsigned int rowCount{static_cast<unsigned int>(data[0].size())};

    std::istringstream stringStream;
    Board board(columnCount, rowCount, stringStream);
    for (unsigned int row = 0; row < rowCount; ++row)
        for (unsigned int column = 0; column < columnCount; ++column)
            board.setColor(column, row, data[column][row]);
    return board;
}

class ImpactGravityTests
    : public ::testing::TestWithParam<
          std::tuple<std::vector<std::vector<int>>,
                     std::vector<std::vector<int>>, std::vector<Point>>>
{
};

TEST_P(ImpactGravityTests, impactGravity)
{
    std::vector<std::vector<int>> boardData{std::get<0>(GetParam())};
    const std::vector<std::vector<int>> expected{std::get<1>(GetParam())};
    const std::vector<Point> impactedColumnsPoints{std::get<2>(GetParam())};

    std::set<int> impactedColumns;
    for (const auto point : impactedColumnsPoints)
        impactedColumns.insert(point.column);

    Board board{createBoard(boardData)};
    impactGravity(board, impactedColumns);
    EXPECT_EQ(board, createBoard(expected));
}

INSTANTIATE_TEST_SUITE_P(
    SameGameTest, ImpactGravityTests,
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
    std::vector<std::vector<int>> boardData{
        {-1, -1, -1, -1, 1}, {-1, -1, -1, 1, 2},  {-1, -1, -1, 1, 1},
        {-1, -1, -1, 1, 2},  {-1, -1, -1, -1, 1}, {-1, -1, 1, 1, 1}};

    Board board{createBoard(boardData)};
    std::set<int> impactedColumns{makeMove(board, {2, 3})};
    EXPECT_EQ(board, createBoard(expected));
    EXPECT_EQ(impactedColumns, std::set<int>({1, 2, 3}));
}

TEST(SameGameTest, makeMove4x4)
{
    std::vector<std::vector<int>> boardData{
        {0, 1, 0, 0}, {0, 1, 1, 1}, {1, 2, 2, 1}, {1, 2, 0, 2}};
    const std::vector<std::vector<int>> expected{
        {0, -1, 0, 0}, {0, -1, -1, -1}, {1, 2, 2, -1}, {1, 2, 0, 2}};

    Board board{createBoard(boardData)};
    std::set<int> impactedColumns{makeMove(board, {2, 3})};
    EXPECT_EQ(board, createBoard(expected));
    EXPECT_EQ(impactedColumns, std::set<int>({0, 1, 2}));
}

class GetClusterTests
    : public ::testing::TestWithParam<
          std::tuple<std::vector<std::vector<int>>, int, Point>>
{
};

TEST_P(GetClusterTests, GetCluster)
{
    const std::vector<std::vector<int>> boardData{std::get<0>(GetParam())};
    const int expectedClusterSize{std::get<1>(GetParam())};
    const Point point{std::get<2>(GetParam())};

    bool checked[Board::MAX_W][Board::MAX_H]{};
    std::memset(checked, false, sizeof(checked));
    Board board{createBoard(boardData)};
    unsigned int currentClusterSize{getClusterSize(board, point, checked)};
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

static Board prepareBoard(unsigned int columnsCount, unsigned int rowsCount,
                          std::string fileName)
{
    std::ifstream in(fileName, std::ifstream::in);
    return Board(columnsCount, rowsCount, in);
}

TEST(SameGameTest, loadBoard)
{
    Board currentBoard{prepareBoard(4, 4, "4x4_3_colors.txt")};
    std::vector<std::vector<int>> expectedBoard{
        {0, 1, 0, 0}, {0, 1, 1, 1}, {1, 2, 2, 1}, {1, 2, 0, 2}};
    EXPECT_EQ(currentBoard, createBoard(expectedBoard));
}

static Board board50x50x3Colors{prepareBoard(50, 50, "50x50_3_colors.txt")};
static Board board50x50x11Colors{prepareBoard(50, 50, "50x50_11_colors.txt")};
static Board board200x200x3Colors{
    prepareBoard(200, 200, "200x200_3_colors.txt")};
static Board board200x200x20Colors{
    prepareBoard(200, 200, "200x200_20_colors.txt")};
static Board board500x500x20Colors{
    prepareBoard(500, 500, "500x500_20_colors.txt")};

class PerformanceTests : public ::testing::TestWithParam<std::tuple<Board>>
{
};

// Test Name             | Release (ms) | Debug (ms)
// board50x50x3Colors    | 8            | 27
// board50x50x11Colors   | 23           | 79
// board200x200x3Colors  | 1268         | 4739
// board200x200x20Colors | 4875         | 18070
// board500x500x20Colors | 236104       | 816494

INSTANTIATE_TEST_SUITE_P(SameGameTest, PerformanceTests,
                         ::testing::Values(board50x50x3Colors,
                                           board50x50x11Colors,
                                           board200x200x3Colors,
                                           board200x200x20Colors
                                           // ,board500x500x20Colors
                                           ));

TEST_P(PerformanceTests, playGame)
{
    const Board board{std::get<0>(GetParam())};
    srand(1);
    playGame(board);
}
