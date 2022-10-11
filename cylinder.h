#ifndef CYLINDER_H
#define CYLINDER_H
#endif

#ifndef POINT_H
#include "point.h"
#endif
#ifndef FACE4_H
#include "face4.h"
#endif
#ifndef FACE3_H
#include "face3.h"
#endif
#include <vector>
#include <GL/glut.h>

class Cylinder 
{
    private:
    
    double radius;
    Point start;
    Point end;
    unsigned DEF;
    Point color;
    std::vector<Point> texCoord;
    bool init;

    public:

    std::vector<Point> vbo;
    std::vector<Face3> faces;

    Cylinder();
    Cylinder(const double);
    Cylinder(const double, const Point, const Point);

    void setStart(const Point);
    Point getStart();

    void setEnd(const Point);
    Point getEnd();

    void setColor(const Point);
    Point getColor();

    void setRadius(const double);
    double getRadius();

    void setDefinition(const unsigned);
    unsigned getDefinition();

    void drawPoints();
    void drawFaces();

    void GLDraw();

};