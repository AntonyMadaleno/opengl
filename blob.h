#ifndef BLOB_H
#define BLOB_H
#endif

#ifndef POINT_H
#include "point.h"
#endif
#ifndef FACE3_H
#include "face3.h"
#endif
#include <vector>
#include <GL/glut.h>
#ifndef METABALL_H
#include "metaball.h"
#endif


class Blob
{
    private:
    
    std::vector<Metaball> balls;
    Point color;
    bool init;
    int precision;

    public:

    std::vector<Point> vbo;
    std::vector<Face3> faces;

    Blob();

    void addMetaball(const Metaball);
    void removeMetaball(const int);
    std::vector<Metaball> getMetaballs();

    void setColor(const Point);
    Point getColor();

    void setPrecision(const int);
    int getPrecision();

    void draw(const Point, const Point);
    void GLDraw();

};