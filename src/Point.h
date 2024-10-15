#pragma once

#include <climits>

struct Point
{
    int column_{NOT_SET};
    int row_{NOT_SET};

    static constexpr int NOT_SET{INT_MAX};

    friend bool operator==(const Point& lhs, const Point& rhs)
    {
        return (lhs.column_ == rhs.column_) && (lhs.row_ == rhs.row_);
    }

    friend bool operator<(const Point& lhs, const Point& rhs)
    {
        if (lhs.column_ < rhs.column_)
            return false;
        return lhs.row_ < rhs.row_;
    }
};
