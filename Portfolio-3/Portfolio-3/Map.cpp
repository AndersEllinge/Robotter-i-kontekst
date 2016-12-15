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
	robotPos = Coordinate(basex, basey);
	distanceTraveled = 0;
}

void Map::searchMap()
{
	pathTakenMap = obstacleMap->copyFlip(0, 0);

	// OFFLINE COMPUTATIONS
	//lineSweep();
	lineSweep2();
	createCells();
	identifyCellsForVertices();
	linkVerticesInCells();

	// ONLINE COMPUTATIONS
	mapCoverage();
	collectTargets();
	std::cout << "Distance traveled: " << distanceTraveled << " pixels" << std::endl;



	//std::cout << std::endl << "Vertices:" << std::endl;
	//roadMap.printVertices();

	//std::cout << std::endl;

	//std::cout << "Edges:" << std::endl;
	//roadMap.printEdges();

	pathTakenMap->saveAsPGM("pathTakenMap.pgm"); // Save output
	itemMap->saveAsPGM("itemMap.pgm"); // Save output
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
	//std::cout << "topleft " << topLeft.x << " " << topLeft.y <<std::endl;
	//std::cout << "bottomright " << bottomRight.x << " " << bottomRight.y << std::endl;

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
					//std::cout << lineCounter << std::endl;
				}

				// Add entry vertices and edge
				if (lineCounter) {
					criticalPointMap->setPixel8U(x - 1, y - lineCounter / 2, 195); // draw left entry point of the linesweep
					criticalPointMap->setPixel8U(x + 1, y - lineCounter / 2, 195); // draw right entry point of the linesweep
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
					//std::cout << lineCounter << std::endl;
				}

				// Add entry vertices and edge
				if (lineCounter) {
					criticalPointMap->setPixel8U(x - 1, y + lineCounter / 2, 195); // draw left entry point of the linesweep
					criticalPointMap->setPixel8U(x + 1, y + lineCounter / 2, 195); // draw right entry point of the linesweep
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

	// Go  trough all vertices
	// Compare to cells to find corresponding cell
	// Update vertex cell key

	for (int i = 0; i < roadMap.getVertices().size(); i++) {
		roadMap.updateCellKey(i, findCellFromCoordinate(roadMap.getVertices()[i].coordinate));
	}
}

void Map::linkVerticesInCells()
{
	for (int i = 0; i < roadMap.getVertices().size(); i++) {
		for (int j = 0; j < roadMap.getVertices().size(); j++) {
			if (i != j) {

				if (roadMap.getVertices()[i].cellKey == roadMap.getVertices()[j].cellKey) {
					roadMap.addEdge(roadMap.getVertices()[i].coordinate, roadMap.getVertices()[j].coordinate);
				}

			}
		}
	}
}

void Map::drawPath(std::stack<Coordinate> path)
{
	Coordinate prev = path.top();
	path.pop();
	while (!path.empty()) {
		drawStraightLine(prev, path.top());
		distanceTraveled = distanceTraveled + sqrt(pow(abs(prev.x - path.top().x), 2) + pow(abs(prev.y - path.top().y), 2));
		//pathTakenMap->setPixel8U(path.top().x, path.top().y, 100);

		//std::cout << "DREW PATH FROM " << prev.x << ", " << prev.y << " TO " << path.top().x << ", " << path.top().y << std::endl;
		prev = path.top();
		path.pop();
	}
}

void Map::drawStraightLine(Coordinate c1, Coordinate c2)
{
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
				pathTakenMap->setPixel8U(x, y, 150);

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
					pathTakenMap->setPixel8U(x, y, 150);

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
					pathTakenMap->setPixel8U(x, y, 150);

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

				pathTakenMap->setPixel8U(x, y, 150);

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
					pathTakenMap->setPixel8U(x, y, 150);

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
					pathTakenMap->setPixel8U(x, y, 150);

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
	for (int i = 0; i < cells.size(); i++) {
		cellCoverage(i);
	}
}

