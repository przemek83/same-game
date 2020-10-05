// Simple board game based on rules of Same Game (known also as JawBreaker and
// few other names). Played by basic algorithm.

// Rules:
// Program gets H x W board on input with C different numbers as cells.
// Assumption:
//  4 <= H <= 50 – board rows count
//  4 <= W <= 50 – board columns count
//  3 <= C <= 20 – color count
// Program can remove cells if there are minimum 1 neighbour in same color
// (above, below, left or right). Gravity fill the gaps if occur.
// Game ends when on boards there is no cells with neighbour in same color.

// Input:
// Number of games followed by line containing H W C (space separated) followed
// H lines with W numbers representing initial board.

// Output:
// In first line "Y" if want to proceed with game and "N" if not followed by
// coordinates of cells to pick. At end "-1 -1" meaning no more moves.

// Example:
// In
//    1
//    4 4 3
//    0 0 1 1
//    1 1 2 2
//    0 1 2 0
//    0 1 1 2

// Out
//    Y
//    1 0
//    1 0
//    3 2
//    -1 -1

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

constexpr int EMPTY = -1;
constexpr unsigned int MIN_W{4};
constexpr unsigned int MIN_H{4};
constexpr unsigned int MIN_C{3};
constexpr unsigned int MAX_W{50};
constexpr unsigned int MAX_H{50};
constexpr unsigned int MAX_C{20};

constexpr int cols[]{-1, 0, 0, 1};
constexpr int rows[]{0, -1, 1, 0};

struct Point
{
    int column{EMPTY};
    int row{EMPTY};
};

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

Point emptyPoint{EMPTY, EMPTY};

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
    const unsigned int randomTries{static_cast<unsigned int>(w * h * .6)};
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

void test();

int main()
{
    // auto start = std::chrono::high_resolution_clock::now();
    test();
    // bigTestFile3Colors.txt bigTestFile.txt
    // testFileVeryBig
    // testFileVeryBig3Colors.txt
    // testFileVeryVeryBig.txt std::ifstream in("testFileVeryVeryBig.txt",
    // std::ifstream::in); std::cin.rdbuf(in.rdbuf());

    srand(time(NULL));

    unsigned int testCount;
    std::cin >> testCount;
    for (unsigned int i = 0; i < testCount; ++i)
    {
        unsigned int h;
        unsigned int w;
        unsigned int c;
        std::cin >> h >> w >> c;

        if (h < MIN_H || h > MAX_H || w < MIN_W || w > MAX_W || c < MIN_C ||
            c > MAX_C)
        {
            std::cout << "N" << std::endl;
            continue;
        }

        std::vector<std::vector<int>> board(w);
        for (auto& column : board)
            column.resize(h);

        for (unsigned int row = 0; row < h; ++row)
            for (unsigned int column = 0; column < w; ++column)
            {
                int field;
                std::cin >> field;
                board[column][row] = field;
            }

        std::ostringstream output;
        // printBoard(board);
        output << "Y" << std::endl;

        static bool checked[MAX_W][MAX_H] = {};
        std::memset(checked, false, sizeof(checked));

        static int impactedColumns[MAX_W];
        std::memset(impactedColumns, EMPTY, sizeof(impactedColumns));

        Point movePoint{emptyPoint};
        while (true)
        {
            movePoint = getNextMove(board, checked, w, h);
            if (movePoint == emptyPoint)
                break;
            printPoint(movePoint, output);
            makeMove(board, checked, impactedColumns, movePoint);
            impactGravity(board, impactedColumns);
            // printBoard(board);
        }
        printPoint(emptyPoint, output);
        std::cout << output.str();
    }

    //    auto stop = std::chrono::high_resolution_clock::now();
    //    auto duration =
    //        std::chrono::duration_cast<std::chrono::milliseconds>(stop -
    //        start);
    //    std::cout << duration.count() << std::endl;
    return 0;
}

void performImpactGravityTest(std::vector<std::vector<int>> board,
                              int (&impactedColumns)[MAX_W],
                              std::vector<std::vector<int>> expected,
                              std::string testName)
{
    impactGravity(board, impactedColumns);
    if (board != expected)
    {
        std::cout << "FAIL: impactGravity " + testName << std::endl;
        std::cout << "Got:" << std::endl;
        printBoard(board);
        std::cout << "Expected:" << std::endl;
        printBoard(expected);
    }
    else
        std::cout << "SUCCESS: impactGravity " + testName << std::endl;
}

