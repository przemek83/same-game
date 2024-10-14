#include <cstring>

#include <gtest/gtest.h>

#include <src/Board.h>
#include <src/Point.h>
#include <src/SameGame.h>

#include "MockedGenerator.h"
#include "TestTools.h"

using BoardData = std::vector<std::vector<int>>;
static int EMPTY{Board::EMPTY};

class ImpactGravityTests
    : public ::testing::TestWithParam<
          std::tuple<BoardData, BoardData, std::vector<Point>>>
{
};

TEST_P(ImpactGravityTests, impactGravity)
{
    const BoardData& boardData{std::get<0>(GetParam())};
    const BoardData& expected{std::get<1>(GetParam())};
    const std::vector<Point>& impactedColumnsPoints{std::get<2>(GetParam())};

    std::set<int> impactedColumns;
    for (const auto point : impactedColumnsPoints)
        impactedColumns.insert(point.column);

    Board board{TestTools::createBoard(boardData)};
    MockedGenerator generator;
    SameGame game{board, generator};
    game.impactGravity(impactedColumns);
    EXPECT_EQ(board, TestTools::createBoard(expected));
}

INSTANTIATE_TEST_SUITE_P(
    SameGameTest, ImpactGravityTests,
    ::testing::Values(
        std::make_tuple(BoardData{{1, 3, 4, EMPTY, 5}},
                        BoardData{{EMPTY, 1, 3, 4, 5}},
                        std::vector<Point>{{0, 3}}),
        std::make_tuple(BoardData{{1, 3, EMPTY, EMPTY, EMPTY}},
                        BoardData{{EMPTY, EMPTY, EMPTY, 1, 3}},
                        std::vector<Point>{{0, 4}}),
        std::make_tuple(BoardData{{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                        BoardData{{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                        std::vector<Point>{{0, Point::NOT_SET}}),
        std::make_tuple(BoardData{{1, 2, 3, 4, 5}}, BoardData{{1, 2, 3, 4, 5}},
                        std::vector<Point>{{0, Point::NOT_SET}}),
        std::make_tuple(BoardData{{1, EMPTY, EMPTY, EMPTY, EMPTY},
                                  {1, 2, EMPTY, EMPTY, EMPTY},
                                  {1, EMPTY, EMPTY, 1, EMPTY},
                                  {1, EMPTY, EMPTY, EMPTY, 2},
                                  {EMPTY, EMPTY, EMPTY, 1, EMPTY},
                                  {1, EMPTY, 1, EMPTY, 1}},
                        BoardData{{EMPTY, EMPTY, EMPTY, EMPTY, 1},
                                  {EMPTY, EMPTY, EMPTY, 1, 2},
                                  {EMPTY, EMPTY, EMPTY, 1, 1},
                                  {EMPTY, EMPTY, EMPTY, 1, 2},
                                  {EMPTY, EMPTY, EMPTY, EMPTY, 1},
                                  {EMPTY, EMPTY, 1, 1, 1}},
                        std::vector<Point>{
                            {0, 4}, {1, 4}, {2, 4}, {3, 3}, {4, 4}, {5, 3}})));

TEST(SameGameTest, makeMove6x5)
{
    const BoardData expected{
        {EMPTY, EMPTY, EMPTY, EMPTY, 1},     {EMPTY, EMPTY, EMPTY, EMPTY, 2},
        {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}, {EMPTY, EMPTY, EMPTY, EMPTY, 2},
        {EMPTY, EMPTY, EMPTY, EMPTY, 1},     {EMPTY, EMPTY, 1, 1, 1}};
    BoardData boardData{
        {EMPTY, EMPTY, EMPTY, EMPTY, 1}, {EMPTY, EMPTY, EMPTY, 1, 2},
        {EMPTY, EMPTY, EMPTY, 1, 1},     {EMPTY, EMPTY, EMPTY, 1, 2},
        {EMPTY, EMPTY, EMPTY, EMPTY, 1}, {EMPTY, EMPTY, 1, 1, 1}};

    Board board{TestTools::createBoard(boardData)};
    MockedGenerator generator;
    SameGame game{board, generator};
    std::set<int> impactedColumns{game.makeMove({2, 3})};
    EXPECT_EQ(board, TestTools::createBoard(expected));
    EXPECT_EQ(impactedColumns, std::set<int>({1, 2, 3}));
}

TEST(SameGameTest, makeMove4x4)
{
    const BoardData boardData{
        {3, 1, 3, 3}, {3, 1, 1, 1}, {1, 2, 2, 1}, {1, 2, 3, 2}};
    const BoardData expected{{3, EMPTY, 3, 3},
                             {3, EMPTY, EMPTY, EMPTY},
                             {1, 2, 2, EMPTY},
                             {1, 2, 3, 2}};

    Board board{TestTools::createBoard(boardData)};
    MockedGenerator generator;
    SameGame game{board, generator};
    std::set<int> impactedColumns{game.makeMove({2, 3})};
    EXPECT_EQ(board, TestTools::createBoard(expected));
    EXPECT_EQ(impactedColumns, std::set<int>({0, 1, 2}));
}

class GetClusterTests
    : public ::testing::TestWithParam<std::tuple<BoardData, int, Point>>
{
};

TEST_P(GetClusterTests, GetCluster)
{
    const BoardData& boardData{std::get<0>(GetParam())};
    const int expectedClusterSize{std::get<1>(GetParam())};
    const Point point{std::get<2>(GetParam())};

    Board board{TestTools::createBoard(boardData)};
    std::vector<std::vector<char>> checked(board.getColumnCount());
    for (auto& column : checked)
        column.resize(board.getRowCount(), false);

    MockedGenerator generator;
    SameGame game{board, generator};
    int currentClusterSize{game.getClusterSize(point, checked)};
    EXPECT_EQ(currentClusterSize, expectedClusterSize);
}

static BoardData symmetricalBoard{
    {3, 1, 3, 3}, {3, 1, 1, 1}, {1, 2, 2, 1}, {1, 2, 3, 2}};
static BoardData singleRowBoard{{EMPTY, 1, 3, 3}};

INSTANTIATE_TEST_SUITE_P(
    SameGameTest, GetClusterTests,
    ::testing::Values(std::make_tuple(symmetricalBoard, 2, Point{0, 0}),
                      std::make_tuple(symmetricalBoard, 5, Point{0, 1}),
                      std::make_tuple(symmetricalBoard, 1, Point{3, 2}),
                      std::make_tuple(symmetricalBoard, 3, Point{2, 1}),
                      std::make_tuple(singleRowBoard, 0, Point{0, 0}),
                      std::make_tuple(singleRowBoard, 1, Point{0, 1}),
                      std::make_tuple(singleRowBoard, 2, Point{0, 3})));

static Board board50x50x3Colors{
    TestTools::prepareBoard(50, 50, "50x50_3_colors.txt")};
static Board board50x50x11Colors{
    TestTools::prepareBoard(50, 50, "50x50_11_colors.txt")};
static Board board200x200x3Colors{
    TestTools::prepareBoard(200, 200, "200x200_3_colors.txt")};
static Board board200x200x20Colors{
    TestTools::prepareBoard(200, 200, "200x200_20_colors.txt")};
static Board board500x500x20Colors{
    TestTools::prepareBoard(500, 500, "500x500_20_colors.txt")};

class Benchmark : public ::testing::TestWithParam<std::tuple<Board>>
{
};

// Test Name             | Release (ms) | Debug (ms)
// board50x50x3Colors    | 3            | 20
// board50x50x11Colors   | 10           | 52
// board200x200x3Colors  | 648          | 3025
// board200x200x20Colors | 2650         | 13076
// board500x500x20Colors | 121887       | 523070

INSTANTIATE_TEST_SUITE_P(SameGameTest, Benchmark,
                         ::testing::Values(board50x50x3Colors,
                                           board50x50x11Colors,
                                           board200x200x3Colors,
                                           board200x200x20Colors
                                           //,board500x500x20Colors
                                           ));

TEST_P(Benchmark, playGame)
{
    // GTEST_SKIP();
    Board board{std::get<0>(GetParam())};
    MockedGenerator generator;
    SameGame game{board, generator};
    game.playGame();
}
