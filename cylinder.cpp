/** Utilisation

Cylinder j0 = Cylinder(0.1, Point(0,0,0), Point(0,5,0));
j0.setColor(Point(0.7,0.7,0.7));
j0.setDefinition(32);
j0.GLDraw(); 

**/

#include "cylinder.h"
#include <cmath>
#include <iostream>
#include <vector>

Cylinder::Cylinder()
{
    (*this).DEF = 6;
    (*this).start = Point(0,-1,0);
    (*this).end = Point(0,1,0);
    (*this).radius = 1.0;
    (*this).color = Point(1,1,1);
    (*this).init = false;
}

Cylinder::Cylinder(const double r)
{
    (*this).DEF = 6;
    (*this).start = Point(0,-1,0);
    (*this).end = Point(0,1,0);
    (*this).radius = r;
    (*this).color = Point(1,1,1);
    (*this).init = false;
}

Cylinder::Cylinder(const double r, const Point a, const Point b)
{
    (*this).DEF = 6;
    (*this).start = a;
    (*this).end = b;
    (*this).radius = r;
    (*this).color = Point(1,1,1);
    (*this).init = false;
}

void Cylinder::setColor(const Point c)
{
    (*this).color = c;
    (*this).init = false;
}

Point Cylinder::getColor()
{
    return (*this).color;
}

void Cylinder::setRadius(const double r)
{
    (*this).radius = r;
    (*this).init = false;
}

double Cylinder::getRadius()
{
    return (*this).radius;
}

void Cylinder::setDefinition(const unsigned n)
{
    (*this).DEF = n;
    (*this).init = false;
}

unsigned Cylinder::getDefinition()
{
    return (*this).DEF;
}

void Cylinder::drawPoints()
{

    std::vector<Point> pts;

    for (unsigned i = 0; i<(*this).DEF; i++)
    {
        double alpha = i*(2*M_PI)/(*this).DEF;

        double lx = (*this).start.x + (*this).radius * cos(alpha);
        double ly = (*this).start.y + (*this).radius * 0;
        double lz = (*this).start.z + (*this).radius * sin(alpha);
        Point lower = Point(lx, ly, lz);
        pts.push_back(lower);

        double ux = (*this).end.x + (*this).radius * cos(alpha);
        double uy = (*this).end.y + (*this).radius * 0;
        double uz = (*this).end.z + (*this).radius * sin(alpha);
        Point upper = Point(ux, uy, uz);
        pts.push_back(upper);

    }

    (*this).vbo = pts;

}

void Cylinder::drawFaces()
{
    std::vector<Face3> faces;

    for (unsigned i = 0; i < (*this).DEF; i++)
    {
        unsigned a = i*2;
        unsigned b = 2 * ( (i+1)%(*this).DEF );
        unsigned c = b+1;
        unsigned d = a+1;

        faces.push_back(Face3(a,b,c));
        faces.push_back(Face3(c,d,a));
    }

    (*this).faces = faces;
}

void Cylinder::GLDraw()
{

     if((*this).init == false)
    {
        (*this).drawPoints();
        (*this).drawFaces();
        (*this).init = true;
    }

    for (Face3 f : (*this).faces)
    {
        glBegin(GL_TRIANGLES);
        for (short i=0;i<3;i++){
            Point p = (*this).vbo[f.getPoint(i)];
            glColor3f((*this).color.x, (*this).color.y, (*this).color.z);
            glVertex3f(p.x,p.y,p.z);
        }
        glEnd();
    }
}
