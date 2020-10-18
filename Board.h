#pragma once

#include <sstream>
#include <vector>

namespace Board
{
constexpr unsigned int MAX_W{500};
constexpr unsigned int MAX_H{500};

void printBoard(const std::vector<std::vector<int>>& board);

std::vector<std::vector<int>> loadBoard(unsigned int columnsCount,
                                        unsigned int rowsCount,
                                        std::istream& in);

bool isBoardDescriptionValid(unsigned int rowCount, unsigned int columnCount,
                             unsigned int colorCount);
};  // namespace Board
