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

    inline int getColor(Point point) const
    {
        return data_[point.column][point.row];
    }

    inline void setColor(Point point, int color)
    {
        data_[point.column][point.row] = color;
    }

    inline void setEmpty(Point point)
    {
        data_[point.column][point.row] = Point::EMPTY;
    }

    bool operator==(const Board& that) const;

private:
    void initData(unsigned int columnsCount, unsigned int rowsCount);

    void loadData(std::istream& in);

    const unsigned int columnsCount_;

    const unsigned int rowsCount_;

    std::vector<std::vector<int>> data_{};

    static constexpr unsigned int MIN_W{4};
    static constexpr unsigned int MIN_H{4};
    static constexpr unsigned int MIN_C{3};
    static constexpr unsigned int MAX_C{20};
};  // namespace Board
