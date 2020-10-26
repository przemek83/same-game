#include "SameGame.h"

#include <cstdlib>
#include <cstring>
#include <queue>

namespace SameGame
{
constexpr int cols[]{-1, 0, 0, 1};
constexpr int rows[]{0, -1, 1, 0};

constexpr Point emptyPoint{Point::EMPTY, Point::EMPTY};

void impactGravity(std::vector<std::vector<int>>& board,
                   std::set<int> impactedColumns)
{
    const unsigned long rowCount{board[0].size()};
    for (int impactedColumn : impactedColumns)
    {
        auto& column{board[impactedColumn]};
        int emptyStartIndex = {Point::EMPTY};
        unsigned int emptyCount{0};

        for (int row = rowCount; row >= 0; --row)
        {
            if (column[row] == Point::EMPTY)
            {
                if (emptyStartIndex == Point::EMPTY)
                    emptyStartIndex = row;
                emptyCount++;
            }
            else
            {
                if (emptyStartIndex == Point::EMPTY)
                    continue;

                for (size_t currentRow = emptyStartIndex;
                     currentRow >= emptyCount; --currentRow)
                {
                    column[currentRow] = column[currentRow - emptyCount];
                    column[currentRow - emptyCount] = Point::EMPTY;
                }
                emptyStartIndex = Point::EMPTY;
                row += emptyCount;
                emptyCount = 0;
            }
        }
    }
}

bool isFieldValid(const std::vector<std::vector<int>>& board,
                  unsigned int column, unsigned int row, int color,
                  unsigned int w, unsigned int h)
{
    return column >= 0 && column < w && row >= 0 && row < h &&
           board[column][row] == color;
}

unsigned int getClusterSize(const std::vector<std::vector<int>>& board,
                            Point startPoint,
                            bool (&checked)[Board::MAX_W][Board::MAX_H],
                            unsigned int w, unsigned int h)
{
    int color{board[startPoint.column][startPoint.row]};
    if (color == Point::EMPTY)
        return 0;

    unsigned int clusterSize{0};
    static std::queue<Point> toCheck;
    toCheck.push(startPoint);
    while (!toCheck.empty())
    {
        const auto point{toCheck.front()};
        toCheck.pop();
        if (board[point.column][point.row] != color)
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
            if (!checked[col][row] &&
                isFieldValid(board, col, row, color, w, h))
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

Point getNextMove(const std::vector<std::vector<int>>& board)
{
    const unsigned int columnsCount{static_cast<unsigned int>(board.size())};
    const unsigned int rowsCount{
        static_cast<unsigned int>(board.front().size())};

    static bool checked[Board::MAX_W][Board::MAX_H] = {};
    std::memset(checked, false, sizeof(checked));
    const unsigned int randomTries{
        static_cast<unsigned int>(columnsCount * rowsCount * .4)};
    unsigned int currentBestScore{0};
    Point currentBestPoint{emptyPoint};
    // Try 40% random hits.
    for (unsigned int tryNumber = 0; tryNumber < randomTries; ++tryNumber)
    {
        Point point{static_cast<int>(fastRandInt() % columnsCount),
                    static_cast<int>(fastRandInt() % rowsCount)};
        unsigned int score =
            getClusterSize(board, point, checked, columnsCount, rowsCount);
        if (score > 1 && score > currentBestScore)
        {
            currentBestScore = score;
            currentBestPoint = point;
        }
    }

    if (currentBestScore > 0)
        return currentBestPoint;

    // If not found iterate one by one searching for cluster.
    for (int row = 0; row < rowsCount; ++row)
    {
        for (int column = 0; column < columnsCount; ++column)
        {
            Point point{column, row};
            if (getClusterSize(board, point, checked, rowsCount, columnsCount) >
                1)
                return point;
        }
    }

    return {};
}

std::set<int> makeMove(std::vector<std::vector<int>>& board, const Point& point)
{
    int color{board[point.column][point.row]};

    std::set<int> impactedColumns;
    std::queue<Point> toCheck;
    toCheck.push(point);
    board[point.column][point.row] = Point::EMPTY;
    while (!toCheck.empty())
    {
        const auto currentPoint{toCheck.front()};
        toCheck.pop();

        impactedColumns.insert(currentPoint.column);

        for (int k = 0; k < 4; ++k)
        {
            int col{currentPoint.column + cols[k]};
            int row{currentPoint.row + rows[k]};
            if (isFieldValid(board, col, row, color, board.size(),
                             board[0].size()))
            {
                board[col][row] = Point::EMPTY;
                toCheck.push({col, row});
            }
        }
    }

    return impactedColumns;
}

std::vector<Point> playGame(std::vector<std::vector<int>> board)
{
    if (board.size() == 0)
        return {emptyPoint};

    std::vector<int> impactedColumns(board.size(), Point::EMPTY);
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