void Map::cellCoverage(int cellId)
{
	
	//drawPath(roadMap.getPath(robotPos, Coordinate(cells[cellId].cellCorners[0].x + 1, cells[cellId].cellCorners[0].y + 1)));
	
	int closestToStart = findClosestVertex(robotPos);
	int closestToGoal = findClosestVertex(Coordinate(cells[cellId].cellCorners[0].x + 1, cells[cellId].cellCorners[0].y + 1));
	drawStraightLine(robotPos, roadMap.getVertices()[closestToStart].coordinate);
	distanceTraveled = distanceTraveled + sqrt(pow(abs(robotPos.x - roadMap.getVertices()[closestToStart].coordinate.x), 2) + pow(abs(robotPos.y - roadMap.getVertices()[closestToStart].coordinate.y), 2));
	drawPath(roadMap.getPath(closestToStart, closestToGoal));
	drawStraightLine(roadMap.getVertices()[closestToGoal].coordinate, Coordinate(cells[cellId].cellCorners[0].x + 1, cells[cellId].cellCorners[0].y + 1));
	distanceTraveled = distanceTraveled + sqrt(pow(abs(roadMap.getVertices()[closestToGoal].coordinate.x - cells[cellId].cellCorners[0].x + 1), 2) + pow(abs(roadMap.getVertices()[closestToGoal].coordinate.y - cells[cellId].cellCorners[0].y + 1), 2));
	robotPos = Coordinate(cells[cellId].cellCorners[0].x + 1, cells[cellId].cellCorners[0].y + 1);

	bool isNotDone = 1;
	bool direction = 0; // 0 is down

	while (isNotDone) {

		if (direction) {
			while (obstacleMap->getPixelValuei(robotPos.x, robotPos.y - 1, 0) != 0) {
				scanForTarget(robotPos);
				robotPos.y = robotPos.y - 1;
				pathTakenMap->setPixel8U(robotPos.x, robotPos.y, 50);
				distanceTraveled++;
				
			}

			for (int i = 0; i < 3; i++) {
				if (obstacleMap->getPixelValuei(robotPos.x + 1, robotPos.y, 0) == 0 || robotPos.x + 1 > cells[cellId].cellCorners[1].x) {
					if (i != 2)
						isNotDone = 0;
					break;
				}
				robotPos.x = robotPos.x + 1;
				distanceTraveled++;
				pathTakenMap->setPixel8U(robotPos.x, robotPos.y, 50);
			}
			direction = 0;

		}
		else {
			while (obstacleMap->getPixelValuei(robotPos.x, robotPos.y + 1, 0) != 0) {
				scanForTarget(robotPos);
				robotPos.y = robotPos.y + 1;
				pathTakenMap->setPixel8U(robotPos.x, robotPos.y, 50);
				distanceTraveled++;
				
			}

			for (int i = 0; i < 3; i++) {
				if (obstacleMap->getPixelValuei(robotPos.x + 1, robotPos.y, 0) == 0 || robotPos.x + 1 > cells[cellId].cellCorners[1].x) {
					if (i != 2)
						isNotDone = 0;
					break;
				}
				robotPos.x = robotPos.x + 1;
				distanceTraveled++;
				pathTakenMap->setPixel8U(robotPos.x, robotPos.y, 50);
			}
			direction = 1;
		}
	}
}

int Map::findCellFromCoordinate(Coordinate coordinate)
{

	for (int i = 0; i < cells.size(); i++) {
		if (coordinate.x >= cells[i].cellCorners[0].x) // larger x than top left corner of cell
			if (coordinate.y >= cells[i].cellCorners[0].y) // larger y than top left corner of cell
				if (coordinate.x <= cells[i].cellCorners[1].x) // less x than bottom right corner of cell
					if (coordinate.y <= cells[i].cellCorners[1].y) // less y than bottom left cornor of cell
						return i;
	}

	return unknown;
}

int Map::findClosestVertex(Coordinate coordinate)
{
	double prevClosest = inf;
	int closestVertex = unknown;
	int cell = findCellFromCoordinate(coordinate);
	for (int i = 0; i < roadMap.getVertices().size(); i++) {
		double dist = sqrt(pow(abs(coordinate.x - roadMap.getVertices()[i].coordinate.x), 2) + pow(abs(coordinate.y - roadMap.getVertices()[i].coordinate.y), 2));
		//std::cout << dist << " " << prevClosest << std::endl;
		if (dist < prevClosest && cell == roadMap.getVertices()[i].cellKey) {
			prevClosest = dist;
			closestVertex = i;
		}
	}
	return closestVertex;
}

