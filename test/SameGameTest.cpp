#include <cstring>

#include <gtest/gtest.h>

#include <src/Board.h>
#include <src/Point.h>
#include <src/SameGame.h>

#include "MockedGenerator.h"
#include "TestTools.h"

using BoardData = std::vector<std::vector<int>>;

namespace
{
constexpr int _{Board::EMPTY};
};  // namespace

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
        impactedColumns.insert(point.column_);

    Board board{TestTools::createBoard(boardData)};
    MockedGenerator generator;
    SameGame game{board, generator};
    game.impactGravity(impactedColumns);
    EXPECT_EQ(board, TestTools::createBoard(expected));
}

INSTANTIATE_TEST_SUITE_P(
    SameGameTest, ImpactGravityTests,
    ::testing::Values(
        std::make_tuple(BoardData{{1, 3, 4, _, 5}}, BoardData{{_, 1, 3, 4, 5}},
                        std::vector<Point>{{0, 3}}),
        std::make_tuple(BoardData{{1, 3, _, _, _}}, BoardData{{_, _, _, 1, 3}},
                        std::vector<Point>{{0, 4}}),
        std::make_tuple(BoardData{{_, _, _, _, _}}, BoardData{{_, _, _, _, _}},
                        std::vector<Point>{{0, Point::NOT_SET}}),
        std::make_tuple(BoardData{{1, 2, 3, 4, 5}}, BoardData{{1, 2, 3, 4, 5}},
                        std::vector<Point>{{0, Point::NOT_SET}}),
        std::make_tuple(BoardData{{1, _, _, _, _},
                                  {1, 2, _, _, _},
                                  {1, _, _, 1, _},
                                  {1, _, _, _, 2},
                                  {_, _, _, 1, _},
                                  {1, _, 1, _, 1}},
                        BoardData{{_, _, _, _, 1},
                                  {_, _, _, 1, 2},
                                  {_, _, _, 1, 1},
                                  {_, _, _, 1, 2},
                                  {_, _, _, _, 1},
                                  {_, _, 1, 1, 1}},
                        std::vector<Point>{
                            {0, 4}, {1, 4}, {2, 4}, {3, 3}, {4, 4}, {5, 3}})));

TEST(SameGameTest, makeMove6x5)
{
    const BoardData expected{{_, _, _, _, 1}, {_, _, _, _, 2}, {_, _, _, _, _},
                             {_, _, _, _, 2}, {_, _, _, _, 1}, {_, _, 1, 1, 1}};
    BoardData boardData{{_, _, _, _, 1}, {_, _, _, 1, 2}, {_, _, _, 1, 1},
                        {_, _, _, 1, 2}, {_, _, _, _, 1}, {_, _, 1, 1, 1}};

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
    const BoardData expected{
        {3, _, 3, 3}, {3, _, _, _}, {1, 2, 2, _}, {1, 2, 3, 2}};

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
    SameGame::CheckedBoard checked(
        static_cast<std::size_t>(board.getColumnCount()));
    for (auto& column : checked)
        column.resize(static_cast<std::size_t>(board.getRowCount()), false);

    MockedGenerator generator;
    SameGame game{board, generator};
    int currentClusterSize{game.getClusterSize(point, checked)};
    EXPECT_EQ(currentClusterSize, expectedClusterSize);
}

namespace
{
const BoardData symmetricalBoard{
    {3, 1, 3, 3}, {3, 1, 1, 1}, {1, 2, 2, 1}, {1, 2, 3, 2}};
const BoardData singleRowBoard{{_, 1, 3, 3}};
};  // namespace

INSTANTIATE_TEST_SUITE_P(
    SameGameTest, GetClusterTests,
    ::testing::Values(std::make_tuple(symmetricalBoard, 2, Point{0, 0}),
                      std::make_tuple(symmetricalBoard, 5, Point{0, 1}),
                      std::make_tuple(symmetricalBoard, 1, Point{3, 2}),
                      std::make_tuple(symmetricalBoard, 3, Point{2, 1}),
                      std::make_tuple(singleRowBoard, 0, Point{0, 0}),
                      std::make_tuple(singleRowBoard, 1, Point{0, 1}),
                      std::make_tuple(singleRowBoard, 2, Point{0, 3})));

TEST(SameGameTest, playWith4x4SymetricalBoard)
{
    MockedGenerator generator;
    Board board{TestTools::createBoard(symmetricalBoard)};
    SameGame game{board, generator};
    std::vector<Point> points{game.playGame()};
    EXPECT_EQ(points.size(), 4);

    std::vector<Point> expected{{1, 0}, {0, 3}, {0, 3}, {2, 3}};
    const std::size_t size{points.size()};
    for (std::size_t i{0}; i < size; ++i)
        EXPECT_EQ(points[i], expected[i]);
}

TEST(SameGameTest, playWith3x1AsymmetricalBoard)
{
    MockedGenerator generator;
    Board board{TestTools::createBoard(singleRowBoard)};
    SameGame game{board, generator};
    std::vector<Point> points{game.playGame()};
    EXPECT_EQ(points.size(), 1);
    Point expected{0, 2};
    EXPECT_EQ(points[0], expected);
}

namespace
{
Board getBoard50x50x3Colors()
{
    return TestTools::prepareBoard(50, 50, "50x50_3_colors.txt");
}

Board getBoard50x50x11Colors()
{
    return TestTools::prepareBoard(50, 50, "50x50_11_colors.txt");
}

Board getBoard200x200x3Colors()
{
    return TestTools::prepareBoard(200, 200, "200x200_3_colors.txt");
}

Board getBoard200x200x20Colors()
{
    return TestTools::prepareBoard(200, 200, "200x200_20_colors.txt");
}

Board getBoard500x500x20Colors()
{
    return TestTools::prepareBoard(500, 500, "500x500_20_colors.txt");
}
};  // namespace

class Benchmark : public ::testing::TestWithParam<std::tuple<Board>>
{
};

// Test Name             | Release (ms) | Debug (ms)
// board50x50x3Colors    | 3            | 21
// board50x50x11Colors   | 10           | 51
// board200x200x3Colors  | 433          | 1888
// board200x200x20Colors | 1476         | 6157
// board500x500x20Colors | 110270       | 352902

INSTANTIATE_TEST_SUITE_P(SameGameTest, Benchmark,
                         ::testing::Values(getBoard50x50x3Colors(),
                                           getBoard50x50x11Colors(),
                                           getBoard200x200x3Colors(),
                                           getBoard200x200x20Colors()
                                           //, getBoard500x500x20Colors()
                                           ));

TEST_P(Benchmark, playGame)
{
    // GTEST_SKIP();
    Board board{std::get<0>(GetParam())};
    MockedGenerator generator;
    SameGame game{board, generator};
    game.playGame();
}
