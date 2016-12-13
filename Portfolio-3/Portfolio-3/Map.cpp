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
	cellDecompositionMap = criticalPointMap->copyFlip(0, 0);

	//findFirstCell();
	
	

	bool moreCells = 1;
	int color = 20;
	while (moreCells)
	{
		moreCells = 0;
		findCells();
		drawCell(cells[cells.size()-1].cellCorners[0], cells[cells.size()-1].cellCorners[1], color);
		moreCells = isMoreCells();
		color = color + 20;

	}

	cellDecompositionMap->saveAsPGM("cellDecompositionMap.pgm"); // Save output
}

void Map::lineSweep()
{
	criticalPointMap = obstacleMap->copyFlip(0, 0);

	for (int x = 0; x < obstacleMap->getWidth(); x++) { // x coordinate
		for (int y = 0; y < obstacleMap->getHeight(); y++) { // y coordinate 
			if (criticalPoint(obstacleMap, x, y, 255)) // check if critical point
			{
				//criticalPointMap->setPixel8U(x, y, 100);
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
			criticalPointMap->setPixel8U(posX, i, 123);
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
			criticalPointMap->setPixel8U(posX, i, 123);
		}
	}	
}

void Map::exitEntryPoints()
{
}

void Map::drawCell(Coordinate topLeft, Coordinate bottomRight, int color)
{
	std::cout << "topleft " << topLeft.x << " " << topLeft.y <<std::endl;
	std::cout << "bottomright " << bottomRight.x << " " << bottomRight.y << std::endl;

	for (size_t i = topLeft.x; i <= bottomRight.x ; i++)
	{
		for (size_t k = topLeft.y; k <= bottomRight.y; k++)
		{
			cellDecompositionMap->setPixel8U(i, k, color);
		}
	}
}

void Map::findCells()
{
	for (int x1 = 0; x1 < obstacleMap->getWidth(); x1++) { // x coordinate
		for (int y1 = 0; y1 < obstacleMap->getHeight(); y1++) { // y coordinate 
			if (cellDecompositionMap->getPixelValuei(x1, y1, 0) == 255 || cellDecompositionMap->getPixelValuei(x1, y1, 0) == 123) { // check if we find a white pixel or line sweep
				if (cellDecompositionMap->getPixelValuei(x1, y1, 0) == 123)
					cellDecompositionMap->setPixel8U(x1, y1, 255);
				for (size_t x2 = x1; x2 < obstacleMap->getWidth(); x2++) {
					if (cellDecompositionMap->getPixelValuei(x2, y1, 0) == 0 || cellDecompositionMap->getPixelValuei(x2, y1, 0) == 123) { // check if we hit obstacle or line sweep
						if (cellDecompositionMap->getPixelValuei(x2, y1, 0) != 123)
							x2--;
						for (size_t y2 = y1; y2 < obstacleMap->getHeight(); y2++) {					 // If we hit an obstacle or line sweep, we have the first coordinate to define a cell
							if (cellDecompositionMap->getPixelValuei(x2, y2, 0) == 0) {				// check if we hit obstacle or line sweep
								//if (cellDecompositionMap->getPixelValuei(x2, y2, 0) != 123)
									y2--;
								cells.push_back(Cell(Coordinate(x1, y1), Coordinate(x2, y2)));				// If we hit an obstacle or line sweep, we have the second coordinate to define a cell
								return;
							}
						}
					}
				}
			}
		}
	}
}

bool Map::isMoreCells()
{
	for (size_t i = 0; i <= cellDecompositionMap->getWidth() - 1; i++)
	{
		for (size_t k = 0; k <= cellDecompositionMap->getHeight() - 1; k++)
		{
			if (cellDecompositionMap->getPixelValuei(i, k, 0) == 255) {
				return 1;
			}
		}
	}
	return 0;
}
