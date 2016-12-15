#pragma once
#include "Image.hpp"
#include "PPMLoader.hpp"
#include "Graph.h"

using namespace rw::sensor;
using namespace rw::loaders;

struct Cell
{
	Cell(Coordinate topLeft, Coordinate bottomRight) {
		cellCorners[0] = topLeft;
		cellCorners[1] = bottomRight;
	}
	Coordinate cellCorners[2];
};

class Map
{
public:
	Map(Image* inputMapObstacle, Image* inputMapItems, int basex, int basey);
	
	/* searches the map for items, then returns them one at a time */
	void searchMap();
	
	~Map();


private:
	/* creates all the cells */
	void createCells();

	/* draws a cell in a different color then white, so findCells can find new cells */
	void drawCell(Coordinate topLeft, Coordinate bottomRight, int color);

	/* Finds a new cell and pushes the cell onto the list of cells */
	void findCells();

	/* Returns true if there are more cells to create */
	bool isMoreCells();

	/* Makes a linesweep on the obstacle map, and creates entry/exit verticies on each side of each sweepline */
	void lineSweep();

	/* Determines and updates the vertices cellId attribute */
	void identifyCellsForVertices();

	/* links all the verticies in the same cell to each other */
	void linkVerticesInCells();

	/* Draws the path taken on a map */
	void drawPath(std::stack<Coordinate> path, Image* map);

	/* draws a straight line between 2 coordinates on a given map */
	void drawStraightLine(Coordinate c1, Coordinate c2, Image* map);

	/* Makes the robot cover the whole map */
	void mapCoverage();

	/* finds a path to the cellID given, and covers it in a up/down pattern starting at the left top corner */
	void cellCoverage(int cellId);

	/* Finds the cell a vertex is in and returns the cell identifier */
	int findCellFromCoordinate(Coordinate coordinate);

	/* Finds the closets vertex to the coordinate in the same cell */
	int findClosestVertex(Coordinate coordinate);

	/* This scans the surrounding pixels of the position for an item */
	void scanForTarget(Coordinate coordinate);

	/* This function collects the targets from its current posistion and then returns them to the base*/
	void collectTargets();
	
	/* This function determines if the position is a critical point */
	bool isCriticalPoint(int x, int y);
	
	
	Image* obstacleMap;
	Image* itemMap;
	Image* pathTakenMap;
	Image* cellDecompositionMap;
	Image* criticalPointMap;
	Image* pickUpMap;
	Coordinate robotPos;
	std::vector<Cell> cells;
	std::vector<Coordinate> targets;
	Graph roadMap;
	int distanceTraveled;
};

