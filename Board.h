#pragma once

#include <sstream>
#include <vector>

#include "Point.h"

class Board
{
public:
    Board(unsigned int columnsCount, unsigned int rowsCount, std::istream& in);

    static constexpr unsigned int MAX_W{500};
    static constexpr unsigned int MAX_H{500};

    void print();

    static bool isDescriptionValid(unsigned int rowCount,
                                   unsigned int columnCount,
                                   unsigned int colorCount);

    inline unsigned int getRowCount() const { return rowsCount_; };

    inline unsigned int getColumnCount() const { return columnsCount_; };

    inline int getColor(unsigned int column, unsigned int row) const
    {
        return data_[column][row];
    }

    inline int getColor(Point point) const
    {
        return getColor(point.column, point.row);
    }

    inline void setColor(unsigned int column, unsigned int row, int color)
    {
        data_[column][row] = color;
    };

    inline void setColor(Point point, int color)
    {
        setColor(point.column, point.row, color);
    };

    inline void setEmpty(unsigned int column, unsigned int row)
    {
        data_[column][row] = Point::EMPTY;
    };

    inline void setEmpty(Point point) { setEmpty(point.column, point.row); };

    bool operator==(const Board& that) const;

private:
    void initData(unsigned int columnsCount, unsigned int rowsCount);

    const unsigned int columnsCount_;

    const unsigned int rowsCount_;

    std::vector<std::vector<int>> data_{};

    static constexpr unsigned int MIN_W{4};
    static constexpr unsigned int MIN_H{4};
    static constexpr unsigned int MIN_C{3};
    static constexpr unsigned int MAX_C{20};
};  // namespace Board
