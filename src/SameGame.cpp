#include "SameGame.h"

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <queue>

#include "Generator.h"

SameGame::SameGame(Board& board, Generator& generator)
    : board_{board}, generator_{generator}
{
}

int SameGame::getClusterSize(Point startPoint, CheckedBoard& checked)
{
    if (getPosition(checked, startPoint) == CHECKED)
        return 0;

    const int color{board_.getColor(startPoint)};
    if (color == Board::EMPTY)
        return 0;

    int clusterSize{1};
    getPosition(checked, startPoint) = CHECKED;
    pointsToCheck_.push(startPoint);
    while (!pointsToCheck_.empty())
    {
        const Point point{pointsToCheck_.front()};
        pointsToCheck_.pop();
        const std::size_t caseCount{cols.size()};
        for (std::size_t k{0}; k < caseCount; ++k)
        {
            const int col{point.column_ + cols.at(k)};
            const int row{point.row_ + rows.at(k)};
            if (isFieldValid(col, row) &&
                (board_.getColor({col, row}) == color) &&
                (getPosition(checked, {col, row}) == NOT_CHECKED))
            {
                getPosition(checked, {col, row}) = CHECKED;
                ++clusterSize;
                pointsToCheck_.emplace(Point{col, row});
            }
        }
    }
    return clusterSize;
}

void SameGame::impactGravity(const std::set<int>& impactedColumns)
{
    for (int column : impactedColumns)
        impactColumn(column);
}

Point SameGame::getNextMove()
{
    Point nextPoint{findBiggestCluster()};
    if (nextPoint == emptyPoint)
        nextPoint = findFirstCluster();
    return nextPoint;
}

std::set<int> SameGame::makeMove(Point point)
{
    const int color{board_.getColor(point)};

    std::set<int> impactedColumns;
    std::queue<Point> pointsToCheck;
    pointsToCheck.push(point);
    board_.setEmpty(point);
    while (!pointsToCheck.empty())
    {
        const auto currentPoint{pointsToCheck.front()};
        pointsToCheck.pop();
        impactedColumns.insert(currentPoint.column_);
        const std::size_t caseCount{cols.size()};
        for (std::size_t k{0}; k < caseCount; ++k)
        {
            const int col{currentPoint.column_ + cols.at(k)};
            const int row{currentPoint.row_ + rows.at(k)};
            if (isFieldValid(col, row) &&
                (board_.getColor({col, row}) == color))
            {
                board_.setEmpty({col, row});
                pointsToCheck.emplace(Point{col, row});
            }
        }
    }
    return impactedColumns;
}

std::vector<Point> SameGame::playGame()
{
    std::vector<Point> points;
    while (true)
    {
        const Point nextPoint{getNextMove()};
        if (nextPoint == emptyPoint)
            break;
        points.push_back(nextPoint);
        std::set<int> impactedColumns{makeMove(nextPoint)};
        impactGravity(impactedColumns);
    }
    return points;
}

void SameGame::removeRows(int column, int fromRow, int count)
{
    for (int row{fromRow}; row >= count; --row)
    {
        const int rowToChange{row - count};
        board_.setColor({column, row}, board_.getColor({column, rowToChange}));
        board_.setEmpty({column, rowToChange});
    }
}

void SameGame::impactColumn(int column)
{
    const int NOT_SET{INT_MAX};
    int emptyStartIndex{NOT_SET};
    int emptyCount{0};

    int row{board_.getRowCount() - 1};
    while (row >= 0)
    {
        if (board_.getColor({column, row}) == Board::EMPTY)
        {
            if (emptyStartIndex == NOT_SET)
                emptyStartIndex = row;
            ++emptyCount;
        }
        else
        {
            if (emptyStartIndex != NOT_SET)
            {
                removeRows(column, emptyStartIndex, emptyCount);

                emptyStartIndex = NOT_SET;
                row += emptyCount;
                emptyCount = 0;
            }
        }
        --row;
    }
}

Point SameGame::findFirstCluster()
{
    CheckedBoard checked{
        createCheckedBoard(board_.getColumnCount(), board_.getRowCount())};
    for (int row{board_.getRowCount() - 1}; row >= 0; --row)
    {
        const int columnCount{board_.getColumnCount()};
        for (int column{0}; column < columnCount; ++column)
        {
            Point point{column, row};
            if (SameGame::getClusterSize(point, checked) > 1)
                return point;
        }
    }
    return emptyPoint;
}

Point SameGame::findBiggestCluster()
{
    CheckedBoard checked{
        createCheckedBoard(board_.getColumnCount(), board_.getRowCount())};
    int bestScore{1};
    Point bestPoint{emptyPoint};
    const int tries{getTries()};
    for (int i{0}; i < tries; ++i)
    {
        const Point point{getRandomPoint()};
        const int score{getClusterSize(point, checked)};
        if (score > bestScore)
        {
            bestScore = score;
            bestPoint = point;
        }
    }

    return bestPoint;
}

bool SameGame::isFieldValid(int column, int row) const
{
    return (column >= 0) && (column < board_.getColumnCount()) && (row >= 0) &&
           (row < board_.getRowCount());
}

SameGame::CheckedBoard SameGame::createCheckedBoard(int columnCount,
                                                    int rowCount) const
{
    CheckedBoard checked{static_cast<std::size_t>(columnCount)};
    for (auto& column : checked)
        column.resize(static_cast<std::size_t>(rowCount), NOT_CHECKED);
    return checked;
}

Point SameGame::getRandomPoint() const
{
    return {generator_.getInt(0, board_.getColumnCount() - 1),
            generator_.getInt(0, board_.getRowCount() - 1)};
}

int SameGame::getTries() const
{
    return static_cast<int>(board_.getColumnCount() * board_.getRowCount() *
                            .4);
}

unsigned char& SameGame::getPosition(CheckedBoard& checked, Point point)
{
    return checked[static_cast<std::size_t>(point.column_)]
                  [static_cast<std::size_t>(point.row_)];
}
