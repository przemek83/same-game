#pragma once

#include <climits>

struct Point
{
    int column{NOT_SET};
    int row{NOT_SET};

    static constexpr int NOT_SET{INT_MAX};

    bool operator==(Point that) const;

    bool operator<(Point that) const;
};
