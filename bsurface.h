#ifndef BSURFACE_H
#define BSURFACE_H
#endif

#include <cmath>
#include <vector>
#include <GL/glut.h>

#ifndef POINT_H
#include "point.h"
#endif

#ifndef FACE4_H
#include "face4.h"
#endif

#ifndef FACE3_H
#include "face3.h"
#endif

#ifndef BEZIER4_H
#include "bezier4.h"
#endif

class BSurface
{

    private:

    std::vector<std::vector<Point>> pts;
    unsigned DEF;
    Point color;
    bool textured;
    bool init;

    public:

    std::vector<Point> vbo;
    std::vector<Face3> faces;
    char* tex;

    BSurface();
    BSurface(const std::vector<std::vector<Point>>);

    std::vector<Point> getTexCoord();
    void setTexture(char*); //no Get for this

    void setDefinition(const unsigned);
    unsigned getDefinition();

    void setColor(const Point);
    Point getColor();

    void drawPoints();
    void drawFaces();

    void GLDraw();

};