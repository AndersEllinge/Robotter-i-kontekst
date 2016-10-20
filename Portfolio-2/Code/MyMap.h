#pragma once
#include "Image.hpp"
#include "PPMLoader.hpp"
#include <stack>
#include <queue>

using namespace rw::sensor;
using namespace rw::loaders;

class MyMap
{
public:
	MyMap();
	MyMap(Image* inputMap);
	~MyMap();

	void createBrushfirePotentialField();
	void createBrushfireCollisionDetection();
	void siftGVD();
	void createGVD();
	void findVertices();
	void findEdges();
	bool isEdgesInTheList(int id1, int id2);
	void printGraph();
	void createPath(int startX, int startY, int goalX, int goalY);
	
	void createRoadMap();


private:
	Image* map;
	Image* potentialFieldMap;
	Image* collisionDetectionMap;
	Image* gvdMap;
	Image* pathMap;
	Image* siftedMap;
	std::vector<int> vertices;
	std::vector<std::vector<int>> edges;
	std::vector<std::vector<int>> graph;
	std::stack<std::vector<int>> path;

	bool isNextTo4Way(Image* map, int posX, int posY, int target);

	bool isNextTo4WayTop(Image* map, int posX, int posY, int target);
	bool isNextTo4WayBottom(Image* map, int posX, int posY, int target);
	bool isNextTo4WayLeft(Image* map, int posX, int posY, int target);
	bool isNextTo4WayRight(Image* map, int posX, int posY, int target);
	
	bool isNextTo8Way(Image* map, int posX, int posY, int target);
	bool isPartOfGVD(Image* map, int posX, int posY);

	void createGraph(int start);
	bool isPathClear(int startX, int startY, int goalX, int goalY);
	int findClosestValidVertex(int posX, int posY);
	int findIndexOfVertex(int vertex);
	void drawStraightLine(int x0, int y0, int x1, int y1);
	void printPath(int startX, int startY, int goalX, int goalY);


};

