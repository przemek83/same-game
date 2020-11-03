#include "SameGame.h"

#include <cstdlib>
#include <cstring>
#include <queue>

namespace SameGame
{
constexpr int cols[]{-1, 0, 0, 1};
constexpr int rows[]{0, -1, 1, 0};

constexpr Point emptyPoint{Point::EMPTY, Point::EMPTY};

void impactGravity(Board& board, std::set<int> impactedColumns)
{
    for (int impactedColumn : impactedColumns)
    {
        int emptyStartIndex = {Point::EMPTY};
        int emptyCount{0};

        for (int row = board.getRowCount(); row >= 0; --row)
        {
            if (board.getColor({impactedColumn, row}) == Point::EMPTY)
            {
                if (emptyStartIndex == Point::EMPTY)
                    emptyStartIndex = row;
                emptyCount++;
            }
            else
            {
                if (emptyStartIndex == Point::EMPTY)
                    continue;

                for (int currentRow = emptyStartIndex; currentRow >= emptyCount;
                     --currentRow)
                {
                    board.setColor({impactedColumn, currentRow},
                                   board.getColor({impactedColumn,
                                                   currentRow - emptyCount}));
                    board.setEmpty({impactedColumn, currentRow - emptyCount});
                }
                emptyStartIndex = Point::EMPTY;
                row += emptyCount;
                emptyCount = 0;
            }
        }
    }
}

bool isFieldValid(const Board& board, unsigned int column, unsigned int row,
                  int color)
{
    return column >= 0 && column < board.getColumnCount() && row >= 0 &&
           row < board.getRowCount() &&
           board.getColor({static_cast<int>(column), static_cast<int>(row)}) ==
               color;
}

unsigned int getClusterSize(const Board& board, Point startPoint,
                            std::vector<std::vector<bool>>& checked)
{
    int color{board.getColor(startPoint)};
    if (color == Point::EMPTY)
        return 0;

    unsigned int clusterSize{0};
    static std::queue<Point> toCheck;
    toCheck.push(startPoint);
    while (!toCheck.empty())
    {
        const auto point{toCheck.front()};
        toCheck.pop();
        if (board.getColor(point) != color)
            continue;

        if (!checked[point.column][point.row])
        {
            clusterSize++;
            checked[point.column][point.row] = true;
        }

        for (int k = 0; k < 4; ++k)
        {
            int col{point.column + cols[k]};
            int row{point.row + rows[k]};
            if (isFieldValid(board, col, row, color) && !checked[col][row])
            {
                checked[col][row] = true;
                clusterSize++;
                toCheck.push({col, row});
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

Point findFirstCluster(const Board& board)
{
    std::vector<std::vector<bool>> checked{
        createCheckedVector(board.getColumnCount(), board.getRowCount())};
    for (int row = board.getRowCount() - 1; row >= 0; --row)
        for (unsigned int column = 0; column < board.getColumnCount(); ++column)
        {
            Point point{static_cast<int>(column), row};
            if (getClusterSize(board, point, checked) > 1)
                return point;
        }
    return emptyPoint;
}

static Point getRandomPoint(const Board& board)
{
    return {static_cast<int>(fastRandInt() % board.getColumnCount()),
            static_cast<int>(fastRandInt() % board.getRowCount())};
}

static unsigned int getRandomTries(const Board& board)
{
    return static_cast<unsigned int>(board.getColumnCount() *
                                     board.getRowCount() * .4);
}

Point findBiggestCluster(const Board& board)
{
    std::vector<std::vector<bool>> checked{
        createCheckedVector(board.getColumnCount(), board.getRowCount())};
    unsigned int bestScore{0};
    Point bestPoint{emptyPoint};
    for (unsigned int i = 0; i < getRandomTries(board); ++i)
    {
        const Point currentPoint{getRandomPoint(board)};
        const unsigned int score{getClusterSize(board, currentPoint, checked)};
        if (score > 1 && score > bestScore)
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

std::set<int> makeMove(Board& board, const Point& startPoint)
{
    const int color{board.getColor(startPoint)};

    std::set<int> impactedColumns;
    std::queue<Point> pointToCheck;
    pointToCheck.push(startPoint);
    board.setEmpty(startPoint);
    while (!pointToCheck.empty())
    {
        const auto currentPoint{pointToCheck.front()};
        pointToCheck.pop();
        impactedColumns.insert(currentPoint.column);
        for (int k = 0; k < 4; ++k)
        {
            const int col{currentPoint.column + cols[k]};
            const int row{currentPoint.row + rows[k]};
            if (isFieldValid(board, col, row, color))
            {
                board.setEmpty({col, row});
                pointToCheck.push({col, row});
            }
        }
    }

    return impactedColumns;
}

std::vector<Point> playGame(Board board)
{
    if (board.getColumnCount() == 0 || board.getRowCount() == 0)
        return {emptyPoint};

    std::vector<Point> points;
    while (true)
    {
        const Point nextPoint{getNextMove(board)};
        points.push_back(nextPoint);
        if (nextPoint == emptyPoint)
            break;
        std::set<int> impactedColumns{makeMove(board, nextPoint)};
        impactGravity(board, std::move(impactedColumns));
    }
    return points;
}

};  // namespace SameGame
