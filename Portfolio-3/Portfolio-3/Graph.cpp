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

void Graph::addEdge(Coordinate fromVertex, Coordinate toVertex, int cost)
{
	vertices[findIndexForVertex(fromVertex)].adjecencyList.push_back(Edge(findIndexForVertex(toVertex), cost));
}

std::stack<Coordinate> Graph::getPath(Coordinate start, Coordinate goal)
{

	// 1) create dijkstra table
	// Init for dijkstra
	std::vector<std::vector<int>> dijkstraGraph;
	dijkstraGraph.resize(vertices.size());
	int vertexUnderInspection = findIndexForVertex(start);
	int numberOfUnknownsRemaining = dijkstraGraph.size();

	for (int i = 0; i < dijkstraGraph.size(); i++) {
		dijkstraGraph[i].resize(3);
		dijkstraGraph[i][0] = 0; // Known parameter of the Dijkstra graph
		dijkstraGraph[i][1] = inf; // Distance parameter of the Dijkstra graph
		dijkstraGraph[i][2] = vertexUnderInspection; // Previous parameter of the Dijkstra graph
	}

	dijkstraGraph[vertexUnderInspection][1] = 0; // Set distance of starting vertex to 0

	// Create table
	while (numberOfUnknownsRemaining) {

		// Find unknown vertex with shortest distance
		int prevDistance = inf;
		for (int i = 0; i < dijkstraGraph.size(); i++) {
			if (dijkstraGraph[i][1] < prevDistance) {
				vertexUnderInspection = i;
				prevDistance = dijkstraGraph[i][1];
			}
		}

		dijkstraGraph[vertexUnderInspection][0] = 1; // Set known
		numberOfUnknownsRemaining--; // Decrease total number of unknowns

		// Update according to adjecent vertices
		for (int i = 0; i < vertices[vertexUnderInspection].adjecencyList.size(); i++) {
			if (dijkstraGraph[vertexUnderInspection][1] + vertices[vertexUnderInspection].adjecencyList[i].cost < dijkstraGraph[vertexUnderInspection][1]) { // Is the distance to this vertex smaller? 
				dijkstraGraph[vertexUnderInspection][1] = dijkstraGraph[vertexUnderInspection][1] + vertices[vertexUnderInspection].adjecencyList[i].cost; // Update distance
				dijkstraGraph[vertexUnderInspection][0] = vertexUnderInspection; // Update pointer
			}
		}
	}


	// 2) find path from dijkstra table




	return std::stack<Coordinate>();
}

int Graph::findIndexForVertex(Coordinate coordinate)
{
	for (int i = 0; i < vertices.size(); i++) {
		if (coordinate == vertices[i].coordinate)
			return i;
	}
	std::cout << "Could not find vertex" << std::endl;
	return inf;
}
