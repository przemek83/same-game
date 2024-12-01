#pragma once

#include <src/Board.h>

namespace TestTools
{
Board createBoard(const std::vector<std::vector<int> >& data);

Board prepareBoard(int columnsCount, int rowsCount,
                   const std::string& fileName);
};  // namespace TestTools
