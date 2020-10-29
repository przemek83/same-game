#pragma once

#include <set>
#include <vector>

#include "Board.h"
#include "Point.h"

namespace SameGame
{
Point getNextMove(const Board& board);

std::set<int> makeMove(Board& board, const Point& point);

void impactGravity(Board& board, std::set<int> impactedColumns);

unsigned int getClusterSize(const Board& board, Point startPoint,
                            bool (&checked)[Board::MAX_W][Board::MAX_H]);

std::vector<Point> playGame(Board board);

};  // namespace SameGame
