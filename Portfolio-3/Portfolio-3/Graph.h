#pragma once
#include <vector>
#include <stack>

struct Coordinate
{
	int x;
	int y;
};

struct AdjecencyObject
{
	Vertex* pointer;
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

	void createGraph();
	void addVertex();
	std::stack<Coordinate> getPath();

private:
	std::vector<Vertex> vertices;


};

