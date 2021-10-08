#include "sphere.h"
#include <math.h>
#include <iostream>
#include <vector>

Sphere::Sphere()
{
    (*this).radius = 1;
    (*this).NM = 6;
    (*this).NP = 4;
    (*this).center = {0,0,0};
}

Sphere::Sphere(const unsigned r)
{
    (*this).radius = r;
    (*this).NM = 6;
    (*this).NP = 4;
    (*this).center = {0,0,0};
}

Sphere::Sphere(const unsigned r,const Point c)
{
    (*this).radius = r;
    (*this).NM = 6;
    (*this).NP = 4;
    (*this).center = c;
}

void Sphere::setCenter(const Point c)
{
    (*this).center = c;
}

Point Sphere::getCenter()
{
    return (*this).center;
}

void Sphere::setRadius(const unsigned r)
{
    (*this).radius = r;
}

unsigned Sphere::getRadius()
{
    return (*this).radius;
}

void Sphere::setMeridians(const unsigned n)
{
    (*this).NM = n;
}
void Sphere::setParralleles(const unsigned n)
{
    (*this).NP = n;
}

unsigned Sphere::getMeridians()
{
    return (*this).NM;
}
unsigned Sphere::getParralleles()
{
    return (*this).NP;
}

std::vector<Point> Sphere::drawPoints()
{
    //Point * pts = (Point*) malloc((*this).NM*(*this).NP);
    std::vector<Point> pts;

    for (unsigned i = 0; i < (*this).NM; i++)
    {
        for (unsigned j = 0; j < (*this).NP; j++)
        {
            double alpha = i*2*M_PI/(*this).NM;
            double teta = j*M_PI/( (*this).NP-1) - M_PI/2;
            double x = (*this).center.x + (*this).radius*cos(alpha)*cos(teta);
            double y = (*this).center.y + (*this).radius*sin(alpha)*cos(teta);
            double z = (*this).center.z + (*this).radius*sin(teta);
            pts.push_back(Point(x,y,z));
        }
    }

    return pts;

}

std::vector<Face4> Sphere::drawFaces()
{
    //Face4 * faces = (Face4*) malloc((*this).NM*(*this).NP - (*this).NM);
    std::vector<Face4> faces;

    for (unsigned i = 0; i < (*this).NM; i++)
    {
        for (unsigned j = 0; j < (*this).NP-1; j++)
        {

            unsigned in = (i+1)%(*this).NM;
            unsigned jn = (j+1)%(*this).NP;

            unsigned p0 = i*(*this).NP + j;
            unsigned p1 = in*(*this).NP + j;
            unsigned p2 = in*(*this).NP + jn;
            unsigned p3 = i*(*this).NP + jn;
            faces.push_back(Face4(p0, p1, p2, p3));
        }
    }

    return faces;
}