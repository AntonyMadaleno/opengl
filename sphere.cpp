/** Utilisation

	Sphere sun = Sphere(1);
	sun.setColor(Point(1,0.75,0));
	sun.setMeridians(60);
	sun.setParralleles(40);
	sun.GLDraw();

**/

#include "sphere.h"
#include <cmath>
#include <iostream>
#include <vector>

#include <cstdlib>
#include "cstdio"
#include "jpeglib.h"
#include <jerror.h>

Sphere::Sphere()
{
    (*this).radius = 1;
    (*this).NM = 32;
    (*this).NP = 30;
    (*this).center = {0,0,0};
    (*this).color = Point(1,1,1); //this a color
    (*this).textured = false;
    (*this).init = false;
}

Sphere::Sphere(const double r)
{
    (*this).radius = r;
    (*this).NM = 32;
    (*this).NP = 30;
    (*this).center = {0,0,0};
    (*this).color = Point(1,1,1);
    (*this).textured = false;
    (*this).init = false;
}

Sphere::Sphere(const double r,const Point c)
{
    (*this).radius = r;
    (*this).NM = 32;
    (*this).NP = 30;
    (*this).center = c;
    (*this).color = Point(1,1,1);
    (*this).textured = false;
    (*this).init = false;
}

void Sphere::setTexture(char* file)
{
    (*this).textured = true;
    (*this).tex = file;
    (*this).init = false;
}

void Sphere::setCenter(const Point c)
{
    (*this).center = c;
    (*this).init = false;
}

Point Sphere::getCenter()
{
    return (*this).center;
}

void Sphere::setColor(const Point c)
{
    (*this).color = c;
    (*this).init = false;
}

Point Sphere::getColor()
{
    return (*this).color;
}

void Sphere::setRadius(const double r)
{
    (*this).radius = r;
    (*this).init = false;
}

double Sphere::getRadius()
{
    return (*this).radius;
}

void Sphere::setMeridians(const unsigned n)
{
    (*this).NM = n;
    (*this).init = false;
}
void Sphere::setParalleles(const unsigned n)
{
    (*this).NP = n;
    (*this).init = false;
}

unsigned Sphere::getMeridians()
{
    return (*this).NM;
}
unsigned Sphere::getParalleles()
{
    return (*this).NP;
}

void Sphere::drawPoints()
{
    std::vector<Point> pts;

    //TOP OF THE SPHERE
    double alpha = 0;
    double teta = -M_PI/2;

    double x = (*this).center.x + (*this).radius*cos(alpha)*cos(teta);
    double z = (*this).center.z + (*this).radius*sin(alpha)*cos(teta);
    double y = (*this).center.y + (*this).radius*sin(teta);

    pts.push_back(Point(x,y,z));

    //BOTTOM OF THE SPHERE
    teta = M_PI/2;

    x = (*this).center.x + (*this).radius*cos(alpha)*cos(teta);
    z = (*this).center.z + (*this).radius*sin(alpha)*cos(teta);
    y = (*this).center.y + (*this).radius*sin(teta);
            
    pts.push_back(Point(x,y,z));

    //SPHERE PARRALELLES
    for (unsigned i = 1; i < (*this).NP - 1; i++)
    {
        for (unsigned j = 0; j < (*this).NM; j++)
        {
            alpha = j*2*M_PI/((*this).NM-1);
            teta = i*M_PI/( (*this).NP-1) - M_PI/2;

            x = (*this).center.x + (*this).radius*cos(alpha)*cos(teta);
            z = (*this).center.z + (*this).radius*sin(alpha)*cos(teta);
            y = (*this).center.y + (*this).radius*sin(teta);
            
            pts.push_back(Point(x,y,z));
        }
    }

    (*this).vbo = pts;

}

void Sphere::drawFaces()
{
    std::vector<Face3> faces;

    //DRAW TOP
    for (unsigned j = 0; j < (*this).NM; j++)
    {
        unsigned jn = (j+1)%(*this).NM;

        unsigned p1 = j + 2;
        unsigned p2 = jn + 2;
        faces.push_back(Face3(0, p1, p2));
    }
            
    //DRAW MIDDLE
    for (unsigned i = 0; i < (*this).NP-3; i++)
    {
        for (unsigned j = 0; j < (*this).NM; j++)
        {
            unsigned in = (i+1)%(*this).NP;
            unsigned jn = (j+1)%(*this).NM;

            unsigned p0 = i*(*this).NM + j + 2;
            unsigned p1 = i*(*this).NM + jn + 2;
            unsigned p2 = in*(*this).NM + jn + 2;
            unsigned p3 = in*(*this).NM + j + 2;
            faces.push_back(Face3(p0, p1, p2));
            faces.push_back(Face3(p2, p3, p0));
        }
    }


    //DRAW BOTTOM
    for (unsigned j = 0; j < (*this).NM; j++)
    {
        unsigned jn = (j+1)%(*this).NM;

        unsigned p1 = ((*this).NP-3) *(*this).NM  + j + 2;
        unsigned p2 = ((*this).NP-3) *(*this).NM  + jn + 2;
        faces.push_back(Face3(1, p1, p2));
    }

    (*this).faces = faces;
}

std::vector<Point> Sphere::getTexCoord()
{

    std::vector<Point> pts;
    double x, y;

    pts.push_back(Point(0.5,1,0));
    pts.push_back(Point(0.5,0,0));

    for (double i = (*this).NM - 1; i >= 2; i--)
    {
        for (double j = (*this).NP - 1; j >= 0; j--)
        {
            //double x = (-j+double( (*this).NM ))/(double( (*this).NM ));
            //double y = (-i+double( (*this).NP )-1)/(double( (*this).NP )-1);
            x = j / ( (*this).NM);
            y = i / (*this).NP;
            pts.push_back(Point(x,y,0));
        }
    }

    return pts;

}

void Sphere::GLDraw()
{

    if((*this).init == false)
    {
        (*this).drawPoints();
        (*this).drawFaces();
        (*this).init = true;

        if ((*this).textured)
        {
            (*this).texCoord = (*this).getTexCoord();
        }
    }
    

    for (Face3 f : (*this).faces)
    {
        glBegin(GL_TRIANGLES);
        for (short i=0;i<3;i++){
            Point p = (*this).vbo[f.getPoint(i)];
            if ((*this).textured)
            {
                double x,y;
                unsigned index = f.getPoint(i);

                    x = texCoord[index].x;
                    y = texCoord[index].y;
                    glTexCoord2f( x, y );
                
            }
            else
            {
                glColor3f((*this).color.x, (*this).color.y, (*this).color.z);
            }
            
            glVertex3f(p.x,p.y,p.z);
        }
        glEnd();
    }
}
