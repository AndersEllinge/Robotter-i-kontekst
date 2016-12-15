#include "Map.h"
#include <iostream>

Map::Map(Image* inputMapObstacle, Image* inputMapItems, int basex, int basey)
{
	obstacleMap = inputMapObstacle;
	itemMap = inputMapItems;
	pathTakenMap = nullptr;
	pickUpMap = nullptr;
	cellDecompositionMap = nullptr;
	criticalPointMap = nullptr;
	roadMap.addVertex(Coordinate(basex, basey));
	robotPos = Coordinate(basex, basey);
	distanceTraveled = 0;
}

void Map::searchMap()
{
	int collectDistance;
	pathTakenMap = obstacleMap->copyFlip(0, 0);
	pickUpMap = obstacleMap->copyFlip(0, 0);

	// OFFLINE COMPUTATIONS
	lineSweep();
	createCells();
	identifyCellsForVertices();
	linkVerticesInCells();

	// ONLINE COMPUTATIONS
	mapCoverage();
	std::cout << "Coverage distance traveled: " << distanceTraveled << " pixels" << std::endl;
	collectDistance = distanceTraveled;
	collectTargets();
	std::cout << "Collect distance traveled: " << distanceTraveled - collectDistance << " pixels" << std::endl;

	// OPERATION DONE
	std::cout << "Total distance traveled: " << distanceTraveled << " pixels" << std::endl;

	pickUpMap->saveAsPGM("pickUpPath.pgm"); // Save output
	pathTakenMap->saveAsPGM("pathTakenMap.pgm"); // Save output
	itemMap->saveAsPGM("itemMap.pgm"); // Save output
}


Map::~Map()
{
}

void Map::createCells()
{
	cellDecompositionMap = criticalPointMap->copyFlip(0, 0); // copy map

	bool moreCells = 1;
	int color = 20; // first cell color
	while (moreCells) // keep going if more cells
	{
		moreCells = 0; 
		findCells();
		drawCell(cells[cells.size()-1].cellCorners[0], cells[cells.size()-1].cellCorners[1], color); // draw the cell found
		moreCells = isMoreCells();
		color = color + 20; // new cell color
	}
	
	cellDecompositionMap->saveAsPGM("cellDecompositionMap.pgm"); // Save output
}

void Map::drawCell(Coordinate topLeft, Coordinate bottomRight, int color)
{
	for (size_t i = topLeft.x; i <= bottomRight.x ; i++) // x coordinates for the cell
	{
		for (size_t k = topLeft.y; k <= bottomRight.y; k++) // y coordinates for the cell
		{
			cellDecompositionMap->setPixel8U(i, k, color); // draw the pixel
		}
	}
}

