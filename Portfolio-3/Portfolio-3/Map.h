#pragma once
#include "Image.hpp"
#include "PPMLoader.hpp"
#include "Graph.h"


using namespace rw::sensor;
using namespace rw::loaders;

struct Cells
{
	Cells(Coordinate(int topLeftY, int topleftY), Coordinate( int bottomRightX, int bottomRightY) {
		cellCorners[0] = ;
		cellCorners[1] = ;
	}
	Coordinate cellCorners[2];
};

class Map
{
public:
	Map(Image* inputMapObstacle, Image* inputMapItems, int basex, int basey);
	
	void drawPathTaken();
	
	
	~Map();


private:
	Image* obstacleMap;
	Image* itemMap;
	Image* pathTakenMap;
	Image* cellDecompositionMap;
	std::vector<Cells*> cells;
	Graph roadMap;

	void lineSweep();
	void exitEntryPoints();

};

