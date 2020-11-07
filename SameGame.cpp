#include "SameGame.h"

#include <cstdlib>
#include <cstring>
#include <queue>

namespace SameGame
{
constexpr int cols[]{-1, 0, 0, 1};
constexpr int rows[]{0, -1, 1, 0};

constexpr Point emptyPoint{Point::NOT_SET, Point::NOT_SET};

void impactGravity(Board& board, std::set<unsigned int> impactedColumns)
{
    for (unsigned int impactedColumn : impactedColumns)
    {
        int emptyStartIndex{-1};
        unsigned int emptyCount{0};

        for (int row = board.getRowCount() - 1; row >= 0; --row)
        {
            if (board.getColor({impactedColumn, static_cast<unsigned int>(
                                                    row)}) == Board::EMPTY)
            {
                if (emptyStartIndex == -1)
                    emptyStartIndex = row;
                emptyCount++;
                continue;
            }

            if (emptyStartIndex == -1)
                continue;

            for (int currentRow = emptyStartIndex; currentRow >= emptyCount;
                 --currentRow)
            {
                board.setColor(
                    {impactedColumn, static_cast<unsigned int>(currentRow)},
                    board.getColor(
                        {impactedColumn,
                         static_cast<unsigned int>(currentRow - emptyCount)}));
                board.setEmpty({impactedColumn, static_cast<unsigned int>(
                                                    currentRow - emptyCount)});
            }
            emptyStartIndex = -1;
            row += emptyCount;
            emptyCount = 0;
        }
    }
}

bool isFieldValid(const Board& board, unsigned int column, unsigned int row,
                  int color)
{
    return column >= 0 && column < board.getColumnCount() && row >= 0 &&
           row < board.getRowCount() && board.getColor({column, row}) == color;
}

unsigned int getClusterSize(const Board& board, Point startPoint,
                            std::vector<std::vector<bool>>& checked)
{
    int color{board.getColor(startPoint)};
    if (color == Board::EMPTY)
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
            unsigned int col{point.column + cols[k]};
            unsigned int row{point.row + rows[k]};
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
            Point point{column, static_cast<unsigned int>(row)};
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

std::set<unsigned int> makeMove(Board& board, const Point& startPoint)
{
    const int color{board.getColor(startPoint)};

    std::set<unsigned int> impactedColumns;
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
            const unsigned int col{currentPoint.column + cols[k]};
            const unsigned int row{currentPoint.row + rows[k]};
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
        return {};

    std::vector<Point> points;
    while (true)
    {
        const Point nextPoint{getNextMove(board)};
        if (nextPoint == emptyPoint)
            break;
        points.push_back(nextPoint);
        std::set<unsigned int> impactedColumns{makeMove(board, nextPoint)};
        impactGravity(board, std::move(impactedColumns));
    }
    return points;
}

};  // namespace SameGame
