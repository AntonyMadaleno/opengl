#include "point.h"

Point::Point()
{
    (*this).x = 0;
    (*this).y = 0;
    (*this).z = 0;
}

Point::Point(const double x, const double y, const double z)
{
    (*this).x = x;
    (*this).y = y;
    (*this).z = z;
}