void Map::findCells()
{
	for (int x1 = 0; x1 < obstacleMap->getWidth(); x1++) { // x1 is going to be the coordinate top left corner
		for (int y1 = 0; y1 < obstacleMap->getHeight(); y1++) { // y1 coordinate is going to be the coordinate for top left corner
			if (cellDecompositionMap->getPixelValuei(x1, y1, 0) == 255 || cellDecompositionMap->getPixelValuei(x1, y1, 0) == 123) { // check if we find a white pixel or line sweep
				if (cellDecompositionMap->getPixelValuei(x1, y1, 0) == 123) // If we find a sweepline pixel, we make it white, because we dont need it anymore
					cellDecompositionMap->setPixel8U(x1, y1, 255);
				for (size_t x2 = x1; x2 < obstacleMap->getWidth(); x2++) {  // x2 is going to be the coordinate for bottom right corner
					if (cellDecompositionMap->getPixelValuei(x2, y1, 0) == 0 || cellDecompositionMap->getPixelValuei(x2, y1, 0) == 123) { // check if we hit obstacle or line sweep
						if (cellDecompositionMap->getPixelValuei(x2, y1, 0) != 123) // if its a linesweep line, then we step one step back
							x2--;
						for (size_t y2 = y1; y2 < obstacleMap->getHeight(); y2++) {	// y2 is going to be the coordinate for the bottom right corner
							if (cellDecompositionMap->getPixelValuei(x2, y2, 0) == 0) {	 // If we hit an obstacle we take one step back
									y2--;
								cells.push_back(Cell(Coordinate(x1, y1), Coordinate(x2, y2))); // If we hit an obstacle or line sweep, we have the second coordinate to define a cell
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
			if (cellDecompositionMap->getPixelValuei(i, k, 0) == 255) { // if there is a white cell, then there are more cells to be made
				return 1;
			}
		}
	}
	return 0;
}

void Map::lineSweep()
{
	criticalPointMap = obstacleMap->copyFlip(0, 0); // make map
	
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
					
				}

				// Add entry vertices and edge
				if (lineCounter) {
					criticalPointMap->setPixel8U(x - 1, y - lineCounter / 2, 195); // draw left entry point of the linesweep, only for debugging purposes
					criticalPointMap->setPixel8U(x + 1, y - lineCounter / 2, 195); // draw right entry point of the linesweep, only for debugging purposes
					roadMap.addVertex(Coordinate(x - 1, y - lineCounter / 2));
					roadMap.addVertex(Coordinate(x + 1, y - lineCounter / 2));
					roadMap.addEdge(Coordinate(x - 1, y - lineCounter / 2), Coordinate(x + 1, y - lineCounter / 2), 2);
					roadMap.addEdge(Coordinate(x + 1, y - lineCounter / 2), Coordinate(x - 1, y - lineCounter / 2), 2);
				}
				

				lineCounter = 0;
				yIterator = y;

				while (criticalPointMap->getPixelValuei(x, yIterator + 1, 0) == 255) { // Draws line down
					yIterator++;
					criticalPointMap->setPixel8U(x, yIterator, 123); // 123 is color identifier for sweep line
					lineCounter++;
					
				}

				// Add entry vertices and edge
				if (lineCounter) {
					criticalPointMap->setPixel8U(x - 1, y + lineCounter / 2, 195); // draw left entry point of the linesweep, only for debugging purposes
					criticalPointMap->setPixel8U(x + 1, y + lineCounter / 2, 195); // draw right entry point of the linesweep, only for debugging purposes
					roadMap.addVertex(Coordinate(x - 1, y + lineCounter / 2));
					roadMap.addVertex(Coordinate(x + 1, y + lineCounter / 2));
					roadMap.addEdge(Coordinate(x - 1, y + lineCounter / 2), Coordinate(x + 1, y + lineCounter / 2), 2);
					roadMap.addEdge(Coordinate(x + 1, y + lineCounter / 2), Coordinate(x - 1, y + lineCounter / 2), 2);
				}


			}
		}
	}
	criticalPointMap->saveAsPGM("criticalPointMap.pgm"); // Save output
}

void Map::identifyCellsForVertices()
{
	// Go through all vertices
	// Compare to cells to find corresponding cell
	// Update vertex cell key

	for (int i = 0; i < roadMap.getVertices().size(); i++) {
		roadMap.updateCellKey(i, findCellFromCoordinate(roadMap.getVertices()[i].coordinate));
	}
}

void Map::linkVerticesInCells()
{
	// Go through all the vertices, and compare them with all the others
	for (int i = 0; i < roadMap.getVertices().size(); i++) {
		for (int j = 0; j < roadMap.getVertices().size(); j++) {
			if (i != j) { // If its the same vertex, then ignore
				if (roadMap.getVertices()[i].cellKey == roadMap.getVertices()[j].cellKey) { // If are in the same cell
					roadMap.addEdge(roadMap.getVertices()[i].coordinate, roadMap.getVertices()[j].coordinate); // create edge
				}
			}
		}
	}
}

void Map::drawPath(std::stack<Coordinate> path, Image* map)
{
	Coordinate prev = path.top(); // temp holder for prev posistion
	path.pop();
	while (!path.empty()) { // while not empty
		drawStraightLine(prev, path.top(), map); // draw line between the prev and current posisition
		distanceTraveled = distanceTraveled + sqrt(pow(abs(prev.x - path.top().x), 2) + pow(abs(prev.y - path.top().y), 2)); // update distance traveled
		prev = path.top(); // update prev position
		path.pop();
	}
}

void Map::drawStraightLine(Coordinate c1, Coordinate c2 , Image* map)
{
	// This function draws a straight line between c1 and c2 on map
	int x0 = c1.x;
	int x1 = c2.x;
	int y0 = c1.y;
	int y1 = c2.y;

	if (x0 < x1) {
		int deltaX = x1 - x0;
		int deltaY = y1 - y0;

		if (y0 > y1)
			deltaY = y0 - y1;

		if (x0 > x1)
			deltaX = x0 - x1;

		if (deltaY < deltaX) {
			int deltaErr = deltaY - deltaX;
			int y = y0;

			for (int x = x0; x <= x1; x++)
			{
				map->setPixel8U(x, y, 150);

				if (deltaErr >= 0) {
					if (y1 > y0)
						y = y + 1;
					else
						y = y - 1;
					deltaErr = deltaErr - deltaX;
				}

				deltaErr = deltaErr + deltaY;

			}
		}

		else {

			int deltaErr = deltaX - deltaY;

			if (y0 > y1) {
				int x = x1;
				for (int y = y1; y <= y0; y++)
				{
					map->setPixel8U(x, y, 150);

					if (deltaErr >= 0) {
						if (x1 > x0)
							x = x - 1;
						else
							x = x + 1;
						deltaErr = deltaErr - deltaY;
					}

					deltaErr = deltaErr + deltaX;

				}
			}
			else {
				int x = x0;
				for (int y = y0; y <= y1; y++)
				{
					map->setPixel8U(x, y, 150);

					if (deltaErr >= 0) {
						if (x1 > x0)
							x = x + 1;
						else
							x = x - 1;
						deltaErr = deltaErr - deltaY;
					}

					deltaErr = deltaErr + deltaX;

				}
			}
		}


	}

	else {
		int deltaX = x0 - x1;
		int deltaY = y0 - y1;
		int y = y1;

		if (y1 > y0)
			deltaY = y1 - y0;

		int deltaErr = deltaY - deltaX;

		if (deltaY < deltaX) {
			for (int x = x1; x <= x0; x++)
			{

				map->setPixel8U(x, y, 150);

				if (deltaErr >= 0) {
					if (y1 > y0)
						y = y - 1;
					else
						y = y + 1;
					deltaErr = deltaErr - deltaX;
				}

				deltaErr = deltaErr + deltaY;

			}
		}

		else {

			int deltaErr = deltaX - deltaY;

			if (y0 > y1) {
				int x = x1;
				for (int y = y1; y <= y0; y++)
				{
					map->setPixel8U(x, y, 150);

					if (deltaErr >= 0) {
						if (x1 > x0)
							x = x - 1;
						else
							x = x + 1;
						deltaErr = deltaErr - deltaY;
					}

					deltaErr = deltaErr + deltaX;

				}
			}
			else {
				int x = x0;
				for (int y = y0; y <= y1; y++)
				{
					map->setPixel8U(x, y, 150);

					if (deltaErr >= 0) {
						if (x1 > x0)
							x = x + 1;
						else
							x = x - 1;
						deltaErr = deltaErr - deltaY;
					}

					deltaErr = deltaErr + deltaX;

				}
			}
		}
	}
}

void Map::mapCoverage()
{
	for (int i = 0; i < cells.size(); i++) { // go through all the cells
		cellCoverage(i); // cover the cells
	}
}

void Map::cellCoverage(int cellId)
{
	int closestToStart = findClosestVertex(robotPos); // vertex identifier of the closet vertex to robot position
	int closestToGoal = findClosestVertex(Coordinate(cells[cellId].cellCorners[0].x + 1, cells[cellId].cellCorners[0].y + 1)); // vertex identifier of the cloest vertex to the goal position
	drawStraightLine(robotPos, roadMap.getVertices()[closestToStart].coordinate, pathTakenMap); // draw straight line from robot position to the roadmap
	distanceTraveled = distanceTraveled + sqrt(pow(abs(robotPos.x - roadMap.getVertices()[closestToStart].coordinate.x), 2) + pow(abs(robotPos.y - roadMap.getVertices()[closestToStart].coordinate.y), 2)); // update the distance traveled
	drawPath(roadMap.getPath(closestToStart, closestToGoal), pathTakenMap); // draw the path taken untill break of from the roadmap
	drawStraightLine(roadMap.getVertices()[closestToGoal].coordinate, Coordinate(cells[cellId].cellCorners[0].x + 1, cells[cellId].cellCorners[0].y + 1), pathTakenMap); // draw straight line from roadmap to the goal
	distanceTraveled = distanceTraveled + sqrt(pow(abs(roadMap.getVertices()[closestToGoal].coordinate.x - cells[cellId].cellCorners[0].x + 1), 2) + pow(abs(roadMap.getVertices()[closestToGoal].coordinate.y - cells[cellId].cellCorners[0].y + 1), 2)); // update the distance traveled
	robotPos = Coordinate(cells[cellId].cellCorners[0].x + 1, cells[cellId].cellCorners[0].y + 1); // update the robot position

	bool isNotDone = 1; // are we done covering?
	bool direction = 0; // 0 is down

	while (isNotDone) { // when done break

		if (direction) { // if we are going up
			while (obstacleMap->getPixelValuei(robotPos.x, robotPos.y - 1, 0) != 0) { // while no obstacles are in the way, move one pixel up
				scanForTarget(robotPos); // scan for items
				robotPos.y = robotPos.y - 1; // update position
				pathTakenMap->setPixel8U(robotPos.x, robotPos.y, 50); // draw the movement
				distanceTraveled++; // update the distance traveled
			}

			for (int i = 0; i < 3; i++) { // go right up to 3 times
				if (obstacleMap->getPixelValuei(robotPos.x + 1, robotPos.y, 0) == 0 || robotPos.x + 1 > cells[cellId].cellCorners[1].x) { // if we hit an obstacle, or go beyond the cell size
					if (i != 2) // if it is not the 3. movement
						isNotDone = 0; // then we are not done yet
					break; // else we are done
				}
				robotPos.x = robotPos.x + 1; // movement to the right
				distanceTraveled++; // update distance traveled
				pathTakenMap->setPixel8U(robotPos.x, robotPos.y, 50); // draw the movement
			}
			direction = 0; // now we go up
		}
		else {
			while (obstacleMap->getPixelValuei(robotPos.x, robotPos.y + 1, 0) != 0) { // while no obstacles are in the way, move on pixel down
				scanForTarget(robotPos); // scan for items
				robotPos.y = robotPos.y + 1; // update posistion
				pathTakenMap->setPixel8U(robotPos.x, robotPos.y, 50); // draw the movement
				distanceTraveled++; // update the distance traveled
			}
			for (int i = 0; i < 3; i++) { // go right up to 3 times
				if (obstacleMap->getPixelValuei(robotPos.x + 1, robotPos.y, 0) == 0 || robotPos.x + 1 > cells[cellId].cellCorners[1].x) { // if we hit and obstacle, or go beyond the cell size
					if (i != 2) // if it is not hte 3. movement
						isNotDone = 0; // then we are not done yet
					break; // else we are done
				}
				robotPos.x = robotPos.x + 1; // movement to the right
				distanceTraveled++; // update distance traveled
				pathTakenMap->setPixel8U(robotPos.x, robotPos.y, 50); // draw the movement
			}
			direction = 1; // now we go up
		}
	}
}

int Map::findCellFromCoordinate(Coordinate coordinate)
{
	for (int i = 0; i < cells.size(); i++) { // go through all the cells
		if (coordinate.x >= cells[i].cellCorners[0].x) // larger x than top left corner of cell
			if (coordinate.y >= cells[i].cellCorners[0].y) // larger y than top left corner of cell
				if (coordinate.x <= cells[i].cellCorners[1].x) // less x than bottom right corner of cell
					if (coordinate.y <= cells[i].cellCorners[1].y) // less y than bottom left cornor of cell
						return i; // return the cell identifier
	}
	return unknown;
}

int Map::findClosestVertex(Coordinate coordinate)
{
	double prevClosest = inf; // start distance since no prevClosest vertex has been made yet
	int closestVertex = unknown; // no closestVertex found yet
	int cell = findCellFromCoordinate(coordinate); // Get the cell of the vertex
	for (int i = 0; i < roadMap.getVertices().size(); i++) { // for all verticies
		double dist = sqrt(pow(abs(coordinate.x - roadMap.getVertices()[i].coordinate.x), 2) + pow(abs(coordinate.y - roadMap.getVertices()[i].coordinate.y), 2)); // distance between coordinate and vertex
		if (dist < prevClosest && cell == roadMap.getVertices()[i].cellKey) { // if distance is less than prevCloset, and it has the same cell key
			prevClosest = dist; // then update the prevClosest
			closestVertex = i; // and update the cloestVertex identifier
		}
	}
	return closestVertex; // return the cloestVertex identifier
}

void Map::scanForTarget(Coordinate position)
{
	if (itemMap->getPixelValuei(robotPos.x, robotPos.y, 0) == 0) { // Check the position pixel, if black then
		targets.push_back(Coordinate(robotPos.x, robotPos.y)); // save target
		itemMap->setPixel8U(robotPos.x, robotPos.y, 150); // recolor target, so it wont be found again
	}

	if (itemMap->getPixelValuei(robotPos.x, robotPos.y - 1, 0) == 0) { // Check top pixel, if black then
		targets.push_back(Coordinate(robotPos.x, robotPos.y - 1));// save target
		itemMap->setPixel8U(robotPos.x, robotPos.y - 1, 150); // recolor target, so it wont be found again
	}

	if (itemMap->getPixelValuei(robotPos.x, robotPos.y + 1, 0) == 0) { // Check bottom pixel, if black then
		targets.push_back(Coordinate(robotPos.x, robotPos.y + 1));// save target
		itemMap->setPixel8U(robotPos.x, robotPos.y + 1, 150); // recolor target, so it wont be found again
	}

	if (itemMap->getPixelValuei(robotPos.x + 1, robotPos.y, 0) == 0) { // Check right pixel, if black then
		targets.push_back(Coordinate(robotPos.x + 1, robotPos.y));// save target
		itemMap->setPixel8U(robotPos.x + 1, robotPos.y, 150); // recolor target, so it wont be found again
	}

	if (itemMap->getPixelValuei(robotPos.x - 1, robotPos.y, 0) == 0) { // Check left pixel, if black then
		targets.push_back(Coordinate(robotPos.x - 1, robotPos.y));// save target
		itemMap->setPixel8U(robotPos.x - 1, robotPos.y, 150); // recolor target, so it wont be found again
	}

	if (itemMap->getPixelValuei(robotPos.x + 1, robotPos.y - 1, 0) == 0) { // Check top right pixel, if black then
		targets.push_back(Coordinate(robotPos.x + 1, robotPos.y - 1));// save target
		itemMap->setPixel8U(robotPos.x + 1, robotPos.y - 1, 150); // recolor target, so it wont be found again
	}

	if (itemMap->getPixelValuei(robotPos.x - 1, robotPos.y - 1, 0) == 0) { // Check top left pixel, if black then
		targets.push_back(Coordinate(robotPos.x - 1, robotPos.y - 1));// save target
		itemMap->setPixel8U(robotPos.x - 1, robotPos.y - 1, 150); // recolor target, so it wont be found again
	}

	if (itemMap->getPixelValuei(robotPos.x + 1, robotPos.y + 1, 0) == 0) { // Check bottom right pixel, if black then
		targets.push_back(Coordinate(robotPos.x + 1, robotPos.y + 1));// save target
		itemMap->setPixel8U(robotPos.x + 1, robotPos.y + 1, 150); // recolor target, so it wont be found again
	}

	if (itemMap->getPixelValuei(robotPos.x - 1, robotPos.y + 1, 0) == 0) { // Check bottom left pixel, if black then
		targets.push_back(Coordinate(robotPos.x - 1, robotPos.y + 1));// save target
		itemMap->setPixel8U(robotPos.x - 1, robotPos.y + 1, 150); // recolor target, so it wont be found again
	}
}

void Map::collectTargets()
{
	int closestToStart;
	int closestToGoal;
	for (int i = targets.size() - 1; i >= 0; i--) { // take the latest target first
		
		// Go to target
		closestToStart = findClosestVertex(robotPos);
		closestToGoal = findClosestVertex(targets[i]);
		drawStraightLine(robotPos, roadMap.getVertices()[closestToStart].coordinate, pickUpMap);
		distanceTraveled = distanceTraveled + sqrt(pow(abs(robotPos.x - roadMap.getVertices()[closestToStart].coordinate.x), 2) + pow(abs(robotPos.y - roadMap.getVertices()[closestToStart].coordinate.y), 2));
		drawPath(roadMap.getPath(closestToStart, closestToGoal), pickUpMap);
		drawStraightLine(roadMap.getVertices()[closestToGoal].coordinate, targets[i], pickUpMap);
		distanceTraveled = distanceTraveled + sqrt(pow(abs(roadMap.getVertices()[closestToGoal].coordinate.x - targets[i].x), 2) + pow(abs(roadMap.getVertices()[closestToGoal].coordinate.y - targets[i].y), 2));
		robotPos = targets[i];

		// Return to base
		closestToStart = findClosestVertex(robotPos);
		drawStraightLine(robotPos, roadMap.getVertices()[closestToStart].coordinate, pickUpMap);
		distanceTraveled = distanceTraveled + sqrt(pow(abs(robotPos.x - roadMap.getVertices()[closestToStart].coordinate.x), 2) + pow(abs(robotPos.y - roadMap.getVertices()[closestToStart].coordinate.y), 2));
		drawPath(roadMap.getPath(closestToStart, 0), pickUpMap);
		robotPos = roadMap.getVertices()[0].coordinate;
	}
}

bool Map::isCriticalPoint(int posX, int posY)
{
	if (posX != 0 && posX != (obstacleMap->getWidth() - 1) && posY != 0 && posY != (obstacleMap->getHeight() - 1) && obstacleMap->getPixelValuei(posX, posY, 0) == 0) { // test for borders and if pixel is black
		if (obstacleMap->getPixelValuei(posX - 1, posY, 0) == 255 && obstacleMap->getPixelValuei(posX, posY - 1, 0) == 255) // left and top is white 
			return 1;

		if (obstacleMap->getPixelValuei(posX - 1, posY, 0) == 255 && obstacleMap->getPixelValuei(posX, posY + 1, 0) == 255) // left and bottom is white
			return 1;

		if (obstacleMap->getPixelValuei(posX + 1, posY, 0) == 255 && obstacleMap->getPixelValuei(posX, posY - 1, 0) == 255) // right and top is white 
			return 1;

		if (obstacleMap->getPixelValuei(posX + 1, posY, 0) == 255 && obstacleMap->getPixelValuei(posX, posY + 1, 0) == 255) // right and bottom is white 
			return 1;
	}
	return 0;
}
