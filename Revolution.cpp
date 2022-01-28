/** Utilisation

    Revolution rbez = Revolution(dots); //dots est un std::vector<Point>
    rbez.setMeridians(12);
    rbez.setAngle(180);
    rbez.GLDraw();

**/


#include "Revolution.h"

Revolution::Revolution()
{
    (*this).textured = false;
}

Revolution::Revolution(const std::vector<Point> pts)
{
    (*this).pts = pts;
    (*this).NM = 24;
    (*this).color = Point(1,1,1);
    (*this).angle = 2*M_PI;
    (*this).textured = false;
}

void Revolution::setTexture(char* file)
{
    (*this).textured = true;
    (*this).tex = file;
}

void Revolution::setMeridians(const unsigned n)
{
    (*this).NM = n;
}

unsigned Revolution::getMeridians()
{
    return (*this).NM;
}

void Revolution::setColor(const Point p)
{
    (*this).color = p;
}

Point Revolution::getColor()
{
    return (*this).color;
}

void Revolution::setAngle(double a)
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
}

double Revolution::getAngle()
{   
    return (*this).angle*360/M_PI;
}

std::vector<Point> Revolution::drawPoints()
{

    std::vector<Point> pts;

    for (unsigned m = 0; m < (*this).NM; m++)
    {
        double a = m*(*this).angle/(*this).NM;

        for (Point p : (*this).pts)
        {

            double x = p.x * cos(a);
            double y = p.y;
            double z = p.x * sin(a);

            pts.push_back(Point(x,y,z));

        } 
    }

    return pts;

}

std::vector<Face4> Revolution::drawFaces()
{

    std::vector<Face4> faces;

    for (unsigned m = 0; m < (*this).NM; m++)
    {

        unsigned mn = (mn+1)%(*this).NM;

        for (unsigned i = 0; i < (*this).pts.size(); i++)
        {
            unsigned in = (i+1)% (*this).pts.size();

            unsigned a = i + m*(*this).pts.size();
            unsigned b = i + mn*(*this).pts.size();
            unsigned c = in + mn*(*this).pts.size();
            unsigned d = in + m*(*this).pts.size();

            faces.push_back(Face4(a,b,c,d));

        }
    }

    return faces;

}

std::vector<Point> Revolution::getTexCoord()
{

    std::vector<Point> pts;

    for (double i = 0; i < (*this).NM; i++)
    {
        for (double j = 0; j < (*this).pts.size(); j++)
        {
            double y = (-i+double( (*this).NM -1))/(double( (*this).NM -1));
            double x = (-j+double( (*this).pts.size() )-1)/(double( (*this).pts.size() )-1);
            pts.push_back(Point(x,y,0));
        }
    }

    return pts;

}


void Revolution::GLDraw()
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
            Point p = pts[f.getPoint(i)];
            glVertex3f(p.x,p.y,p.z);
        }
        glEnd();
    }

}