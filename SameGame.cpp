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
                            bool (&checked)[Board::MAX_W][Board::MAX_H])
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
            if (!checked[col][row] && isFieldValid(board, col, row, color))
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

Point findFirstCluster(const Board& board,
                       bool (&checked)[Board::MAX_W][Board::MAX_H])
{
    // Iterate one by one searching for cluster.
    for (unsigned int row = 0; row < board.getRowCount(); ++row)
    {
        for (unsigned int column = 0; column < board.getColumnCount(); ++column)
        {
            Point point{static_cast<int>(column), static_cast<int>(row)};
            if (getClusterSize(board, point, checked) > 1)
                return point;
        }
    }
    return emptyPoint;
}

Point findBiggestCluster(const Board& board,
                         bool (&checked)[Board::MAX_W][Board::MAX_H])
{
    const unsigned int randomTries{static_cast<unsigned int>(
        board.getColumnCount() * board.getRowCount() * .4)};
    unsigned int currentBestScore{0};
    Point currentBestPoint{emptyPoint};
    for (unsigned int tryNumber = 0; tryNumber < randomTries; ++tryNumber)
    {
        Point point{static_cast<int>(fastRandInt() % board.getColumnCount()),
                    static_cast<int>(fastRandInt() % board.getRowCount())};
        unsigned int score = getClusterSize(board, point, checked);
        if (score > 1 && score > currentBestScore)
        {
            currentBestScore = score;
            currentBestPoint = point;
        }
    }

    if (currentBestScore > 0)
        return currentBestPoint;

    return emptyPoint;
}

Point getNextMove(const Board& board)
{
    static bool checked[Board::MAX_W][Board::MAX_H] = {};
    std::memset(checked, false, sizeof(checked));

    Point nextPoint{findBiggestCluster(board, checked)};
    if (nextPoint == emptyPoint)
        nextPoint = findFirstCluster(board, checked);
    return nextPoint;
}

std::set<int> makeMove(Board& board, const Point& point)
{
    int color{board.getColor(point)};

    std::set<int> impactedColumns;
    std::queue<Point> toCheck;
    toCheck.push(point);
    board.setEmpty(point);
    while (!toCheck.empty())
    {
        const auto currentPoint{toCheck.front()};
        toCheck.pop();

        impactedColumns.insert(currentPoint.column);

        for (int k = 0; k < 4; ++k)
        {
            int col{currentPoint.column + cols[k]};
            int row{currentPoint.row + rows[k]};
            if (isFieldValid(board, col, row, color))
            {
                board.setEmpty({col, row});
                toCheck.push({col, row});
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
