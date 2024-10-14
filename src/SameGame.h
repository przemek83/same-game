#pragma once

#include <set>
#include <vector>

#include "Board.h"
#include "Point.h"

class Generator;

namespace SameGame
{
Point getNextMove(const Board& board, Generator& generator);

std::set<int> makeMove(Board& board, Point point);

void impactGravity(Board& board, const std::set<int>& impactedColumns);

int getClusterSize(const Board& board, Point startPoint,
                   std::vector<std::vector<char> >& checked);

std::vector<Point> playGame(Board board, Generator& generator);

};  // namespace SameGame
