#pragma once

#include <array>
#include <queue>
#include <set>
#include <vector>

#include "Board.h"
#include "Point.h"

class Generator;

class SameGame
{
public:
    SameGame(Board& board, Generator& generator);

    using CheckedBoard = std::vector<std::vector<unsigned char>>;

    int getClusterSize(Point startPoint, CheckedBoard& checked);

    Point getNextMove();

    std::set<int> makeMove(Point point);

    void impactGravity(const std::set<int>& impactedColumns);

    std::vector<Point> playGame();

private:
    void removeRows(int column, int fromRow, int count);

    void impactColumn(int column);

    Point findFirstCluster();

    Point findBiggestCluster();

    bool isFieldValid(const Board& board, int column, int row) const;

    CheckedBoard createCheckedVector(int columnCount, int rowCount) const;

    Point getRandomPoint(const Board& board, Generator& generator) const;

    int getRandomTries(const Board& board) const;

    unsigned char& getPosition(CheckedBoard& checked, Point point);

    Board& board_;
    Generator& generator_;

    constexpr static std::array<int, 4> cols{-1, 0, 0, 1};
    constexpr static std::array<int, 4> rows{0, -1, 1, 0};

    constexpr static unsigned char CHECKED{1};
    constexpr static unsigned char NOT_CHECKED{0};

    constexpr static Point emptyPoint{Point::NOT_SET, Point::NOT_SET};

    std::queue<Point> pointsToCheck_;
};
