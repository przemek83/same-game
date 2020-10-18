#pragma once

#include <vector>

#include "Board.h"

namespace SameGame
{
constexpr int EMPTY = -1;

struct Point
{
    int column{EMPTY};
    int row{EMPTY};
};

constexpr Point emptyPoint{EMPTY, EMPTY};

bool operator==(const Point& left, const Point& right);

Point getNextMove(const std::vector<std::vector<int>>& board, unsigned int w,
                  unsigned int h);

void makeMove(std::vector<std::vector<int>>& board,
              int (&impactedColumns)[Board::MAX_W], const Point& point);

void impactGravity(std::vector<std::vector<int>>& board,
                   int (&impactedColumns)[Board::MAX_W]);

unsigned int getClusterSize(const std::vector<std::vector<int>>& board,
                            Point startPoint,
                            bool (&checked)[Board::MAX_W][Board::MAX_H],
                            unsigned int w, unsigned int h);

std::vector<Point> playGame(std::vector<std::vector<int>> board);

};  // namespace SameGame
