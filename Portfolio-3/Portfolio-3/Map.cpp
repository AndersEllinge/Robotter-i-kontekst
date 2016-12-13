#include "Map.h"



Map::Map(Image* inputMapObstacle, Image* inputMapItems)
{
	obstacleMap = inputMapObstacle;
	itemMap = inputMapItems;
	pathTakenMap = nullptr;
}


Map::~Map()
{
}

void Map::lineSweep()
{

}
