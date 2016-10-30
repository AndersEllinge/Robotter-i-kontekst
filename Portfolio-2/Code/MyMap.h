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

	/*
	creates a Potential Field with brush fire, the potential is seen as the difference from a pixel's value to neighboring pixel's value 
	*/
	void createBrushfirePotentialField();

	/*
	Creates a GVD map, and also makes a collision detection map, where a collision is shown as two colors bordering eachother.
	*/
	void createBrushfireCollisionDetection();

	/*
	This function is not used in the solution for this project.
	*/
	void siftGVD();

	/*
	This fucntion is not used in the solution for this project.
	*/
	void createGVD();
	
	/*
	This function finds the vertices from the GVDmap.
	*/
	void findVertices();

	/*
	This function finds the edges between the vertices.
	*/
	void findEdges();

	/*
	This function looks through the edges list and determines if the edge is allready in the list. Only interested in one edge pr. set of vertices.
	*/
	bool isEdgesInTheList(int id1, int id2);

	/*
	Prints the graph made from createGraph().
	*/
	void printGraph();

	/*
	This function creates, draw and print the path between two points.
	*/
	void createPath(int startX, int startY, int goalX, int goalY);
	
	/*
	This function creates the road map of the enviorment map.
	*/
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

	/*
	Returns true if just 1 of the neighbors in 4-way is equal to target.
	*/
	bool isNextTo4Way(Image* map, int posX, int posY, int target);

	/*
	Works like isNextTo4Way, but if determining which neighbors are equal to target is of relevence, these functions are needed.
	*/
	bool isNextTo4WayTop(Image* map, int posX, int posY, int target); 
	bool isNextTo4WayBottom(Image* map, int posX, int posY, int target);
	bool isNextTo4WayLeft(Image* map, int posX, int posY, int target);
	bool isNextTo4WayRight(Image* map, int posX, int posY, int target);
	
	/*
	Return true if just 1 of the neighbors in 8-way is equal to target
	*/
	bool isNextTo8Way(Image* map, int posX, int posY, int target);

	/*
	This function was the first try at determining wether a pixel is part of GVD or not out from the potentialfield. This is not a function that is used in the finished solution.
	*/
	bool isPartOfGVD(Image* map, int posX, int posY);

	/*
	Creates a graph of vertices connected to start with shortes distances. 
	*/
	void createGraph(int start);

	/*
	Checks if any obstacles are on the path.
	*/
	bool isPathClear(int startX, int startY, int goalX, int goalY);

	/*
	Finds the cloest valid vertex to a given point.
	*/
	int findClosestValidVertex(int posX, int posY);

	/*
	Used to find the index of a vertex, used in createGraph().
	*/
	int findIndexOfVertex(int vertex);

	/*
	Draws a line from a point to another.
	*/
	void drawStraightLine(int x0, int y0, int x1, int y1);

	/*
	Prints the path taken and draws it on the map.
	*/
	void printPath(int startX, int startY, int goalX, int goalY);


};