void performMakeMoveTest(std::vector<std::vector<int>> board,
                         bool (&checked)[MAX_W][MAX_H],
                         int (&impactedColumns)[MAX_W], const Point& point,
                         std::vector<std::vector<int>> expected,
                         std::string testName)
{
    makeMove(board, checked, impactedColumns, point);
    if (board != expected)
    {
        std::cout << "FAIL: makeMove " + testName << std::endl;
        std::cout << "Got:" << std::endl;
        printBoard(board);
        std::cout << "Expected:" << std::endl;
        printBoard(expected);
    }
    else
        std::cout << "SUCCESS: makeMove " + testName << std::endl;
}

void performGetClusterTests(const std::vector<std::vector<int>>& board,
                            unsigned int expectedClusterSize, Point point,
                            std::string testName)
{
    bool checked[MAX_W][MAX_H] = {};
    std::memset(checked, false, sizeof(checked));
    unsigned int currentClusterSize{
        getClusterSize(board, point, checked, board.size(), board[0].size())};
    if (currentClusterSize != expectedClusterSize)
    {
        std::cout << "FAIL: getCluster " + testName << std::endl;
        std::cout << "Got:" << currentClusterSize << ", expected "
                  << expectedClusterSize << std::endl;
    }
    else
        std::cout << "SUCCESS: getCluster " + testName << std::endl;
}

void test()
{
    std::vector<std::vector<int>> board{{1, 3, 4, -1, 5}};
    std::vector<std::vector<int>> expected{{-1, 1, 3, 4, 5}};
    int impactedColumns[MAX_W];
    impactedColumns[0] = 3;
    performImpactGravityTest(board, impactedColumns, expected, "1");
    board = {{1, 3, -1, -1, -1}};
    expected = {{-1, -1, -1, 1, 3}};
    impactedColumns[0] = 4;
    performImpactGravityTest(board, impactedColumns, expected, "2");
    board = {{-1, -1, -1, -1, -1}};
    expected = {{-1, -1, -1, -1, -1}};
    impactedColumns[0] = EMPTY;
    performImpactGravityTest(board, impactedColumns, expected, "3");
    board = {{1, 2, 3, 4, 5}};
    expected = {{1, 2, 3, 4, 5}};
    impactedColumns[0] = EMPTY;
    performImpactGravityTest(board, impactedColumns, expected, "4");
    board = {{1, -1, -1, -1, -1}, {1, 2, -1, -1, -1},  {1, -1, -1, 1, -1},
             {1, -1, -1, -1, 2},  {-1, -1, -1, 1, -1}, {1, -1, 1, -1, 1}};
    expected = {{-1, -1, -1, -1, 1}, {-1, -1, -1, 1, 2},  {-1, -1, -1, 1, 1},
                {-1, -1, -1, 1, 2},  {-1, -1, -1, -1, 1}, {-1, -1, 1, 1, 1}};
    impactedColumns[0] = 4;
    impactedColumns[1] = 4;
    impactedColumns[2] = 4;
    impactedColumns[3] = 3;
    impactedColumns[4] = 4;
    impactedColumns[5] = 3;
    performImpactGravityTest(board, impactedColumns, expected, "5");

    bool checked[MAX_W][MAX_H] = {};
    checked[2][3] = true;
    checked[2][4] = true;
    checked[3][3] = true;
    std::memset(impactedColumns, EMPTY, sizeof(impactedColumns));

    performMakeMoveTest(expected, checked, impactedColumns, {2, 3},
                        {{-1, -1, -1, -1, 1},
                         {-1, -1, -1, -1, 2},
                         {-1, -1, -1, -1, -1},
                         {-1, -1, -1, -1, 2},
                         {-1, -1, -1, -1, 1},
                         {-1, -1, 1, 1, 1}},
                        "1");

    if (checked[2][3] || checked[2][4] || checked[3][3])
        std::cout << "FAIL: makeMove checked" << std::endl;

    board = {{0, 1, 0, 0}, {0, 1, 1, 1}, {1, 2, 2, 1}, {1, 2, 0, 2}};
    expected = {{0, -1, 0, 0}, {0, -1, -1, -1}, {1, 2, 2, -1}, {1, 2, 0, 2}};
    performMakeMoveTest(board, checked, impactedColumns, {2, 3}, expected, "2");

    board = {{0, 1, 0, 0}, {0, 1, 1, 1}, {1, 2, 2, 1}, {1, 2, 0, 2}};
    performGetClusterTests(board, 2, {0, 0}, "1");
    performGetClusterTests(board, 5, {0, 1}, "2");
    performGetClusterTests(board, 1, {3, 2}, "3");
    performGetClusterTests(board, 3, {2, 1}, "4");

    board = {{-1, 1, 0, 0}};
    performGetClusterTests(board, 0, {0, 0}, "5");
    performGetClusterTests(board, 1, {0, 1}, "6");
    performGetClusterTests(board, 2, {0, 3}, "7");
    std::cout << "========== END TESTS ===========" << std::endl;
}
