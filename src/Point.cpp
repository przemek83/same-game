#include "Point.h"

bool Point::operator==(Point that) const
{
    return (column == that.column) && (row == that.row);
}

bool Point::operator<(Point that) const
{
    if (column < that.column)
        return false;
    return row < that.row;
}
