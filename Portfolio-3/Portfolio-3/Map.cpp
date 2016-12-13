#include "Map.h"
#include <iostream>


Map::Map(Image* inputMapObstacle, Image* inputMapItems, int basex, int basey)
{
	obstacleMap = inputMapObstacle;
	itemMap = inputMapItems;
	pathTakenMap = nullptr;
	cellDecompositionMap = nullptr;
	criticalPointMap = nullptr;
	roadMap.addVertex(Coordinate(basex, basey));
}

void Map::searchMap()
{
	lineSweep();
	createCells();
}

void Map::drawPathTaken()
{
	
}


Map::~Map()
{
}

void Map::createCells()
{
	if (cells.empty()) // vector is empty when creating the first cell
	{
		for (int x1 = 0; x1 < obstacleMap->getWidth(); x1++) { // x coordinate
			for (int y1 = 0; y1 < obstacleMap->getHeight(); y1++) { // y coordinate 
				if (criticalPointMap->getPixelValuei(x1, y1, 255)){ // check if we find a white pixel or line sweep
					for (size_t x2 = x1; x2 < obstacleMap->getWidth(); x2++){
						if (criticalPointMap->getPixelValuei(x2, y1, 0) || criticalPointMap->getPixelValuei(x2, y1, 100)){ // check if we hit obstacle or line sweep
							for (size_t y2 = y1; y2 < obstacleMap->getHeight(); y2++){									   // If we hit an obstacle or line sweep, we have the first coordinate to define a cell
								if (criticalPointMap->getPixelValuei(x2, y2, 0)){ // check if we hit obstacle or line sweep
									cells.push_back(Cell(Coordinate(x1, y1), Coordinate(x2, y2)));								   // If we hit an obstacle or line sweep, we have the second coordinate to define a cell
									break;
								}
							}
							if (!cells.empty()) // break if cell has been created
								break;
						}
					}
					if (!cells.empty()) // break if cell has been created
						break;
				}
			}
			if (!cells.empty()) // break if cell has been created
				break;
		}
	}



}

void Map::lineSweep()
{
	criticalPointMap = obstacleMap->copyFlip(0, 0);

	for (int x = 0; x < obstacleMap->getWidth(); x++) { // x coordinate
		for (int y = 0; y < obstacleMap->getHeight(); y++) { // y coordinate 
			if (criticalPoint(obstacleMap, x, y, 255)) // check if critical point
			{
				criticalPointMap->setPixel8U(x, y, 100);
			}
		}
	}
	criticalPointMap->saveAsPGM("criticalPointMap.pgm"); // Save output
}

bool Map::criticalPoint(Image* map, int posX, int posY, int target)
{
	if (posX != 0 && posX != (map->getWidth() - 1) && posY != 0 && posY != (map->getHeight() - 1) && map->getPixelValuei(posX, posY, 0) == 0) // test for borders if pixel is black
	{
		if (map->getPixelValuei(posX - 1, posY, 0) == target && map->getPixelValuei(posX, posY - 1, 0) == target) { // left and top is white - scenario 1
			drawLineSweep(posX, posY, 1);
			return 1;
		}

		if (map->getPixelValuei(posX - 1, posY, 0) == target && map->getPixelValuei(posX, posY + 1, 0) == target) { // left and bottom is white - scenario 2
			drawLineSweep(posX, posY, 2);
			return 1;
		}

		if (map->getPixelValuei(posX + 1, posY, 0) == target && map->getPixelValuei(posX, posY - 1, 0) == target) {// right and top is white - scenario 1
			drawLineSweep(posX, posY, 1);
			return 1;
		}

		if (map->getPixelValuei(posX + 1, posY, 0) == target && map->getPixelValuei(posX, posY + 1, 0) == target) { // right and bottom is white - scenario 2
			drawLineSweep(posX, posY, 2);
			return 1;
		}
	}

	return 0;
}

void Map::drawLineSweep(int posX, int posY, int scenario)
{
	if (scenario == 1) // scenario 1
	{
		for (size_t i = posY-1; i > 0; i--)
		{
			if (criticalPointMap->getPixelValuei(posX, i, 0) == 0)
			{
				break;
			}
			criticalPointMap->setPixel8U(posX, i, 100);
		}
	}
	if (scenario == 2) // scenario 2
	{
		for (size_t i = posY + 1; i < criticalPointMap->getHeight() - 1; i++)
		{
			if (criticalPointMap->getPixelValuei(posX, i, 0) == 0)
			{
				break;
			}
			criticalPointMap->setPixel8U(posX, i, 100);
		}
	}	
}

void Map::exitEntryPoints()
{
}
