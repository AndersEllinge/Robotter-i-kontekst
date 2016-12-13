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

void Graph::addVertex(Coordinate coordinate, std::vector<Edge> adjecencyList)
{
	vertices.push_back(Vertex(coordinate, adjecencyList));
}

void Graph::addVertex(Coordinate coordinate, int cellKey)
{
	vertices.push_back(Vertex(coordinate, cellKey));
}

void Graph::addVertex(Coordinate coordinate, int cellKey, std::vector<Edge> adjecencyList)
{
	vertices.push_back(Vertex(coordinate, cellKey, adjecencyList));
}
