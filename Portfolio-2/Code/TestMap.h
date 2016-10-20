#pragma once
#include <vector>
#include <queue>
#include <stack>
#include <iostream>
#include "Image.hpp"
#include "PPMLoader.hpp"

using namespace rw::sensor;
using namespace rw::loaders;

class TestMap
{
public:
	TestMap();
	TestMap(Image * inputMap);
	~TestMap();

	void printGraph();
	void createPath(int startX, int startY, int goalX, int goalY);
	void printPath();
	void makeDot();




private:
	void createGraph(int start);
	bool isPathClear(int startX, int startY, int goalX, int goalY);
	int findClosestValidVertex(int posX, int posY);
	int findIndexOfVertex(int vertex);
	void drawStraightLine(int x0, int y0, int x1, int y1);

	
	Image* map;
	Image* pathMap;
	std::vector<int> vertices;
	std::vector<std::vector<int>> edges;
	std::vector<std::vector<int>> graph;
	std::stack<std::vector<int>> path;

};

