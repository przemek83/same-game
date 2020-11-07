#pragma once

#include <sstream>
#include <vector>

#include "Point.h"

class Board
{
public:
    Board(unsigned int columnsCount, unsigned int rowsCount, std::istream& in);

    static constexpr unsigned int EMPTY{0};

    std::string print() const;

    static bool isDescriptionValid(unsigned int colorCount);

    inline unsigned int getRowCount() const { return rowsCount_; };

    inline unsigned int getColumnCount() const { return columnsCount_; };

    inline int getColor(Point point) const
    {
        return data_[point.column][point.row];
    }

    inline void setColor(Point point, unsigned int color)
    {
        data_[point.column][point.row] = color;
    }

    inline void setEmpty(Point point)
    {
        data_[point.column][point.row] = EMPTY;
    }

    bool operator==(const Board& that) const;

    friend std::ostream& operator<<(std::ostream& os, const Board& board)
    {
        return os << board.print();
    }

private:
    void initData(unsigned int columnsCount, unsigned int rowsCount);

    void loadData(std::istream& in);

    const unsigned int columnsCount_;

    const unsigned int rowsCount_;

    std::vector<std::vector<unsigned int>> data_{};

    static constexpr unsigned int MIN_COLOR_COUNT{1};
};  // namespace Board
