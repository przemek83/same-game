#include "TestTools.h"

#include <fstream>
#include <sstream>

namespace TestTools
{
Board createBoard(const std::vector<std::vector<unsigned int>>& data)
{
    const unsigned int columnCount{static_cast<unsigned int>(data.size())};
    const unsigned int rowCount{static_cast<unsigned int>(data[0].size())};

    std::istringstream stringStream;
    Board board(columnCount, rowCount, stringStream);
    for (unsigned int row = 0; row < rowCount; ++row)
        for (unsigned int column = 0; column < columnCount; ++column)
            board.setColor({column, row}, data[column][row]);
    return board;
}

Board prepareBoard(unsigned int columnsCount, unsigned int rowsCount,
                   const std::string& fileName)
{
    std::ifstream in(fileName, std::ifstream::in);
    return Board(columnsCount, rowsCount, in);
}
};  // namespace TestTools
