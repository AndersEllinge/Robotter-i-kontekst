#include "Map.h"



Map::Map(Image* inputMapObstacle, Image* inputMapItems, int basex, int basey)
{
	obstacleMap = inputMapObstacle;
	itemMap = inputMapItems;
	pathTakenMap = nullptr;
	cellDecompositionMap = nullptr;
	roadMap.addVertex(Coordinate(basex, basey));
}

void Map::drawPathTaken()
{
}


Map::~Map()
{
}

void Map::lineSweep()
{

}

void Map::exitEntryPoints()
{
}
