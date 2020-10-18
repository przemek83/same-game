#include "Board.h"

#include <iostream>

namespace Board
{
constexpr unsigned int MIN_W{4};
constexpr unsigned int MIN_H{4};
constexpr unsigned int MIN_C{3};
constexpr unsigned int MAX_C{20};

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

bool isBoardDescriptionValid(unsigned int rowCount, unsigned int columnCount,
                             unsigned int colorCount)
{
    return rowCount >= MIN_H && rowCount < MAX_H && columnCount >= MIN_W &&
           columnCount < MAX_W && colorCount >= MIN_C && colorCount < MAX_C;
}

}  // namespace Board
