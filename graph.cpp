#include "graph.h"

Graph::Graph(const std::string filename, const std::string ext)
{
	std::ifstream indata;
	unsigned from;
	unsigned to;

	indata.open(filename + "." + ext);

	if(!indata) {std::cout << "cant open the file"; exit(1);}

	while ( !indata.eof() ) 
	{
		indata >> from;
      	indata >> to;

   	}

   	indata.close();

}