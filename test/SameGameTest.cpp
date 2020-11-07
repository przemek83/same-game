#include "gtest/gtest.h"

#include <cstring>

#include "../Board.h"
#include "../Point.h"
#include "../SameGame.h"
#include "TestTools.h"

using namespace SameGame;

class ImpactGravityTests
    : public ::testing::TestWithParam<std::tuple<
          std::vector<std::vector<unsigned int>>,
          std::vector<std::vector<unsigned int>>, std::vector<Point>>>
{
};

TEST_P(ImpactGravityTests, impactGravity)
{
    std::vector<std::vector<unsigned int>> boardData{std::get<0>(GetParam())};
    const std::vector<std::vector<unsigned int>> expected{
        std::get<1>(GetParam())};
    const std::vector<Point> impactedColumnsPoints{std::get<2>(GetParam())};

    std::set<unsigned int> impactedColumns;
    for (const auto point : impactedColumnsPoints)
        impactedColumns.insert(point.column);

    Board board{TestTools::createBoard(boardData)};
    impactGravity(board, impactedColumns);
    EXPECT_EQ(board, TestTools::createBoard(expected));
}

INSTANTIATE_TEST_SUITE_P(
    SameGameTest, ImpactGravityTests,
    ::testing::Values(
        std::make_tuple(
            std::vector<std::vector<unsigned int>>{{1, 3, 4, Board::EMPTY, 5}},
            std::vector<std::vector<unsigned int>>{{Board::EMPTY, 1, 3, 4, 5}},
            std::vector<Point>{{0, 3}}),
        std::make_tuple(
            std::vector<std::vector<unsigned int>>{
                {1, 3, Board::EMPTY, Board::EMPTY, Board::EMPTY}},
            std::vector<std::vector<unsigned int>>{
                {Board::EMPTY, Board::EMPTY, Board::EMPTY, 1, 3}},
            std::vector<Point>{{0, 4}}),
        std::make_tuple(
            std::vector<std::vector<unsigned int>>{{Board::EMPTY, Board::EMPTY,
                                                    Board::EMPTY, Board::EMPTY,
                                                    Board::EMPTY}},
            std::vector<std::vector<unsigned int>>{{Board::EMPTY, Board::EMPTY,
                                                    Board::EMPTY, Board::EMPTY,
                                                    Board::EMPTY}},
            std::vector<Point>{{0, Point::NOT_SET}}),
        std::make_tuple(std::vector<std::vector<unsigned int>>{{1, 2, 3, 4, 5}},
                        std::vector<std::vector<unsigned int>>{{1, 2, 3, 4, 5}},
                        std::vector<Point>{{0, Point::NOT_SET}}),
        std::make_tuple(
            std::vector<std::vector<unsigned int>>{
                {1, Board::EMPTY, Board::EMPTY, Board::EMPTY, Board::EMPTY},
                {1, 2, Board::EMPTY, Board::EMPTY, Board::EMPTY},
                {1, Board::EMPTY, Board::EMPTY, 1, Board::EMPTY},
                {1, Board::EMPTY, Board::EMPTY, Board::EMPTY, 2},
                {Board::EMPTY, Board::EMPTY, Board::EMPTY, 1, Board::EMPTY},
                {1, Board::EMPTY, 1, Board::EMPTY, 1}},
            std::vector<std::vector<unsigned int>>{
                {Board::EMPTY, Board::EMPTY, Board::EMPTY, Board::EMPTY, 1},
                {Board::EMPTY, Board::EMPTY, Board::EMPTY, 1, 2},
                {Board::EMPTY, Board::EMPTY, Board::EMPTY, 1, 1},
                {Board::EMPTY, Board::EMPTY, Board::EMPTY, 1, 2},
                {Board::EMPTY, Board::EMPTY, Board::EMPTY, Board::EMPTY, 1},
                {Board::EMPTY, Board::EMPTY, 1, 1, 1}},
            std::vector<Point>{
                {0, 4}, {1, 4}, {2, 4}, {3, 3}, {4, 4}, {5, 3}})));

