#include "Graph.h"



Graph::Graph()
{
}


Graph::~Graph()
{
}

void Graph::addVertex(Coordinate coordinate)
{
	vertices.push_back(Vertex(coordinate));
}
