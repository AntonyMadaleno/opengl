#ifndef REVOLUTION_H
#define REVOLUTION_H
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

class Revolution
{

//Z paramater will be ignored
//! Cone will use Revolution but you'd be able to get any revolution kind of object to be displayed trough this

    private:

    std::vector<Point> pts;
    unsigned NM;
    Point color;
    double angle;
    bool textured;
    std::vector<Point> texCoord;
    bool init;

    public:

    std::vector<Point> vbo;
    std::vector<Face3> faces;

    char* tex;

    Revolution();
    Revolution(const std::vector<Point>);

    void setTexture(char*); //no Get for this

    void setMeridians(const unsigned);
    unsigned getMeridians();

    void setColor(const Point);
    Point getColor();

    void setAngle(double);
    double getAngle();

    void drawPoints();
    void drawFaces();

    void GLDraw();

    std::vector<Point> getTexCoord();

};