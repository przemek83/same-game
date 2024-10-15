#include "Board.h"

#include <cstddef>
#include <iostream>
#include <ostream>
#include <sstream>

Board::Board(int columnsCount, int rowsCount, std::istream& in)
    : columnsCount_(columnsCount),
      rowsCount_(rowsCount),
      data_(static_cast<size_t>(columnsCount))
{
    initData();
    loadData(in);
}

std::string Board::print() const
{
    std::ostringstream outString;
    for (std::size_t row{0}; row < data_[0].size(); ++row)
    {
        for (const auto& column : data_)
            outString << column[row] << "\t";
        outString << "\n";
    }
    return outString.str();
}

bool Board::isDescriptionValid(int colorCount)
{
    return colorCount >= MIN_COLOR_COUNT;
}

void Board::initData()
{
    data_.resize(static_cast<std::size_t>(columnsCount_));
    for (auto& column : data_)
        column.resize(static_cast<std::size_t>(rowsCount_));
}

void Board::loadData(std::istream& in)
{
    for (std::size_t row{0}; row < static_cast<std::size_t>(rowsCount_); ++row)
        for (std::size_t column{0};
             column < static_cast<std::size_t>(columnsCount_); ++column)
        {
            int field{0};
            in >> field;
            data_[column][row] = field;
        }
}
