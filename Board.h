#pragma once

#include <sstream>
#include <vector>

namespace Board
{
void printBoard(const std::vector<std::vector<int>>& board);

std::vector<std::vector<int>> loadBoard(unsigned int columnsCount,
                                        unsigned int rowsCount,
                                        std::istream& in);
};  // namespace Board
