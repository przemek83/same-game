#include "Point.h"

bool Point::operator==(Point that) const
{
    return (column_ == that.column_) && (row_ == that.row_);
}

bool Point::operator<(Point that) const
{
    if (column_ < that.column_)
        return false;
    return row_ < that.row_;
}
