#pragma once

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

    Board& board_;
    Generator& generator_;
};
