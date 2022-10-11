#include "face3.h"

Face3::Face3(const unsigned p0, const unsigned p1, const unsigned p2)
{
    (*this).p0 = p0;
    (*this).p1 = p1;
    (*this).p2 = p2;
}

unsigned* Face3::getPoints()
{
    unsigned * pts = new unsigned[3];
    pts[0] = (*this).p0;
    pts[1] = (*this).p1;
    pts[2] = (*this).p2;

    return pts;
}

unsigned Face3::getPoint(unsigned short n)
{
    if (n == 0) {return (*this).p0;}
    if (n == 1) {return (*this).p1;}
    if (n == 2) {return (*this).p2;}
    else {return 0;}
}
