#pragma once

#include <string>
#include <vector>

#include "Point.h"

class Board
{
public:
    Board(int columnsCount, int rowsCount, std::istream& in);

    static constexpr int EMPTY{0};

    std::string print() const;

    static bool isDescriptionValid(int colorCount);

    int getRowCount() const { return rowsCount_; }

    int getColumnCount() const { return columnsCount_; }

    int getColor(Point point) const
    {
        return data_[static_cast<std::size_t>(point.column)]
                    [static_cast<std::size_t>(point.row)];
    }

    void setColor(Point point, int color) { setElement(point, color); }

    void setEmpty(Point point) { setElement(point, EMPTY); }

    friend bool operator==(const Board& lhs, const Board& rhs)
    {
        return (lhs.columnsCount_ == rhs.columnsCount_) &&
               (lhs.rowsCount_ == rhs.rowsCount_) && (lhs.data_ == rhs.data_);
    }

    friend std::ostream& operator<<(std::ostream& os, const Board& board)
    {
        return os << board.print();
    }

private:
    void setElement(Point point, int color)
    {
        data_[static_cast<std::size_t>(point.column)]
             [static_cast<std::size_t>(point.row)] = color;
    }

    void initData();

    void loadData(std::istream& in);

    const int columnsCount_;

    const int rowsCount_;

    std::vector<std::vector<int>> data_;

    static constexpr int MIN_COLOR_COUNT{1};
};  // namespace Board
