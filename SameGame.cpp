#include "SameGame.h"

#include <array>
#include <cstdlib>
#include <cstring>
#include <queue>

namespace SameGame
{
constexpr std::array<int, 4> cols{-1, 0, 0, 1};
constexpr std::array<int, 4> rows{0, -1, 1, 0};

constexpr Point emptyPoint{Point::NOT_SET, Point::NOT_SET};

static void removeRows(Board& board, unsigned int column, unsigned int fromRow,
                       unsigned int count)
{
    for (unsigned int row{fromRow + 1}; row-- > count;)
    {
        const unsigned int rowToChange{row - count};
        board.setColor({column, row}, board.getColor({column, rowToChange}));
        board.setEmpty({column, rowToChange});
    }
}

static void impactColumn(Board& board, unsigned int column)
{
    const unsigned int NOT_SET{UINT_MAX};
    unsigned int emptyStartIndex{NOT_SET};
    unsigned int emptyCount{0};
    for (unsigned int row{board.getRowCount()}; row-- > 0;)
    {
        if (board.getColor({column, row}) == Board::EMPTY)
        {
            if (emptyStartIndex == NOT_SET)
                emptyStartIndex = row;
            emptyCount++;
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

void impactGravity(Board& board, const std::set<unsigned int>& impactedColumns)
{
    for (unsigned int column : impactedColumns)
        impactColumn(board, column);
}

static bool isFieldValid(const Board& board, unsigned int column,
                         unsigned int row)
{
    return column < board.getColumnCount() && row < board.getRowCount();
}

unsigned int getClusterSize(const Board& board, Point startPoint,
                            std::vector<std::vector<bool>>& checked)
{
    if (checked[startPoint.column][startPoint.row])
        return 0;

    const unsigned int color{board.getColor(startPoint)};
    if (color == Board::EMPTY)
        return 0;

    unsigned int clusterSize{1};
    checked[startPoint.column][startPoint.row] = true;
    static std::queue<Point> pointsToCheck;
    pointsToCheck.push(startPoint);
    while (!pointsToCheck.empty())
    {
        const Point point{pointsToCheck.front()};
        pointsToCheck.pop();
        for (unsigned int k{0}; k < 4; ++k)
        {
            const unsigned int col{point.column + cols.at(k)};
            const unsigned int row{point.row + rows.at(k)};
            if (isFieldValid(board, col, row) &&
                board.getColor({col, row}) == color && !checked[col][row])
            {
                checked[col][row] = true;
                clusterSize++;
                pointsToCheck.emplace(Point{col, row});
            }
        }
    }
    return clusterSize;
}

// Following
// https://stackoverflow.com/questions/1640258/need-a-fast-random-generator-for-c
inline unsigned int fastRandInt()
{
    static unsigned int g_seed{static_cast<unsigned int>(rand())};
    g_seed = (214013 * g_seed + 2531011);
    return (g_seed >> 16) & 0x7FFF;
}
static std::vector<std::vector<bool>> createCheckedVector(
    unsigned int columnCount, unsigned int rowCount)
{
    std::vector<std::vector<bool>> checked(columnCount);
    for (auto& column : checked)
        column.resize(rowCount, false);
    return checked;
}

static Point findFirstCluster(const Board& board)
{
    std::vector<std::vector<bool>> checked{
        createCheckedVector(board.getColumnCount(), board.getRowCount())};
    for (unsigned int row{board.getRowCount()}; row-- > 0;)
        for (unsigned int column{0}; column < board.getColumnCount(); ++column)
        {
            Point point{column, row};
            if (getClusterSize(board, point, checked) > 1)
                return point;
        }
    return emptyPoint;
}

static Point getRandomPoint(const Board& board)
{
    return {fastRandInt() % board.getColumnCount(),
            fastRandInt() % board.getRowCount()};
}

static unsigned int getRandomTries(const Board& board)
{
    return static_cast<unsigned int>(board.getColumnCount() *
                                     board.getRowCount() * .4);
}

static Point findBiggestCluster(const Board& board)
{
    std::vector<std::vector<bool>> checked{
        createCheckedVector(board.getColumnCount(), board.getRowCount())};
    unsigned int bestScore{1};
    Point bestPoint{emptyPoint};
    for (unsigned int i{0}; i < getRandomTries(board); ++i)
    {
        const Point currentPoint{getRandomPoint(board)};
        const unsigned int score{getClusterSize(board, currentPoint, checked)};
        if (score > bestScore)
        {
            bestScore = score;
            bestPoint = currentPoint;
        }
    }
    return bestScore > 0 ? bestPoint : emptyPoint;
}

Point getNextMove(const Board& board)
{
    Point nextPoint{findBiggestCluster(board)};
    if (nextPoint == emptyPoint)
        nextPoint = findFirstCluster(board);
    return nextPoint;
}

std::set<unsigned int> makeMove(Board& board, Point startPoint)
{
    const unsigned int color{board.getColor(startPoint)};

    std::set<unsigned int> impactedColumns;
    std::queue<Point> pointsToCheck;
    pointsToCheck.push(startPoint);
    board.setEmpty(startPoint);
    while (!pointsToCheck.empty())
    {
        const auto currentPoint{pointsToCheck.front()};
        pointsToCheck.pop();
        impactedColumns.insert(currentPoint.column);
        for (unsigned int k{0}; k < 4; ++k)
        {
            const unsigned int col{currentPoint.column + cols.at(k)};
            const unsigned int row{currentPoint.row + rows.at(k)};
            if (isFieldValid(board, col, row) &&
                board.getColor({col, row}) == color)
            {
                board.setEmpty({col, row});
                pointsToCheck.emplace(Point{col, row});
            }
        }
    }
    return impactedColumns;
}

std::vector<Point> playGame(Board board)
{
    if (board.getColumnCount() == 0 || board.getRowCount() == 0)
        return {};

    std::vector<Point> points;
    while (true)
    {
        const Point nextPoint{getNextMove(board)};
        if (nextPoint == emptyPoint)
            break;
        points.push_back(nextPoint);
        std::set<unsigned int> impactedColumns{makeMove(board, nextPoint)};
        impactGravity(board, impactedColumns);
    }
    return points;
}

};  // namespace SameGame
