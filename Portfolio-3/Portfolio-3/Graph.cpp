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

void Graph::addEdge(Coordinate fromVertex, Coordinate toVertex)
{
	int cost = sqrt(pow(abs(fromVertex.x - toVertex.x), 2) + pow(abs(fromVertex.y - toVertex.y), 2));

	vertices[findIndexForVertex(fromVertex)].adjecencyList.push_back(Edge(findIndexForVertex(toVertex), cost));
}

std::vector<Vertex> Graph::getVertices()
{
	return vertices;
}

void Graph::updateCellKey(int index, int iCellKey)
{
	vertices[index].cellKey = iCellKey;
}

std::stack<Coordinate> Graph::getPath(int start, int goal)
{

	// 1) Create dijkstra table
	// Init for dijkstra
	std::vector<std::vector<int>> dijkstraGraph;
	dijkstraGraph.resize(vertices.size());
	//int closestToStart = findClosestVertex(start);
	//int closestToGoal = findClosestVertex(goal);
	int vertexUnderInspection; // = findIndexForVertex(start);
	int numberOfUnknownsRemaining = dijkstraGraph.size();

	for (int i = 0; i < dijkstraGraph.size(); i++) {
		dijkstraGraph[i].resize(3);
		dijkstraGraph[i][0] = 0; // Known parameter of the Dijkstra graph
		dijkstraGraph[i][1] = inf; // Distance parameter of the Dijkstra graph
		dijkstraGraph[i][2] = start; // Previous parameter of the Dijkstra graph
	}

	dijkstraGraph[start][1] = 0; // Set distance of starting vertex to 0

	// Create table
	while (numberOfUnknownsRemaining) {

		// Find unknown vertex with shortest distance
		int prevDistance = inf;
		for (int i = 0; i < dijkstraGraph.size(); i++) {
			if (dijkstraGraph[i][1] < prevDistance && dijkstraGraph[i][0] == 0) {
				vertexUnderInspection = i;
				prevDistance = dijkstraGraph[i][1];
			}
		}

		dijkstraGraph[vertexUnderInspection][0] = 1; // Set known
		numberOfUnknownsRemaining--; // Decrease total number of unknowns

		// Update according to adjecent vertices
		for (int i = 0; i < vertices[vertexUnderInspection].adjecencyList.size(); i++) {
			int newCost = dijkstraGraph[vertexUnderInspection][1] + vertices[vertexUnderInspection].adjecencyList[i].cost;
			if (newCost < dijkstraGraph[vertices[vertexUnderInspection].adjecencyList[i].pointer][1]) { // Is the distance to this vertex smaller? 
				dijkstraGraph[vertices[vertexUnderInspection].adjecencyList[i].pointer][1] = newCost; // Update distance
				dijkstraGraph[vertices[vertexUnderInspection].adjecencyList[i].pointer][2] = vertexUnderInspection; // Update pointer
			}
		}
	}


	
	// 2) Find path from dijkstra table
	std::stack<Coordinate> path;
	int vertexForPath = goal;

	//path.push(goal); // Push goal coordinate onto path

	while (dijkstraGraph[vertexForPath][1] != 0) {
		path.push(vertices[vertexForPath].coordinate); // Push vertex coordinate onto path
		vertexForPath = dijkstraGraph[vertexForPath][2]; // Set vertex to next vertex in path
	}

	path.push(vertices[vertexForPath].coordinate); // Push starting vertex coordinate
	//path.push(start);

	return path;
}

void Graph::printVertices()
{
	for (int i = 0; i < vertices.size(); i++) {
		std::cout << i << " " << vertices[i].coordinate.x << " " << vertices[i].coordinate.y << " " << vertices[i].cellKey << std::endl;
	}
}

void Graph::printEdges()
{
	for (int i = 0; i < vertices.size(); i++) {
		for (int j = 0; j < vertices[i].adjecencyList.size(); j++) {
			std::cout << i << " " << vertices[i].adjecencyList[j].pointer << " " << vertices[i].adjecencyList[j].cost << "; ";
		}
		std::cout << std::endl;
	}
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

int Graph::findClosestVertex(Coordinate coordinate)
{
	double prevClosest = inf;
	int closestVertex = unknown;
	for (int i = 0; i < vertices.size(); i++) {
		double dist = sqrt(pow(abs(coordinate.x - vertices[i].coordinate.x), 2) + pow(abs(coordinate.y - vertices[i].coordinate.y), 2));
		//std::cout << dist << " " << prevClosest << std::endl;
		if (dist < prevClosest) {
			prevClosest = dist;
			closestVertex = i;
		}
	}
	return closestVertex;
}
