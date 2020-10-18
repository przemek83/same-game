#pragma once

#include <sstream>
#include <vector>

namespace SameGame
{
constexpr int EMPTY = -1;
constexpr unsigned int MIN_W{4};
constexpr unsigned int MIN_H{4};
constexpr unsigned int MIN_C{3};
constexpr unsigned int MAX_W{500};
constexpr unsigned int MAX_H{500};
constexpr unsigned int MAX_C{20};

struct Point
{
    int column{EMPTY};
    int row{EMPTY};
};

constexpr Point emptyPoint{EMPTY, EMPTY};

bool operator==(const Point& left, const Point& right);

void printBoard(const std::vector<std::vector<int>>& board);

Point getNextMove(const std::vector<std::vector<int>>& board, unsigned int w,
                  unsigned int h);

void makeMove(std::vector<std::vector<int>>& board,
              int (&impactedColumns)[MAX_W], const Point& point);

void impactGravity(std::vector<std::vector<int>>& board,
                   int (&impactedColumns)[MAX_W]);

unsigned int getClusterSize(const std::vector<std::vector<int>>& board,
                            Point startPoint, bool (&checked)[MAX_W][MAX_H],
                            unsigned int w, unsigned int h);

std::vector<std::vector<int>> loadBoard(unsigned int columnsCount,
                                        unsigned int rowsCount,
                                        std::istream& in);

std::vector<Point> playGame(std::vector<std::vector<int>> board);

};  // namespace SameGame
