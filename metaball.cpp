#include "metaball.h"
#include <cmath>
#include <iostream>
#include <vector>

Metaball::Metaball()
{
	(*this).force = 1;
	(*this).center = Point(0,0,0);
}

Metaball::Metaball(const Point c)
{
	(*this).force = 1;
	(*this).center = c;
}

Metaball::Metaball(const double f)
{
	(*this).force = f;
	(*this).center = Point(0,0,0);
}

Metaball::Metaball(const Point c, const double f)
{
	(*this).force = f;
	(*this).center = c;
}

void Metaball::setCenter(const Point c)
{
    (*this).center = c;
}
Point Metaball::getCenter()
{
    return (*this).center;
}

void Metaball::setForce(const double f)
{
    (*this).force = f;
}
double Metaball::getForce()
{
    return (*this).force;
}

double Metaball::evaluate(Point p)
{
	double dx, dy, dz;
	dx = (p.x - (*this).center.x) / (*this).force;
	dy = (p.y - (*this).center.y) / (*this).force;
	dz = (p.z - (*this).center.z) / (*this).force;

	double r = 1 / std::sqrt( dx*dx + dy*dy + dz*dz );

	

	if ((*this).force < 0)
		return -r;
	else
		return r;

}