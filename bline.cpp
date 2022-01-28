/** Utilisation

//description de notre BLine
BLine t = BLine(5, 1);
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

#include "bline.h"
#include <cmath>
#include <iostream>
#include <vector>

BLine::BLine()
{
    (*this).R = 1;
    (*this).color = Point(1,1,1);
    (*this).DEF = 16;
    (*this).ER = (*this).R;
    (*this).textured = false;
}

BLine::BLine(const double r)
{
    (*this).R = r;
    (*this).color = Point(1,1,1);
    (*this).DEF = 16;
    (*this).ER = (*this).R;
    (*this).textured = false;
}

void BLine::setTexture(char* file)
{
    (*this).textured = true;
    (*this).tex = file;
}

void BLine::setColor(const Point c)
{
    (*this).color = c;
}
Point BLine::getColor()
{
    return (*this).color;
}

void BLine::setLine(std::vector<Point> pts)
{
    (*this).pts = pts;
}
std::vector<Point> BLine::getLine()
{
    return (*this).pts;
}

void BLine::setRadius(const double r)
{
    (*this).R = r;
}
double BLine::getRadius()
{
    return (*this).R;
}

void BLine::setDefinition(const unsigned n)
{
    (*this).DEF = n;
}
unsigned BLine::getDefinition()
{
    return (*this).DEF;
}

std::vector<Point> BLine::drawPoints()
{

    std::vector<Point> pts;
    int i = 0;

    for (Point p : (*this).pts)
    {

        double R = (*this).R * ( ((*this).pts.size() - 1) - i )/ ((*this).pts.size() - 1) + (*this).ER * i / (((*this).pts.size() - 1));

        for (double i = 0; i < (*this).DEF; i++)
        {
            double a = i*(2*M_PI/(*this).DEF);

            double z = cos(a) * R + p.z;
            double y = sin(a) * R + p.y;
            double x = p.x;

            pts.push_back(Point(x,y,z));

        }
        i++;
    }

    return pts;

}

std::vector<Face4> BLine::drawFaces()
{

    std::vector<Face4> faces;
    std::vector<Point> pts = (*this).drawPoints();

    for (unsigned m = 0; m < (*this).pts.size() -1; m++)
    {

        unsigned mn = (mn+1)%(*this).pts.size();

        for (unsigned i = 0; i < (*this).DEF; i++)
        {
            unsigned in = (i+1)%(*this).DEF;

            unsigned a = i + m*(*this).DEF;
            unsigned b = i + mn*(*this).DEF;
            unsigned c = in + mn*(*this).DEF;
            unsigned d = in + m*(*this).DEF;

            faces.push_back(Face4(a,b,c,d));

        }

    }

    return faces;

}

std::vector<Point> BLine::getTexCoord()
{

    std::vector<Point> pts;

    for (double i = 0; i < (*this).DEF; i++)
    {
        for (double j = 0; j < (*this).pts.size(); j++)
        {
            double x = (-i+double( (*this).DEF -1))/(double( (*this).DEF -1));
            double y = (-j+double( (*this).pts.size() )-1)/(double( (*this).pts.size() )-1);
            pts.push_back(Point(x,y,0));
        }
    }

    return pts;

}

void BLine::GLDraw()
{
    std::vector<Point> pts = (*this).drawPoints();
    std::vector<Face4> faces = (*this).drawFaces();
    std::vector<Point> texCoord;

    if ((*this).textured)
    {
        texCoord = (*this).getTexCoord();
    }

    for (Face4 f : faces)
    {
        glBegin(GL_POLYGON);
        for (short i=0;i<4;i++){
            Point p = pts[f.getPoint(i)];
            glVertex3f(p.x,p.y,p.z);
            if ((*this).textured)
            {
                double x,y;
                x = texCoord[f.getPoint(i)].x;
                y = texCoord[f.getPoint(i)].y;
                glTexCoord2f( x, y );
                
            }
            else
            {
                glColor3f((*this).color.x, (*this).color.y, (*this).color.z);
            }
        }
        glEnd();
    }
}

//The optional stuff

void BLine::setEndingRadius(const double r)
{
    (*this).ER = r;
}

double BLine::getEndingRadius()
{
    return (*this).ER;
}