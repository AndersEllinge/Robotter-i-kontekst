#pragma once
#include <vector>
#include <queue>
#include <stack>
#include <iostream>

class TestMap
{
public:
	TestMap();
	~TestMap();

	void createGraph(int start);
	void printGraph();

	int findClosestValidVertex(int posX, int posY);

	void createPath(int startX, int startY, int goalX, int goalY);
	void printPath();


	std::vector<std::vector<int>> vertices;
	std::vector<std::vector<int>> edges;
	std::vector<std::vector<int>> graph;
	std::stack<std::vector<int>> path;

private:
	
	

};

