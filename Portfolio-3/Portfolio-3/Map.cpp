#include "Map.h"



Map::Map(Image* inputMapObstacle, Image* inputMapItems)
{
	obstacleMap = inputMapObstacle;
	itemMap = inputMapItems;
	pathTakenMap = nullptr;
	cellDecompositionMap = nullptr;
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
