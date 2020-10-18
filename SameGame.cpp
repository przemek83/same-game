#include "SameGame.h"

#include <limits.h>
#include <algorithm>
#include <chrono>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>

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

Point getNextMove(const std::vector<std::vector<int>>& board,
                  bool (&checked)[MAX_W][MAX_H], unsigned int w, unsigned int h)
{
    std::memset(checked, false, sizeof(checked));
    const unsigned int randomTries{static_cast<unsigned int>(w * h * .4)};
    // const int maxSuccessfullTries{50};
    // unsigned int successfullTries{0};
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
            // checked[point.column][point.row] = false;
        }
    }

    return {};
}

void makeMove(std::vector<std::vector<int>>& board,
              bool (&checked)[MAX_W][MAX_H], int (&impactedColumns)[MAX_W],
              const Point& point)
{
    int color{board[point.column][point.row]};

    static std::queue<Point> toCheck;
    toCheck.push(point);
    board[point.column][point.row] = EMPTY;
    checked[point.column][point.row] = false;
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
                checked[col][row] = false;
                toCheck.push({col, row});
            }
        }
    }
}

void printBoard(const std::vector<std::vector<int>>& board)
{
    if (board.empty())
        return;

    for (unsigned int row = 0; row < board[0].size(); ++row)
    {
        for (unsigned int column = 0; column < board.size(); ++column)
            std::cout << board[column][row] << "\t";
        std::cout << std::endl;
    }
}

void printPoint(Point point, std::ostringstream& output)
{
    output << point.row << " " << point.column << std::endl;
}

std::vector<std::vector<int>> loadBoard(unsigned int columnsCount,
                                        unsigned int rowsCount,
                                        std::istream& in)
{
    std::vector<std::vector<int>> board(columnsCount);
    for (auto& column : board)
        column.resize(rowsCount);

    for (unsigned int row = 0; row < rowsCount; ++row)
        for (unsigned int column = 0; column < columnsCount; ++column)
        {
            int field;
            in >> field;
            board[column][row] = field;
        }

    return board;
}

std::vector<Point> playGame(std::vector<std::vector<int>> board)
{
    if (board.size() == 0)
        return {emptyPoint};

    const unsigned int columnsCount{static_cast<unsigned int>(board.size())};
    const unsigned int rowsCount{
        static_cast<unsigned int>(board.front().size())};
    std::vector<Point> points;
    static bool checked[MAX_W][MAX_H] = {};
    std::memset(checked, false, sizeof(checked));

    static int impactedColumns[MAX_W];
    std::memset(impactedColumns, EMPTY, sizeof(impactedColumns));

    Point movePoint{emptyPoint};
    while (true)
    {
        movePoint = getNextMove(board, checked, columnsCount, rowsCount);

        points.push_back(movePoint);
        if (movePoint == emptyPoint)
            break;
        makeMove(board, checked, impactedColumns, movePoint);
        impactGravity(board, impactedColumns);
        // printBoard(board);
    }
    return points;
}

};  // namespace SameGame
