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
	//lineSweep();
	lineSweep2();
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
				// criticalPointMap->setPixel8U(x, y, 100);
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
			if (criticalPointMap->getPixelValuei(posX, posY-1, 0) == 123)
			{
				return 1;
			}
			drawLineSweep(posX, posY, 1);
			return 1;
		}

		if (map->getPixelValuei(posX - 1, posY, 0) == target && map->getPixelValuei(posX, posY + 1, 0) == target) { // left and bottom is white - scenario 2
			if (criticalPointMap->getPixelValuei(posX, posY+1, 0) == 123)
			{
				return 1;
			}
			drawLineSweep(posX, posY, 2);
			return 1;
		}

		if (map->getPixelValuei(posX + 1, posY, 0) == target && map->getPixelValuei(posX, posY - 1, 0) == target) { // right and top is white - scenario 1
			if (criticalPointMap->getPixelValuei(posX, posY-1, 0) == 123)
			{
				return 1;
			}
			drawLineSweep(posX, posY, 1);
			return 1;
		}

		if (map->getPixelValuei(posX + 1, posY, 0) == target && map->getPixelValuei(posX, posY + 1, 0) == target) { // right and bottom is white - scenario 2
			if (criticalPointMap->getPixelValuei(posX, posY+1, 0) == 123)
			{
				return 1;
			}
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
		int lineCount = 0;
		for (size_t i = posY-1; i > 0; i--) {
			lineCount++;
			if (criticalPointMap->getPixelValuei(posX, i, 0) == 0) { // Stop at black pixel
				criticalPointMap->setPixel8U(posX - 1, posY - 1 - lineCount / 2, 195); // draw left entry point of the linesweep
				criticalPointMap->setPixel8U(posX + 1, posY - 1 - lineCount / 2, 195); // draw right entry point of the linesweep
				roadMap.addVertex(Coordinate(posX - 1, posY - 1));
				roadMap.addVertex(Coordinate(posX + 1, posY - 1));
				roadMap.addEdge(Coordinate(posX - 1, posY - 1), Coordinate(posX + 1, posY - 1), 2);
				roadMap.addEdge(Coordinate(posX + 1, posY - 1), Coordinate(posX - 1, posY - 1), 2);
				break;
			}
			criticalPointMap->setPixel8U(posX, i, 123);
		}
	}
	if (scenario == 2) // scenario 2
	{
		int lineCount = 0;
		for (size_t i = posY + 1; i < criticalPointMap->getHeight() - 1; i++) {
			lineCount++;
			if (criticalPointMap->getPixelValuei(posX, i, 0) == 0) {
				criticalPointMap->setPixel8U(posX - 1, posY + 1 + lineCount / 2, 195);
				criticalPointMap->setPixel8U(posX + 1, posY + 1 + lineCount / 2, 195);
				roadMap.addVertex(Coordinate(posX - 1, posY + 1));
				roadMap.addVertex(Coordinate(posX + 1, posY + 1));
				roadMap.addEdge(Coordinate(posX - 1, posY + 1), Coordinate(posX + 1, posY + 1), 2);
				roadMap.addEdge(Coordinate(posX + 1, posY + 1), Coordinate(posX - 1, posY + 1), 2);
				break;
			}
			criticalPointMap->setPixel8U(posX, i, 123);
		}
	}	
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
							if (cellDecompositionMap->getPixelValuei(x2, y2, 0) == 0) {				 // check if we hit obstacle or line sweep
																									 // if (cellDecompositionMap->getPixelValuei(x2, y2, 0) != 123)
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

void Map::lineSweep2()
{
	criticalPointMap = obstacleMap->copyFlip(0, 0);

	for (int x = 0; x < obstacleMap->getWidth(); x++) { // x coordinate
		for (int y = 0; y < obstacleMap->getHeight(); y++) { // y coordinate 

			// 1) Identify critical point
			if (isCriticalPoint(x, y)) {
				
				int lineCounter = 0;
				int yIterator = y;

				while (criticalPointMap->getPixelValuei(x, yIterator - 1, 0) == 255) { // Draws line up
					yIterator--;
					criticalPointMap->setPixel8U(x, yIterator, 123); // 123 is color identifier for sweep line
					lineCounter++;
					std::cout << lineCounter << std::endl;
				}

				// Add entry vertices and edge
				if (lineCounter) {
					criticalPointMap->setPixel8U(x - 1, y - lineCounter / 2, 195); // draw left entry point of the linesweep
					criticalPointMap->setPixel8U(x + 1, y - lineCounter / 2, 195); // draw right entry point of the linesweep
					// NOT DONE!
				}
				

				lineCounter = 0;
				yIterator = y;

				while (criticalPointMap->getPixelValuei(x, yIterator + 1, 0) == 255) { // Draws line down
					yIterator++;
					criticalPointMap->setPixel8U(x, yIterator, 123); // 123 is color identifier for sweep line
					lineCounter++;
					std::cout << lineCounter << std::endl;
				}

				// Add entry vertices and edge
				if (lineCounter) {
					criticalPointMap->setPixel8U(x - 1, y + lineCounter / 2, 195); // draw left entry point of the linesweep
					criticalPointMap->setPixel8U(x + 1, y + lineCounter / 2, 195); // draw right entry point of the linesweep
					// NOT DONE!
				}


			}
		}
	}
	



	criticalPointMap->saveAsPGM("criticalPointMap.pgm"); // Save output
}

bool Map::isCriticalPoint(int posX, int posY)
{
	if (posX != 0 && posX != (obstacleMap->getWidth() - 1) && posY != 0 && posY != (obstacleMap->getHeight() - 1) && obstacleMap->getPixelValuei(posX, posY, 0) == 0) { // test for borders and if pixel is black

		if (obstacleMap->getPixelValuei(posX - 1, posY, 0) == 255 && obstacleMap->getPixelValuei(posX, posY - 1, 0) == 255) // left and top is white - scenario 1
			return 1;

		if (obstacleMap->getPixelValuei(posX - 1, posY, 0) == 255 && obstacleMap->getPixelValuei(posX, posY + 1, 0) == 255) // left and bottom is white - scenario 2
			return 1;

		if (obstacleMap->getPixelValuei(posX + 1, posY, 0) == 255 && obstacleMap->getPixelValuei(posX, posY - 1, 0) == 255) // right and top is white - scenario 1
			return 1;

		if (obstacleMap->getPixelValuei(posX + 1, posY, 0) == 255 && obstacleMap->getPixelValuei(posX, posY + 1, 0) == 255) // right and bottom is white - scenario 2
			return 1;

	}

	return 0;
}