TEST(SameGameTest, makeMove6x5)
{
    const std::vector<std::vector<unsigned int>> expected{
        {Board::EMPTY, Board::EMPTY, Board::EMPTY, Board::EMPTY, 1},
        {Board::EMPTY, Board::EMPTY, Board::EMPTY, Board::EMPTY, 2},
        {Board::EMPTY, Board::EMPTY, Board::EMPTY, Board::EMPTY, Board::EMPTY},
        {Board::EMPTY, Board::EMPTY, Board::EMPTY, Board::EMPTY, 2},
        {Board::EMPTY, Board::EMPTY, Board::EMPTY, Board::EMPTY, 1},
        {Board::EMPTY, Board::EMPTY, 1, 1, 1}};
    std::vector<std::vector<unsigned int>> boardData{
        {Board::EMPTY, Board::EMPTY, Board::EMPTY, Board::EMPTY, 1},
        {Board::EMPTY, Board::EMPTY, Board::EMPTY, 1, 2},
        {Board::EMPTY, Board::EMPTY, Board::EMPTY, 1, 1},
        {Board::EMPTY, Board::EMPTY, Board::EMPTY, 1, 2},
        {Board::EMPTY, Board::EMPTY, Board::EMPTY, Board::EMPTY, 1},
        {Board::EMPTY, Board::EMPTY, 1, 1, 1}};

    Board board{TestTools::createBoard(boardData)};
    std::set<unsigned int> impactedColumns{makeMove(board, {2, 3})};
    EXPECT_EQ(board, TestTools::createBoard(expected));
    EXPECT_EQ(impactedColumns, std::set<unsigned int>({1, 2, 3}));
}

TEST(SameGameTest, makeMove4x4)
{
    std::vector<std::vector<unsigned int>> boardData{
        {3, 1, 3, 3}, {3, 1, 1, 1}, {1, 2, 2, 1}, {1, 2, 3, 2}};
    const std::vector<std::vector<unsigned int>> expected{
        {3, Board::EMPTY, 3, 3},
        {3, Board::EMPTY, Board::EMPTY, Board::EMPTY},
        {1, 2, 2, Board::EMPTY},
        {1, 2, 3, 2}};

    Board board{TestTools::createBoard(boardData)};
    std::set<unsigned int> impactedColumns{makeMove(board, {2, 3})};
    EXPECT_EQ(board, TestTools::createBoard(expected));
    EXPECT_EQ(impactedColumns, std::set<unsigned int>({0, 1, 2}));
}

class GetClusterTests
    : public ::testing::TestWithParam<
          std::tuple<std::vector<std::vector<unsigned int>>, int, Point>>
{
};

TEST_P(GetClusterTests, GetCluster)
{
    const std::vector<std::vector<unsigned int>> boardData{
        std::get<0>(GetParam())};
    const int expectedClusterSize{std::get<1>(GetParam())};
    const Point point{std::get<2>(GetParam())};

    Board board{TestTools::createBoard(boardData)};
    std::vector<std::vector<bool>> checked(board.getColumnCount());
    for (auto& column : checked)
        column.resize(board.getRowCount(), false);
    unsigned int currentClusterSize{getClusterSize(board, point, checked)};
    EXPECT_EQ(currentClusterSize, expectedClusterSize);
}

static std::vector<std::vector<unsigned int>> symmetricalBoard{
    {3, 1, 3, 3}, {3, 1, 1, 1}, {1, 2, 2, 1}, {1, 2, 3, 2}};
static std::vector<std::vector<unsigned int>> singleRowBoard{
    {Board::EMPTY, 1, 3, 3}};

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

class PerformanceTests : public ::testing::TestWithParam<std::tuple<Board>>
{
};

// Test Name             | Release (ms) | Debug (ms)
// board50x50x3Colors    | 8            | 53
// board50x50x11Colors   | 19           | 133
// board200x200x3Colors  | 1240         | 8286
// board200x200x20Colors | 3948         | 26148
// board500x500x20Colors | 197906       | 1173411

INSTANTIATE_TEST_SUITE_P(SameGameTest, PerformanceTests,
                         ::testing::Values(board50x50x3Colors,
                                           board50x50x11Colors,
                                           board200x200x3Colors,
                                           board200x200x20Colors
                                           //,board500x500x20Colors
                                           ));

TEST_P(PerformanceTests, playGame)
{
    const Board board{std::get<0>(GetParam())};
    srand(1);
    playGame(board);
}