void Map::scanForTarget(Coordinate position)
{
	if (itemMap->getPixelValuei(robotPos.x, robotPos.y, 0) == 0) { // Check this
		targets.push_back(Coordinate(robotPos.x, robotPos.y));
		itemMap->setPixel8U(robotPos.x, robotPos.y, 150);
	}

	if (itemMap->getPixelValuei(robotPos.x, robotPos.y - 1, 0) == 0) { // Check top
		targets.push_back(Coordinate(robotPos.x, robotPos.y - 1));
		itemMap->setPixel8U(robotPos.x, robotPos.y - 1, 150);
	}

	if (itemMap->getPixelValuei(robotPos.x, robotPos.y + 1, 0) == 0) { // Check bottom
		targets.push_back(Coordinate(robotPos.x, robotPos.y + 1));
		itemMap->setPixel8U(robotPos.x, robotPos.y + 1, 150);
	}

	if (itemMap->getPixelValuei(robotPos.x + 1, robotPos.y, 0) == 0) { // Check right
		targets.push_back(Coordinate(robotPos.x + 1, robotPos.y));
		itemMap->setPixel8U(robotPos.x + 1, robotPos.y, 150);
	}

	if (itemMap->getPixelValuei(robotPos.x - 1, robotPos.y, 0) == 0) { // Check left
		targets.push_back(Coordinate(robotPos.x - 1, robotPos.y));
		itemMap->setPixel8U(robotPos.x - 1, robotPos.y, 150);
	}

	if (itemMap->getPixelValuei(robotPos.x + 1, robotPos.y - 1, 0) == 0) { // Check top right
		targets.push_back(Coordinate(robotPos.x + 1, robotPos.y - 1));
		itemMap->setPixel8U(robotPos.x + 1, robotPos.y - 1, 150);
	}

	if (itemMap->getPixelValuei(robotPos.x - 1, robotPos.y - 1, 0) == 0) { // Check top left
		targets.push_back(Coordinate(robotPos.x - 1, robotPos.y - 1));
		itemMap->setPixel8U(robotPos.x - 1, robotPos.y - 1, 150);
	}

	if (itemMap->getPixelValuei(robotPos.x + 1, robotPos.y + 1, 0) == 0) { // Check bottom right
		targets.push_back(Coordinate(robotPos.x + 1, robotPos.y + 1));
		itemMap->setPixel8U(robotPos.x + 1, robotPos.y + 1, 150);
	}

	if (itemMap->getPixelValuei(robotPos.x - 1, robotPos.y + 1, 0) == 0) { // Check bottom left
		targets.push_back(Coordinate(robotPos.x - 1, robotPos.y + 1));
		itemMap->setPixel8U(robotPos.x - 1, robotPos.y + 1, 150);
	}
}

void Map::collectTargets()
{
	int closestToStart;
	int closestToGoal;
	for (int i = 0; i < targets.size(); i++) {
		
		// Go to target
		closestToStart = findClosestVertex(robotPos);
		closestToGoal = findClosestVertex(targets[i]);
		drawStraightLine(robotPos, roadMap.getVertices()[closestToStart].coordinate);
		distanceTraveled = distanceTraveled + sqrt(pow(abs(robotPos.x - roadMap.getVertices()[closestToStart].coordinate.x), 2) + pow(abs(robotPos.y - roadMap.getVertices()[closestToStart].coordinate.y), 2));
		drawPath(roadMap.getPath(closestToStart, closestToGoal));
		drawStraightLine(roadMap.getVertices()[closestToGoal].coordinate, targets[i]);
		distanceTraveled = distanceTraveled + sqrt(pow(abs(roadMap.getVertices()[closestToGoal].coordinate.x - targets[i].x), 2) + pow(abs(roadMap.getVertices()[closestToGoal].coordinate.y - targets[i].y), 2));


		robotPos = targets[i];


		// Return to base
		closestToStart = findClosestVertex(robotPos);
		drawStraightLine(robotPos, roadMap.getVertices()[closestToStart].coordinate);
		distanceTraveled = distanceTraveled + sqrt(pow(abs(robotPos.x - roadMap.getVertices()[closestToStart].coordinate.x), 2) + pow(abs(robotPos.y - roadMap.getVertices()[closestToStart].coordinate.y), 2));
		drawPath(roadMap.getPath(closestToStart, 0));
		robotPos = roadMap.getVertices()[0].coordinate;

	}


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
