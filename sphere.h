#ifndef SPHERE_H
#define SPHERE_H
#endif

#include "point.h"
#include "face4.h"
#include <vector>

class Sphere 
{
    private:
    
    unsigned radius;
    Point center;
    unsigned NM;
    unsigned NP;

    public:

    Sphere();
    Sphere(const unsigned);
    Sphere(const unsigned, const Point);

    void setCenter(const Point);
    Point getCenter();

    void setRadius(const unsigned);
    unsigned getRadius();

    void setMeridians(const unsigned);
    void setParralleles(const unsigned);

    unsigned getMeridians();
    unsigned getParralleles();

    std::vector<Point> drawPoints();
    std::vector<Face4> drawFaces();

};