#ifndef POINT_H
#define POINT_H

struct Point
{
    int column{EMPTY};
    int row{EMPTY};

    static constexpr int EMPTY = -1;

    bool operator==(const Point& that) const;

    bool operator<(const Point& that) const;
};

#endif  // POINT_H
