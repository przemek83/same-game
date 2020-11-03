#include "Board.h"

#include <iostream>

Board::Board(unsigned int columnsCount, unsigned int rowsCount,
             std::istream& in)
    : columnsCount_(columnsCount), rowsCount_(rowsCount), data_(columnsCount)
{
    initData(columnsCount, rowsCount);
    loadData(in);
}

void Board::print()
{
    if (data_.empty())
        return;

    for (unsigned int row = 0; row < data_[0].size(); ++row)
    {
        for (unsigned int column = 0; column < data_.size(); ++column)
            std::cout << data_[column][row] << "\t";
        std::cout << std::endl;
    }
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
            int field;
            in >> field;
            data_[column][row] = field;
        }
}
