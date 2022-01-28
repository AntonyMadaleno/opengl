#ifndef BLINE_H
#define BLINE_H
#endif

#ifndef POINT_H
#include "point.h"
#endif
#ifndef FACE4_H
#include "face4.h"
#endif
#include <vector>
#include <GL/glut.h>

class BLine
{
    private:
    
    double R, ER;
    Point color;
    unsigned DEF;
    std::vector<Point> pts;
    bool textured;

    public:

    char* tex;

    BLine();
    BLine(const double);

    void setTexture(char*); //no Get for this

    void setColor(const Point);
    Point getColor();

    void setLine(std::vector<Point>);
    std::vector<Point> getLine();

    void setRadius(const double);
    double getRadius();

    void setDefinition(const unsigned);
    unsigned getDefinition();

    std::vector<Point> drawPoints();
    std::vector<Face4> drawFaces();

    void GLDraw();

    std::vector<Point> getTexCoord();

    //some optional function to make horn and that sort of stuff

    void setEndingRadius(const double);
    double getEndingRadius();


};