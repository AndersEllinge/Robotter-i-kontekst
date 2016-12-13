#pragma once
#include <vector>
#include <stack>
#define unknown 999999

struct Coordinate
{
	Coordinate() {};

	Coordinate(int ix, int iy) {
		x = ix;
		y = iy;
	}

	int x;
	int y;
};

struct Edge
{
	int pointer;
	int cost;
};

struct Vertex
{
	Vertex(Coordinate iCoordinate) {
		cellKey = unknown;
		coordinate = iCoordinate;
		adjecencyList.clear();
	}

	Vertex(Coordinate iCoodinate, std::vector<Edge> iAdjecencyList) {
		cellKey = unknown;
		coordinate = iCoodinate;
		adjecencyList = iAdjecencyList;
	}

	Vertex(Coordinate iCoodinate, int iCellKey) {
		cellKey = iCellKey;
		coordinate = iCoodinate;
		adjecencyList.clear();
	}

	Vertex(Coordinate iCoodinate, int iCellKey, std::vector<Edge> iAdjecencyList) {
		cellKey = iCellKey;
		coordinate = iCoodinate;
		adjecencyList = iAdjecencyList;
	}

	int cellKey;
	Coordinate coordinate;
	std::vector<Edge> adjecencyList;
};

class Graph
{
public:
	Graph();
	~Graph();

	void addVertex(Coordinate coordinate);
	void addVertex(Coordinate coordinate, std::vector<Edge> adjecencyList);
	void addVertex(Coordinate coordinate, int cellKey);
	void addVertex(Coordinate coordinate, int cellKey, std::vector<Edge> adjecencyList);
	void addEdge();
	std::stack<Coordinate> getPath();

private:
	std::vector<Vertex> vertices;
	std::vector<std::vector<int>> createDijkstraGraph();
	std::stack<Coordinate> findPathFromDijkstraGraph();

};

