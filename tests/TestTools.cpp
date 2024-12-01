#include "TestTools.h"

#include <fstream>
#include <sstream>

namespace TestTools
{
Board createBoard(const std::vector<std::vector<int>>& data)
{
    const int columnCount{static_cast<int>(data.size())};
    const int rowCount{static_cast<int>(data[0].size())};

    std::istringstream stringStream;
    Board board(columnCount, rowCount, stringStream);
    for (int row = 0; row < rowCount; ++row)
        for (int column = 0; column < columnCount; ++column)
            board.setColor({column, row}, data[column][row]);
    return board;
}

Board prepareBoard(int columnsCount, int rowsCount, const std::string& fileName)
{
    std::ifstream in(fileName, std::ifstream::in);
    return Board(columnsCount, rowsCount, in);
}
};  // namespace TestTools
