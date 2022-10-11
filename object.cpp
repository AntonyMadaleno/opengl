#include "object.h"
#include <cmath>
#include <iostream>
#include <vector>
#include <iostream>
#include <fstream>

#include <cstdlib>
#include "cstdio"

Object::Object()
{

}

Object::Object(std::vector<Point> pts, std::vector<Face3> faces)
{
	for (Point p : pts)
	{
		(*this).VBO.push_back([p.x,p.y,p.z]);
	}

	for (Face3 f: faces)
	{
		for (short i = 0; i < 3; i++)
		{
			(*this).IBO.push_back(f.getPoint(i));
		}
		
	}
}

void Object::setContent(std::vector<Point> pts, std::vector<Face3> faces)
{
	for (Point p : pts)
	{
		(*this).VBO.push_back([p.x,p.y,p.z]);
	}

	for (Face3 f: faces)
	{
		for (short i = 0; i < 3; i++)
		{
			(*this).IBO.push_back(f.getPoint(i));
		}
		
	}
}

void Object::SaveOBJ(const std::vector<Point> vertices, const std::vector<Face3> faces, const std::string filename)
{

	std::ofstream file;
	file.open("obj/" + filename);

	for (Point v : vertices)
	{
		file << "v " << v.x << " " << v.y << " " << v.z << std::endl;
	}

	for(Face3 f : faces)
	{
		file << "f " << f.getPoint(0) << " " << f.getPoint(1) << " " << f.getPoint(2) << std::endl;
	}

	file.close();

}