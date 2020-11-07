#pragma once

#include "../Board.h"

namespace TestTools
{
Board createBoard(const std::vector<std::vector<unsigned int> >& data);

Board prepareBoard(unsigned int columnsCount, unsigned int rowsCount,
                   std::string fileName);
};  // namespace TestTools
