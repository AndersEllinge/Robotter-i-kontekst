#pragma once
#include <vector>
#include <stack>

struct Coordinate
{
	Coordinate(int ix, int iy) {
		x = ix;
		y = iy;
	}

	int x;
	int y;
};

struct AdjecencyObject
{
	int pointer;
	int cost;
};

struct Vertex
{
	int cellKey;
	Coordinate coordinate;
	std::vector<AdjecencyObject> adjecencyList;
};

class Graph
{
public:
	Graph();
	~Graph();

	void addVertex(Coordinate coordinate);
	void addVertex(Coordinate coordinate, std::vector<AdjecencyObject> adjecencyList);
	void addVertex(Coordinate coordinate, int cellKey);
	void addVertex(Coordinate coordinate, int cellKey, std::vector<AdjecencyObject> adjecencyList);
	void addEdge();
	std::stack<Coordinate> getPath();

private:
	std::vector<Vertex> vertices;
	std::vector<std::vector<int>> createDijkstraGraph();
	std::stack<Coordinate> findPathFromDijkstraGraph();

};

