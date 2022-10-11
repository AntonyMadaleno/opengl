#ifndef TORE_H
#define TORE_H
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

class Tore
{
    private:
    
    double irad;
    double orad;
    Point center;
    unsigned NS;
    unsigned NP;
    Point color;
    double angle;
    double erad;
    bool init;

    public:

    std::vector<Point> vbo;
    std::vector<Face3> faces;

    Tore();
    Tore(const double, const double);
    Tore(const double, const double, const Point);

    void setCenter(const Point);
    Point getCenter();

    void setColor(const Point);
    Point getColor();

    void setAngle(double);
    double getAngle();

    void setInnerRadius(const double);
    double getInnerRadius();

    void setOuterRadius(const double);
    double getOuterRadius();

    void setSegments(const unsigned);
    unsigned getSegments();

    void setParalleles(const unsigned);
    unsigned getParalleles();

    void drawPoints();
    void drawFaces();

    void GLDraw();

    //some optional function to make horn and that sort of stuff

    void setEndingRadius(const double);
    double getEndingRadius();


};