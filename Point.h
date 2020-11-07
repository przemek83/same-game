#ifndef POINT_H
#define POINT_H

#include <climits>

struct Point
{
    unsigned int column{NOT_SET};
    unsigned int row{NOT_SET};

    static constexpr unsigned int NOT_SET{UINT_MAX};

    bool operator==(const Point& that) const;

    bool operator<(const Point& that) const;
};

#endif  // POINT_H
