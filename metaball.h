#ifndef METABALL_H
#define METABALL_H
#endif

#ifndef POINT_H
#include "point.h"
#endif
#include <vector>

class Metaball
{
    private:
    
    double force;
    Point center; 

    public:

    Metaball();
    Metaball(const Point);
    Metaball(const double);
    Metaball(const Point, const double);

    void setCenter(const Point);
    Point getCenter();

    void setForce(const double);
    double getForce();

    double evaluate(Point);
};