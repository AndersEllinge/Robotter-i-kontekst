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
	
	void searchMap();

	void drawPathTaken();
	
	~Map();


private:
	Image* obstacleMap;
	Image* itemMap;
	Image* pathTakenMap;
	Image* cellDecompositionMap;
	Image* criticalPointMap;
	Coordinate robotPos;
	std::vector<Cell> cells;
	std::vector<Coordinate> targets;
	Graph roadMap;
	int distanceTraveled;

	void createCells();
	void lineSweep();
	bool criticalPoint(Image* map, int posX, int posY, int target);
	void drawLineSweep(int posX, int posY, int scenario);
	void drawCell(Coordinate topLeft, Coordinate bottomRight, int color);
	void findCells();
	bool isMoreCells();
	void lineSweep2();
	void identifyCellsForVertices();
	void linkVerticesInCells();

	void drawPath(std::stack<Coordinate> path);
	void drawStraightLine(Coordinate c1, Coordinate c2);

	void mapCoverage();
	void cellCoverage(int cellId);

	int findCellFromCoordinate(Coordinate coordinate);

	int findClosestVertex(Coordinate coordinate);

	void scanForTarget(Coordinate position);
	void collectTargets();

	bool isCriticalPoint(int x, int y);

};

