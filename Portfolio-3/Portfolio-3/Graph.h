#pragma once
#include <iostream>
#include <vector>
#include <stack>
#define unknown 999999
#define inf 9999999

struct Coordinate
{
	Coordinate() {};

	Coordinate(int ix, int iy) {
		x = ix;
		y = iy;
	}

	bool operator==(Coordinate input) {
		if (x == input.x && y == input.y)
			return 1;
		return 0;
	};

	int x;
	int y;
};

struct Edge
{
	Edge(int iPointer, int iCost) {
		pointer = iPointer;
		cost = iCost;
	}

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
	void addEdge(Coordinate fromVertex, Coordinate toVertex, int cost);
	std::stack<Coordinate> getPath(Coordinate start, Coordinate goal);

private:
	std::vector<Vertex> vertices;
	std::vector<std::vector<int>> createDijkstraGraph();
	std::stack<Coordinate> findPathFromDijkstraGraph();

	int findIndexForVertex(Coordinate coordinate);

};

