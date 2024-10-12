#pragma once

#include <src/Board.h>

namespace TestTools
{
Board createBoard(const std::vector<std::vector<unsigned int> >& data);

Board prepareBoard(unsigned int columnsCount, unsigned int rowsCount,
                   const std::string& fileName);
};  // namespace TestTools
