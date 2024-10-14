#pragma once

#include <array>
#include <set>
#include <vector>

#include "Board.h"
#include "Point.h"

class Generator;

class SameGame
{
public:
    SameGame(Board& board, Generator& generator);

    int getClusterSize(Point startPoint,
                       std::vector<std::vector<char>>& checked);

    Point getNextMove();

    std::set<int> makeMove(Point point);

    void impactGravity(const std::set<int>& impactedColumns);

    std::vector<Point> playGame();

private:
    void removeRows(int column, int fromRow, int count);

    void impactColumn(int column);

    Point findFirstCluster();

    Point findBiggestCluster();

    bool isFieldValid(const Board& board, int column, int row);

    std::vector<std::vector<char>> createCheckedVector(int columnCount,
                                                       int rowCount);

    Point getRandomPoint(const Board& board, Generator& generator);

    int getRandomTries(const Board& board);

    char& getPosition(std::vector<std::vector<char>>& checked, Point point);

    Board& board_;
    Generator& generator_;

    constexpr static std::array<int, 4> cols{-1, 0, 0, 1};
    constexpr static std::array<int, 4> rows{0, -1, 1, 0};

    constexpr static char CHECKED{1};
    constexpr static char NOT_CHECKED{0};

    constexpr static Point emptyPoint{Point::NOT_SET, Point::NOT_SET};
};
