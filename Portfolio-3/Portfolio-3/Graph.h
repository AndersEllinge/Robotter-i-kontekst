#pragma once
#include <iostream>
#include <vector>
#include <stack>
#include <cmath>
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
	void addEdge(Coordinate fromVertex, Coordinate toVertex);

	std::vector<Vertex> getVertices();

	void updateCellKey(int index, int iCellKey);

	std::stack<Coordinate> getPath(Coordinate start, Coordinate goal);

	void printVertices();
	void printEdges();

private:
	std::vector<Vertex> vertices;

	int findIndexForVertex(Coordinate coordinate);
	int findClosestVertex(Coordinate coordinate);

};

