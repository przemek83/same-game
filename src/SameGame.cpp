#include "SameGame.h"

#include <array>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <queue>

#include "Generator.h"

namespace
{
constexpr std::array<int, 4> cols{-1, 0, 0, 1};
constexpr std::array<int, 4> rows{0, -1, 1, 0};

constexpr char CHECKED{1};
constexpr char NOT_CHECKED{0};

constexpr Point emptyPoint{Point::NOT_SET, Point::NOT_SET};

void removeRows(Board& board, int column, int fromRow, int count)
{
    for (int row{fromRow}; row >= count; --row)
    {
        const int rowToChange{row - count};
        board.setColor({column, row}, board.getColor({column, rowToChange}));
        board.setEmpty({column, rowToChange});
    }
}

void impactColumn(Board& board, int column)
{
    const int NOT_SET{INT_MAX};
    int emptyStartIndex{NOT_SET};
    int emptyCount{0};
    for (int row{board.getRowCount() - 1}; row >= 0; --row)
    {
        if (board.getColor({column, row}) == Board::EMPTY)
        {
            if (emptyStartIndex == NOT_SET)
                emptyStartIndex = row;
            ++emptyCount;
            continue;
        }

        if (emptyStartIndex == NOT_SET)
            continue;

        removeRows(board, column, emptyStartIndex, emptyCount);

        emptyStartIndex = NOT_SET;
        row += emptyCount;
        emptyCount = 0;
    }
}

bool isFieldValid(const Board& board, int column, int row)
{
    return (column >= 0) && (column < board.getColumnCount()) && (row >= 0) &&
           (row < board.getRowCount());
}

std::vector<std::vector<char>> createCheckedVector(int columnCount,
                                                   int rowCount)
{
    std::vector<std::vector<char>> checked(columnCount);
    for (auto& column : checked)
        column.resize(rowCount, NOT_CHECKED);
    return checked;
}

Point findFirstCluster(const Board& board)
{
    std::vector<std::vector<char>> checked{
        createCheckedVector(board.getColumnCount(), board.getRowCount())};
    for (int row{board.getRowCount() - 1}; row >= 0; --row)
        for (int column{0}; column < board.getColumnCount(); ++column)
        {
            Point point{column, row};
            if (SameGame::getClusterSize(board, point, checked) > 1)
                return point;
        }
    return emptyPoint;
}

Point getRandomPoint(const Board& board, Generator& generator)
{
    return {generator.getInt(0, board.getColumnCount() - 1),
            generator.getInt(0, board.getRowCount()) - 1};
}

int getRandomTries(const Board& board)
{
    return static_cast<int>(board.getColumnCount() * board.getRowCount() * .4);
}

Point findBiggestCluster(const Board& board, Generator& generator)
{
    std::vector<std::vector<char>> checked{
        createCheckedVector(board.getColumnCount(), board.getRowCount())};
    int bestScore{1};
    Point bestPoint{emptyPoint};
    for (int i{0}; i < getRandomTries(board); ++i)
    {
        const Point currentPoint{getRandomPoint(board, generator)};
        const int score{SameGame::getClusterSize(board, currentPoint, checked)};
        if (score > bestScore)
        {
            bestScore = score;
            bestPoint = currentPoint;
        }
    }
    return bestScore > 0 ? bestPoint : emptyPoint;
}

char& getPosition(std::vector<std::vector<char>>& checked, Point point)
{
    return checked[static_cast<std::size_t>(point.column)]
                  [static_cast<std::size_t>(point.row)];
}
}  // namespace

namespace SameGame
{
int getClusterSize(const Board& board, Point startPoint,
                   std::vector<std::vector<char>>& checked)
{
    if (getPosition(checked, startPoint) == CHECKED)
        return 0;

    const int color{board.getColor(startPoint)};
    if (color == Board::EMPTY)
        return 0;

    int clusterSize{1};
    getPosition(checked, startPoint) = CHECKED;
    static std::queue<Point> pointsToCheck;
    pointsToCheck.push(startPoint);
    while (!pointsToCheck.empty())
    {
        const Point point{pointsToCheck.front()};
        pointsToCheck.pop();
        for (std::size_t k{0}; k < 4; ++k)
        {
            const int col{point.column + cols.at(k)};
            const int row{point.row + rows.at(k)};
            if (isFieldValid(board, col, row) &&
                (board.getColor({col, row}) == color) &&
                (getPosition(checked, {col, row}) == NOT_CHECKED))
            {
                getPosition(checked, {col, row}) = CHECKED;
                ++clusterSize;
                pointsToCheck.emplace(Point{col, row});
            }
        }
    }
    return clusterSize;
}

void impactGravity(Board& board, const std::set<int>& impactedColumns)
{
    for (int column : impactedColumns)
        impactColumn(board, column);
}

Point getNextMove(const Board& board, Generator& generator)
{
    Point nextPoint{findBiggestCluster(board, generator)};
    if (nextPoint == emptyPoint)
        nextPoint = findFirstCluster(board);
    return nextPoint;
}

std::set<int> makeMove(Board& board, Point point)
{
    const int color{board.getColor(point)};

    std::set<int> impactedColumns;
    std::queue<Point> pointsToCheck;
    pointsToCheck.push(point);
    board.setEmpty(point);
    while (!pointsToCheck.empty())
    {
        const auto currentPoint{pointsToCheck.front()};
        pointsToCheck.pop();
        impactedColumns.insert(currentPoint.column);
        for (std::size_t k{0}; k < 4; ++k)
        {
            const int col{currentPoint.column + cols.at(k)};
            const int row{currentPoint.row + rows.at(k)};
            if (isFieldValid(board, col, row) &&
                (board.getColor({col, row}) == color))
            {
                board.setEmpty({col, row});
                pointsToCheck.emplace(Point{col, row});
            }
        }
    }
    return impactedColumns;
}

std::vector<Point> playGame(Board board, Generator& generator)
{
    if ((board.getColumnCount() == 0) || (board.getRowCount() == 0))
        return {};

    std::vector<Point> points;
    while (true)
    {
        const Point nextPoint{getNextMove(board, generator)};
        if (nextPoint == emptyPoint)
            break;
        points.push_back(nextPoint);
        std::set<int> impactedColumns{makeMove(board, nextPoint)};
        impactGravity(board, impactedColumns);
    }
    return points;
}

};  // namespace SameGame
