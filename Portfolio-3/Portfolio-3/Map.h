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
	std::vector<Cell> cells;
	Graph roadMap;

	void createCells();
	void lineSweep();
	bool criticalPoint(Image* map, int posX, int posY, int target);
	void drawLineSweep(int posX, int posY, int scenario);
	void exitEntryPoints();

};

