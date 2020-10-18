#include "Board.h"

#include <iostream>

namespace Board
{
void printBoard(const std::vector<std::vector<int>>& board)
{
    if (board.empty())
        return;

    for (unsigned int row = 0; row < board[0].size(); ++row)
    {
        for (unsigned int column = 0; column < board.size(); ++column)
            std::cout << board[column][row] << "\t";
        std::cout << std::endl;
    }
}

std::vector<std::vector<int>> loadBoard(unsigned int columnsCount,
                                        unsigned int rowsCount,
                                        std::istream& in)
{
    std::vector<std::vector<int>> board(columnsCount);
    for (auto& column : board)
        column.resize(rowsCount);

    for (unsigned int row = 0; row < rowsCount; ++row)
        for (unsigned int column = 0; column < columnsCount; ++column)
        {
            int field;
            in >> field;
            board[column][row] = field;
        }

    return board;
}
}  // namespace Board
