#include "Board.h"

#include <iostream>
#include <ostream>

Board::Board(unsigned int columnsCount, unsigned int rowsCount,
             std::istream& in)
    : columnsCount_(columnsCount), rowsCount_(rowsCount), data_(columnsCount)
{
    initData(columnsCount, rowsCount);
    loadData(in);
}

std::string Board::print() const
{
    std::ostringstream outString;
    for (unsigned int row = 0; row < data_[0].size(); ++row)
    {
        for (const auto& column : data_)
            outString << column[row] << "\t";
        outString << std::endl;
    }
    return outString.str();
}

bool Board::isDescriptionValid(unsigned int colorCount)
{
    return colorCount >= MIN_COLOR_COUNT;
}

bool Board::operator==(const Board& that) const
{
    return columnsCount_ == that.columnsCount_ &&
           rowsCount_ == that.rowsCount_ && data_ == that.data_;
}

void Board::initData(unsigned int columnsCount, unsigned int rowsCount)
{
    data_.resize(columnsCount);
    for (auto& column : data_)
        column.resize(rowsCount);
}

void Board::loadData(std::istream& in)
{
    for (unsigned int row = 0; row < rowsCount_; ++row)
        for (unsigned int column = 0; column < columnsCount_; ++column)
        {
            unsigned int field{0};
            in >> field;
            data_[column][row] = field;
        }
}
