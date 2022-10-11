#include "bsurface.h"

unsigned factoriel(double n)
{

    double r = 1;

    for (double i=2; i <= n; i++)
    {
        r = r*i;
    }

    return r;
}

double bin(double n, double i)
{
    return factoriel(n) / (factoriel(i) * factoriel(n-i));
}

double B(double n, double i, double u)
{
    return bin(n,i)*pow(u,i)*pow((1-u), n-i);
}

BSurface::BSurface()
{
    (*this).textured = false;
    (*this).init = false;
}

BSurface::BSurface(const std::vector<std::vector<Point>> pts)
{
    (*this).pts = pts;
    (*this).DEF = 16;
    (*this).color = Point(1,1,1);
    (*this).textured = false;
    (*this).init = false;

}

void BSurface::setTexture(char* file)
{
    (*this).textured = true;
    (*this).tex = file;
}

void BSurface::setDefinition(const unsigned n)
{
    (*this).DEF = n;
    (*this).init = false;
}

unsigned BSurface::getDefinition()
{
    return (*this).DEF;
}

void BSurface::setColor(const Point p)
{
    (*this).color = p;
    (*this).init = false;
}

Point BSurface::getColor()
{
    return (*this).color;
}

void BSurface::drawPoints()
{

    std::vector<Point> pts;
    int n,m;

    n = (*this).pts.size();
    m = (*this).pts[0].size();

    for (double U = 0; U < (*this).DEF; U++)
    {
        double t0 = U/(*this).DEF;
        for (double V = 0; V < (*this).DEF; V++)
        {
            double t1 = V/(*this).DEF;
            Point p = Point(0,0,0);

            for (double i = 1; i <= n; i++)
            {


                for (double j = 1; j <= m; j++)
                {

                    p.x += B(n,i,t0) * B(m,j,t1) * (*this).pts[i-1][j-1].x;
                    p.y += B(n,i,t0) * B(m,j,t1) * (*this).pts[i-1][j-1].y;
                    p.z += B(n,i,t0) * B(m,j,t1) * (*this).pts[i-1][j-1].z;

                }

            }

            pts.push_back(p);

        }

    }

    (*this).vbo = pts;

}

void BSurface::drawFaces()
{

    std::vector<Face3> faces;

    for (int i=0; i < (*this).DEF-1; i++)
    {

        for (int j=0; j < (*this).DEF-1; j++)
        {
            int a = j + i * (*this).DEF;
            int b = j + (i+1) * (*this).DEF;
            int c = j+1 + (i+1) * (*this).DEF;
            int d = j+1 + i * (*this).DEF;

            faces.push_back(Face3(a,b,c));
            faces.push_back(Face3(c,d,a));
        }

    }

    (*this).faces = faces;

}

std::vector<Point> BSurface::getTexCoord()
{

    std::vector<Point> pts;

    for (double i = 0; i < (*this).DEF; i++)
    {
        for (double j = 0; j < (*this).DEF; j++)
        {
            double x = (-i+double( (*this).DEF -1))/(double( (*this).DEF -1));
            double y = (-j+double( (*this).DEF )-1)/(double( (*this).DEF )-1);
            pts.push_back(Point(x,y,0));
        }
    }

    return pts;

}

void BSurface::GLDraw()
{

    if((*this).init == false)
    {
        (*this).drawPoints();
        (*this).drawFaces();
        (*this).init = true;
    }

    std::vector<Point> texCoord;

    if ((*this).textured)
    {
        texCoord = (*this).getTexCoord();
    }

    for (Face3 f : (*this).faces)
    {

        glBegin(GL_TRIANGLES);
        for (short i=0;i<3;i++){
            Point p = (*this).vbo[f.getPoint(i)];
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