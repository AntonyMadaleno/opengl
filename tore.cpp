/** Utilisation

//description de notre Tore
Tore t = Tore(5, 1);
t.setColor(Point(1,1,1));
t.setSegments(16);
t.setParalleles(8);
t.setAngle(90);
t.setEndingRadius(0);

//la partie GL
glPushMatrix();
glRotatef(90,1,0,0);
t.GLDraw();
glPopMatrix();

**/

#include "tore.h"
#include <cmath>
#include <iostream>
#include <vector>

Tore::Tore()
{
    (*this).irad = 1;
    (*this).orad = 0.1;
    (*this).center = Point(0,0,0);
    (*this).color = Point(1,1,1);
    (*this).NS = 12;
    (*this).NP = 8;
    (*this).angle = 2*M_PI;
    (*this).erad = (*this).orad;
    (*this).init = false;
}

Tore::Tore(const double ir, const double r)
{
    (*this).irad = ir;
    (*this).orad = r;
    (*this).center = Point(0,0,0);
    (*this).color = Point(1,1,1);
    (*this).NS = 12;
    (*this).NP = 8;
    (*this).angle = 2*M_PI;
    (*this).erad = (*this).orad;
    (*this).init = false;
}

Tore::Tore(const double ir, const double r, const Point c)
{
    (*this).irad = ir;
    (*this).orad = r;
    (*this).center = c;
    (*this).color = Point(1,1,1);
    (*this).NS = 12;
    (*this).NP = 8;
    (*this).angle = 2*M_PI;
    (*this).erad = (*this).orad;
    (*this).init = false;
}

void Tore::setCenter(const Point c)
{
    (*this).center = c;
}
Point Tore::getCenter()
{
    return (*this).center;
}

void Tore::setColor(const Point c)
{
    (*this).color = c;
}
Point Tore::getColor()
{   
    return (*this).color;
}

void Tore::setAngle(double a)
{
    while (a > 360)
    {
        a = a - 360;
    }
    while (a < 0)
    {
        a = a + 360;
    }
    (*this).angle = 2*M_PI*a/360;
    (*this).init = false;
}
double Tore::getAngle()
{   
    return (*this).angle*360/M_PI;
}

void Tore::setInnerRadius(const double r)
{
    (*this).irad = r;
    (*this).init = false;
}
double Tore::getInnerRadius()
{
    return (*this).irad;
}

void Tore::setOuterRadius(const double r)
{
    (*this).orad = r;
    (*this).init = false;
}
double Tore::getOuterRadius()
{
    return (*this).orad;
}

void Tore::setSegments(const unsigned n)
{
    (*this).NS = n;
    (*this).init = false;
}
unsigned Tore::getSegments()
{
    return (*this).NS;
}

void Tore::setParalleles(const unsigned n)
{
    (*this).NP = n;
    (*this).init = false;
}
unsigned Tore::getParalleles()
{
    return (*this).NP;
}

void Tore::drawPoints()
{

    std::vector<Point> pts;

    double R = (*this).irad;
    double r = (*this).orad - ( (*this).orad - (*this).erad );

    for (double i = 0; i < (*this).NS; i++)
    {

        double a = (i*(*this).angle)/(*this).NS;
        double r = (*this).orad - ( (*this).orad - (*this).erad )*i/(*this).NS;

        for(unsigned j = 0; j < (*this).NP; j++)
        {

            double b = j*(2*M_PI)/(*this).NP;

            double x = (*this).center.x + R*cos(a) + r*cos(b)*cos(a);
            double y = (*this).center.y + R*sin(a) + r*cos(b)*sin(a);
            double z = (*this).center.z + r*sin(b);

            pts.push_back(Point(x,y,z));

        }
    }

    (*this).vbo = pts;

}

void Tore::drawFaces()
{

    std::vector<Face3> faces;

    for (unsigned i = 0; i < (*this).NS; i++)
    {

        unsigned in = (i+1)%(*this).NS;
        if((*this).angle != 2*M_PI) 
        {
            if (in == 0) {in = i;}
        }

        for(unsigned j = 0; j < (*this).NP; j++)
        {

            unsigned jn = (j+1)%(*this).NP;

            unsigned a = i*(*this).NP + j;
            unsigned b = in*(*this).NP + j;
            unsigned c = in*(*this).NP + jn;
            unsigned d = i*(*this).NP + jn;

            faces.push_back(Face3(a,b,c));
            faces.push_back(Face3(c,d,a));

        }
    }

    (*this).faces = faces;

}

void Tore::GLDraw()
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

//The optional stuff

void Tore::setEndingRadius(const double r)
{
    (*this).erad = r;
    (*this).init = false;
}

double Tore::getEndingRadius()
{
    return (*this).erad;
    (*this).init = false;
}