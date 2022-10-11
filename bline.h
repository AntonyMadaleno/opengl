#ifndef BLINE_H
#define BLINE_H
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

class BLine
{
    private:
    
    double R, ER;
    Point color;
    unsigned DEF;
    std::vector<Point> pts;
    std::vector<Point> texCoord;
    bool textured;
    bool init;

    public:

    std::vector<Point> vbo;
    std::vector<Face3> faces;

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

    void drawPoints();
    void drawFaces();

    void GLDraw();

    std::vector<Point> getTexCoord();

    //some optional function to make horn and that sort of stuff

    void setEndingRadius(const double);
    double getEndingRadius();


};