#include "SameGame.h"

#include <cstdlib>
#include <cstring>
#include <queue>

namespace SameGame
{
constexpr int cols[]{-1, 0, 0, 1};
constexpr int rows[]{0, -1, 1, 0};

bool operator==(const Point& left, const Point& right)
{
    return left.column == right.column && left.row == right.row;
}

bool operator<(const Point& left, const Point& right)
{
    if (left.column < right.column)
        return false;
    return left.row < right.row;
}

void impactGravity(std::vector<std::vector<int>>& board,
                   int (&impactedColumns)[MAX_W])
{
    for (unsigned int i = 0; i < board.size(); ++i)
    {
        if (impactedColumns[i] == EMPTY)
            continue;

        auto& column{board[i]};
        int emptyStartIndex = {EMPTY};
        unsigned int emptyCount{0};
        for (int row = impactedColumns[i]; row >= 0; --row)
        {
            if (column[row] == EMPTY)
            {
                if (emptyStartIndex == EMPTY)
                    emptyStartIndex = row;
                emptyCount++;
            }
            else
            {
                if (emptyStartIndex != EMPTY)
                {
                    for (size_t currentRow = emptyStartIndex;
                         currentRow >= emptyCount; --currentRow)
                    {
                        column[currentRow] = column[currentRow - emptyCount];
                        column[currentRow - emptyCount] = EMPTY;
                    }
                    emptyStartIndex = EMPTY;
                    row += emptyCount;
                    emptyCount = 0;
                }
            }
        }
        impactedColumns[i] = EMPTY;
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
                            Point startPoint, bool (&checked)[MAX_W][MAX_H],
                            unsigned int w, unsigned int h)
{
    int color{board[startPoint.column][startPoint.row]};
    if (color == EMPTY)
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

Point getNextMove(const std::vector<std::vector<int>>& board, unsigned int w,
                  unsigned int h)
{
    static bool checked[MAX_W][MAX_H] = {};
    std::memset(checked, false, sizeof(checked));
    const unsigned int randomTries{static_cast<unsigned int>(w * h * .4)};
    unsigned int currentBestScore{0};
    Point currentBestPoint{emptyPoint};
    // Try 40% random hits.
    for (unsigned int tryNumber = 0; tryNumber < randomTries; ++tryNumber)
    {
        Point point{static_cast<int>(fastRandInt() % w),
                    static_cast<int>(fastRandInt() % h)};
        unsigned int score = getClusterSize(board, point, checked, w, h);
        if (score > 1 && score > currentBestScore)
        {
            currentBestScore = score;
            currentBestPoint = point;
        }
    }

    if (currentBestScore > 0)
        return currentBestPoint;

    // If not found iterate one by one searching for cluster.
    for (int row = 0; row < h; ++row)
    {
        for (int column = 0; column < w; ++column)
        {
            Point point{column, row};
            if (getClusterSize(board, point, checked, w, h) > 1)
                return point;
        }
    }

    return {};
}

void makeMove(std::vector<std::vector<int>>& board,
              int (&impactedColumns)[MAX_W], const Point& point)
{
    int color{board[point.column][point.row]};

    static std::queue<Point> toCheck;
    toCheck.push(point);
    board[point.column][point.row] = EMPTY;
    while (!toCheck.empty())
    {
        const auto currentPoint{toCheck.front()};
        toCheck.pop();

        if (impactedColumns[currentPoint.column] == EMPTY)
            impactedColumns[currentPoint.column] = currentPoint.row;
        else
        {
            if (impactedColumns[currentPoint.column] < currentPoint.row)
                impactedColumns[currentPoint.column] = currentPoint.row;
        }

        for (int k = 0; k < 4; ++k)
        {
            int col{currentPoint.column + cols[k]};
            int row{currentPoint.row + rows[k]};
            if (isFieldValid(board, col, row, color, board.size(),
                             board[0].size()))
            {
                board[col][row] = EMPTY;
                toCheck.push({col, row});
            }
        }
    }
}

std::vector<Point> playGame(std::vector<std::vector<int>> board)
{
    if (board.size() == 0)
        return {emptyPoint};

    const unsigned int columnsCount{static_cast<unsigned int>(board.size())};
    const unsigned int rowsCount{
        static_cast<unsigned int>(board.front().size())};

    static int impactedColumns[MAX_W];
    std::memset(impactedColumns, EMPTY, sizeof(impactedColumns));

    std::vector<Point> points;
    while (true)
    {
        const Point movePoint{getNextMove(board, columnsCount, rowsCount)};

        points.push_back(movePoint);
        if (movePoint == emptyPoint)
            break;
        makeMove(board, impactedColumns, movePoint);
        impactGravity(board, impactedColumns);
        // printBoard(board);
    }
    return points;
}

};  // namespace SameGame
