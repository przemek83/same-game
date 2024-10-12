#pragma once

#include <set>
#include <vector>

#include "Board.h"
#include "Point.h"

namespace SameGame
{
Point getNextMove(const Board& board);

std::set<unsigned int> makeMove(Board& board, Point point);

void impactGravity(Board& board, const std::set<unsigned int>& impactedColumns);

unsigned int getClusterSize(const Board& board, Point startPoint,
                            std::vector<std::vector<bool> >& checked);

std::vector<Point> playGame(Board board);

};  // namespace SameGame
