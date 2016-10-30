#include "MyMap.h"
#include <iostream>
#include <vector>
#include <array>

MyMap::MyMap()
{
}

MyMap::MyMap(Image * inputMap)
{
	map = inputMap;
	potentialFieldMap = nullptr;
	collisionDetectionMap = nullptr;
	gvdMap = nullptr;
	siftedMap = nullptr;
	pathMap = nullptr;
}

MyMap::~MyMap()
{
}

void MyMap::createBrushfirePotentialField()
{
	potentialFieldMap = map->copyFlip(0,0); // Create copy of map

	for (int range = 0; range < 255; range++) { // Runs through every possible brushfire value
		for (int width = 0; width < potentialFieldMap->getWidth(); width++) { // Runs through all widths
			for (int height = 0; height < potentialFieldMap->getHeight(); height++) { // Runs through all heights for specific width 
				
				if (potentialFieldMap->getPixelValuei(width, height, 0) == 255) { // Test for blank space
					if (isNextTo8Way(potentialFieldMap, width, height, range) == 1) { // Test for change of space
						potentialFieldMap->setPixel8U(width, height, range + 1); // Set blank space to new value

					}
				}
			}
		}
	}

	potentialFieldMap->saveAsPGM("potentialFieldMap.pgm"); // Save output
}

void MyMap::createBrushfireCollisionDetection()
{
	std::vector<std::vector<int>> pixelsToBeColored;

	collisionDetectionMap = map->copyFlip(0, 0); // Create copy of map
	gvdMap = map->copyFlip(0, 0);

	int min = 0; // minimum distance from the closest edges of the map
	if (collisionDetectionMap->getHeight() > collisionDetectionMap->getWidth())
		min = map->getWidth() / 2 + 1;
	else
		min = map->getHeight() / 2 + 1;

	std::cout << "When " << min-1 << " is reached, then the GVD is found and drawn!" << std::endl;

	for (int i = 0; i < min; i++) { // Times we need to keep brushfiring to be sure we get all collisions
		std::cout << i << std::endl;
		for (int color = 0; color < 36; color+=5) { // Going through all the shades from white to black
			for (int width = 0; width < collisionDetectionMap->getWidth(); width++) { // Iterating through X
				for (int height = 0; height < collisionDetectionMap->getHeight(); height++) { // Iterating through Y
					if (collisionDetectionMap->getPixelValuei(width, height, 0) == 255 ) { // If the pixel is white
						if (isNextTo8Way(collisionDetectionMap, width, height, color)) { // If a pixel around the current pixel is the color
							pixelsToBeColored.push_back({ width, height, color }); // add pixel to the list of pixel to be colored on the collisionDetectionMap
							for (int j = 0; j < pixelsToBeColored.size(); j++) {

								if ((pixelsToBeColored[j][0] == width) && (pixelsToBeColored[j][1] == height) && (pixelsToBeColored[j][2] != color)) { // check if list is same as other pixel in list with diff color 
									gvdMap->setPixel8U(width, height, 1); // Add pixel to gvdMap
								}

								if (pixelsToBeColored[j][0] == width + 1) // check if list is neighbor of pixels of different colors
									if (pixelsToBeColored[j][1] == height)
										if (pixelsToBeColored[j][2] != color) {
											gvdMap->setPixel8U(width, height, 1);
											gvdMap->setPixel8U(pixelsToBeColored[j][0], pixelsToBeColored[j][1], 1);
										}

								if (pixelsToBeColored[j][0] == width - 1) // check if list is neighbor of pixels of different colors
									if (pixelsToBeColored[j][1] == height)
										if (pixelsToBeColored[j][2] != color) {
											gvdMap->setPixel8U(width, height, 1);
											gvdMap->setPixel8U(pixelsToBeColored[j][0], pixelsToBeColored[j][1], 1);
										}

								if (pixelsToBeColored[j][1] == height - 1) // check if list is neighbor of pixels of different colors
									if (pixelsToBeColored[j][0] == width)
										if (pixelsToBeColored[j][2] != color) {
											gvdMap->setPixel8U(width, height, 1);
											gvdMap->setPixel8U(pixelsToBeColored[j][0], pixelsToBeColored[j][1], 1);
										}

								if (pixelsToBeColored[j][1] == height + 1) // check if list is neighbor of pixels of different colors
									if (pixelsToBeColored[j][0] == width)
										if (pixelsToBeColored[j][2] != color) {
											gvdMap->setPixel8U(width, height, 1);
											gvdMap->setPixel8U(pixelsToBeColored[j][0], pixelsToBeColored[j][1], 1);
										}
							}
						}
					}
				}
			}
		}
		for (int i = 0; i < pixelsToBeColored.size(); i++) {
			collisionDetectionMap->setPixel8U(pixelsToBeColored[i][0], pixelsToBeColored[i][1], pixelsToBeColored[i][2]); // color pixel
		}
		pixelsToBeColored.clear();
	}
	collisionDetectionMap->saveAsPGM("collisionDetectionMap.pgm"); // Save output
	gvdMap->saveAsPGM("gvdMap.pgm");
}

void MyMap::siftGVD()
{
	siftedMap = map->copyFlip(0, 0); // Create copy of map

	std::vector<std::vector<int>> siftedGVD;

	for (int width = 0; width < gvdMap->getWidth(); width++) {
		for (int height = 0; height < gvdMap->getHeight(); height++) {
			if (gvdMap->getPixelValuei(width, height, 0) == 1) { 
				if ((width % 2 == 0) && (height % 2 == 0))
				{
					siftedGVD.push_back({ width,height });
				}
				if ((width % 2 != 0) && (height % 2 != 0))
				{
					siftedGVD.push_back({ width,height });
				}
			}
		}
	}
	for (int i = 0; i < siftedGVD.size(); i++) {
		siftedMap->setPixel8U(siftedGVD[i][0], siftedGVD[i][1], 1); // color pixel
	}
	siftedMap->saveAsPGM("siftedMap.pgm");

}

void MyMap::createGVD()
{
	if (potentialFieldMap == nullptr) // If no brushfire was done, do brushfire
		createBrushfirePotentialField();

	gvdMap = map->copyFlip(0,0); // Create copy of map

	for (int width = 0; width < potentialFieldMap->getWidth(); width++) { // Runs through all widths
		for (int height = 0; height < potentialFieldMap->getHeight(); height++) { // Runs through all heights for specific width 

			if (isPartOfGVD(potentialFieldMap, width, height)) // Check if point is on GVD
				gvdMap->setPixel8U(width, height, 150); // Mark GVD point

		}
	}

	gvdMap->saveAsPGM("gvd.pgm"); // Save output
}

void MyMap::findVertices()
{
	for (int width = 0; width < gvdMap->getWidth(); width++) { // Iterating through X
		for (int height = 0; height < gvdMap->getHeight(); height++) { // Iterating through Y
			if (gvdMap->getPixelValuei(width, height, 0) == 1) { // If the pixel is GVD
				vertices.push_back((width * 1000) + height);
			}
		}
	}
}

void MyMap::findEdges()
{
	for (int i = 0; i < vertices.size(); i++) {
		
		if (isNextTo4WayTop(gvdMap, vertices[i] / 1000, vertices[i] % 1000, 1))
			if (!isEdgesInTheList(vertices[i], vertices[i] - 1)) {
				edges.push_back({ vertices[i], vertices[i] - 1 });
			}

		if (isNextTo4WayBottom(gvdMap, vertices[i] / 1000, vertices[i] % 1000, 1))
			if (!isEdgesInTheList(vertices[i], vertices[i] + 1)) {
				edges.push_back({ vertices[i], vertices[i] + 1 });
			}

		if (isNextTo4WayLeft(gvdMap, vertices[i] / 1000, vertices[i] % 1000, 1))
			if (!isEdgesInTheList(vertices[i], vertices[i] - 1000)) {
				edges.push_back({ vertices[i], vertices[i] - 1000 });
			}

		if (isNextTo4WayRight(gvdMap, vertices[i] / 1000, vertices[i] % 1000, 1))
			if (!isEdgesInTheList(vertices[i], vertices[i] + 1000)) {
				edges.push_back({ vertices[i], vertices[i] + 1000 });
			}

	}
}

bool MyMap::isEdgesInTheList(int id1, int id2)
{
	for (int k = 0; k < edges.size(); k++) {
		if (edges[k][0] == id1 && edges[k][1] == id2)
			return true;
		if (edges[k][0] == id2 && edges[k][1] == id1)
			return true;
	}
	return false;
}

bool MyMap::isNextTo4Way(Image* map, int posX, int posY, int target)
{

	if (posX != 0) // Test for border
		if (map->getPixelValuei(posX - 1, posY, 0) == target) // Test below
			return 1;

	if (posX != map->getWidth() - 1) // Test for border
		if (map->getPixelValuei(posX + 1, posY, 0) == target) // Test above
			return 1;

	if (posY != 0) // Test for border
		if (map->getPixelValuei(posX, posY - 1, 0) == target) // Test left
			return 1;

	if (posY != map->getHeight() - 1) // Test for border
		if (map->getPixelValuei(posX, posY + 1, 0) == target) // Test right
			return 1;

	return 0;
}

bool MyMap::isNextTo4WayTop(Image * map, int posX, int posY, int target)
{
	if (posX != 0) // Test for border
		if (map->getPixelValuei(posX , posY - 1, 0) == target) // Test above
			return 1;
	return 0;
}

bool MyMap::isNextTo4WayBottom(Image * map, int posX, int posY, int target)
{
	if (posX != map->getHeight() - 1) // Test for border
		if (map->getPixelValuei(posX, posY + 1, 0) == target) // Test below
			return 1;
	return 0;
}

bool MyMap::isNextTo4WayLeft(Image * map, int posX, int posY, int target)
{
	if (posY != 0) // Test for border
		if (map->getPixelValuei(posX - 1, posY, 0) == target) // Test left
			return 1;
	return 0;
}

bool MyMap::isNextTo4WayRight(Image * map, int posX, int posY, int target)
{
	if (posX != map->getWidth() - 1) // Test for border
		if (map->getPixelValuei(posX + 1, posY, 0) == target) // Test right
			return 1;
	return 0;
}

bool MyMap::isNextTo8Way(Image* map, int posX, int posY, int target)
{
	if (posX != 0) // Test for border
		if (map->getPixelValuei(posX - 1, posY, 0) == target) // Test left
			return 1;

	if (posX != (map->getWidth() - 1)) // Test for border
		if (map->getPixelValuei(posX + 1, posY, 0) == target) // Test right
			return 1;

	if (posY != 0) // Test for border
		if (map->getPixelValuei(posX, posY - 1, 0) == target) // Test top
			return 1;

	if (posY != (map->getHeight() - 1)) // Test for border
		if (map->getPixelValuei(posX, posY + 1, 0) == target) // Test bottom
			return 1;

	if (posX != 0 || (posY != map->getHeight() - 1)) // Test for border
		if(map->getPixelValuei(posX - 1, posY +1 , 0) == target) // Test bottom - left
			return 1;

	if (posX != 0 || posY != 0) // Test for border
		if (map->getPixelValuei(posX - 1, posY - 1, 0) == target) // Test top - left
			return 1;

	if ((posX != map->getWidth() - 1) || (posY != map->getHeight() - 1)) // Test for border
		if (map->getPixelValuei(posX + 1, posY + 1, 0) == target) // Test bottom - right
			return 1;

	if ((posX != map->getWidth() - 1) || posY != 0) // Test for border
		if (map->getPixelValuei(posX + 1, posY - 1, 0) == target) // Test top - right
			return 1;


	return 0;
}

bool MyMap::isPartOfGVD(Image * map, int posX, int posY)
{
	// Test if on obstacle
	if (map->getPixelValuei(posX, posY, 0) == 0)
		return 0;

	// Test for borders
	if (posX == 0 || posX == map->getWidth() - 1)
		return 0;

	if (posY == 0 || posY == map->getHeight() - 1)
		return 0;


	// Test for sides
	if (map->getPixelValuei(posX + 1, posY, 0) < map->getPixelValuei(posX, posY, 0)) {
		if (map->getPixelValuei(posX - 1, posY, 0) == map->getPixelValuei(posX, posY, 0))
			if (map->getPixelValuei(posX - 2, posY, 0) < map->getPixelValuei(posX, posY, 0))
				return 1;
		if (map->getPixelValuei(posX - 1, posY, 0) < map->getPixelValuei(posX, posY, 0))
			return 1;
	}

	if (map->getPixelValuei(posX - 1, posY, 0) < map->getPixelValuei(posX, posY, 0)) {
		if (map->getPixelValuei(posX + 1, posY, 0) == map->getPixelValuei(posX, posY, 0))
			if (map->getPixelValuei(posX + 2, posY, 0) < map->getPixelValuei(posX, posY, 0))
				return 1;
		if (map->getPixelValuei(posX + 1, posY, 0) < map->getPixelValuei(posX, posY, 0))
			return 1;
	}


	// Test for top and bottom
	if (map->getPixelValuei(posX, posY + 1, 0) < map->getPixelValuei(posX, posY, 0)) {
		if (map->getPixelValuei(posX, posY - 1, 0) == map->getPixelValuei(posX, posY, 0))
			if (map->getPixelValuei(posX, posY - 2, 0) < map->getPixelValuei(posX, posY, 0))
				return 1;
		if (map->getPixelValuei(posX, posY - 1, 0) < map->getPixelValuei(posX, posY, 0))
			return 1;

	}

	if (map->getPixelValuei(posX, posY - 1, 0) < map->getPixelValuei(posX, posY, 0)) {
		if (map->getPixelValuei(posX, posY + 1, 0) == map->getPixelValuei(posX, posY, 0))
			if (map->getPixelValuei(posX, posY + 2, 0) < map->getPixelValuei(posX, posY, 0))
				return 1;
		if (map->getPixelValuei(posX, posY + 1, 0) < map->getPixelValuei(posX, posY, 0))
			return 1;
	}


	// Test for one slash
	if (map->getPixelValuei(posX - 1, posY + 1, 0) < map->getPixelValuei(posX, posY, 0)) {
		if (map->getPixelValuei(posX + 1, posY - 1, 0) == map->getPixelValuei(posX, posY, 0))
			if (map->getPixelValuei(posX + 2, posY - 2, 0) < map->getPixelValuei(posX, posY, 0))
				return 1;
		if (map->getPixelValuei(posX + 1, posY - 1, 0) < map->getPixelValuei(posX, posY, 0))
			return 1;
	}		

	if (map->getPixelValuei(posX + 1, posY - 1, 0) < map->getPixelValuei(posX, posY, 0)) {
		if (map->getPixelValuei(posX - 1, posY + 1, 0) == map->getPixelValuei(posX, posY, 0))
			if (map->getPixelValuei(posX - 2, posY + 2, 0) < map->getPixelValuei(posX, posY, 0))
				return 1;
		if (map->getPixelValuei(posX - 1, posY + 1, 0) < map->getPixelValuei(posX, posY, 0))
			return 1;
	}


	// Test for other slash
	if (map->getPixelValuei(posX + 1, posY + 1, 0) < map->getPixelValuei(posX, posY, 0)) {
		if (map->getPixelValuei(posX - 1, posY - 1, 0) == map->getPixelValuei(posX, posY, 0))
			if (map->getPixelValuei(posX - 2, posY - 2, 0) < map->getPixelValuei(posX, posY, 0))
				return 1;
		if (map->getPixelValuei(posX - 1, posY - 1, 0) < map->getPixelValuei(posX, posY, 0))
			return 1;
	}
		
	if (map->getPixelValuei(posX - 1, posY - 1, 0) < map->getPixelValuei(posX, posY, 0)) {
		if (map->getPixelValuei(posX + 1, posY + 1, 0) == map->getPixelValuei(posX, posY, 0))
			if (map->getPixelValuei(posX + 2, posY + 2, 0) < map->getPixelValuei(posX, posY, 0))
				return 1;
		if (map->getPixelValuei(posX + 1, posY + 1, 0) < map->getPixelValuei(posX, posY, 0))
			return 1;
	}

	return 0;
}

void MyMap::createGraph(int start)
{
	int inf = 10000;

	graph.clear();

	for (int i = 0; i < vertices.size(); i++) {
		graph.push_back({ inf, start });
	}

	graph[start][0] = 0;

	std::queue<int> q;
	std::queue<int> adjecentVertices;
	int graphIndex = 0;
	int vertex;

	q.push(start);

	while (!q.empty()) {
		vertex = q.front();
		q.pop();

		for (int i = 0; i < edges.size(); i++) {
			if (edges[i][0] == vertices[vertex])
				adjecentVertices.push(findIndexOfVertex(edges[i][1]));
			if (edges[i][1] == vertices[vertex])
				adjecentVertices.push(findIndexOfVertex(edges[i][0]));
		}

		while (!adjecentVertices.empty()) {
			if (graph[adjecentVertices.front()][0] == inf)
				q.push(adjecentVertices.front());

			if (graph[adjecentVertices.front()][0] > (graph[vertex][0] + 1)) { // substitute 1 with cost
				graph[adjecentVertices.front()][0] = (graph[vertex][0] + 1);
				graph[adjecentVertices.front()][1] = vertex;
			}

			adjecentVertices.pop();
		}
	}
}

void MyMap::printGraph()
{
	std::cout << "Graph is: " << std::endl;
	for (size_t i = 0; i < graph.size(); i++)
	{
		std::cout << "vNr:" << i << "   Dist:" << graph[i][0] << "   From:" << graph[i][1] << std::endl;
	}
	std::cout << "-- End of graph --" << std::endl;
}

int MyMap::findClosestValidVertex(int posX, int posY)
{
	double dist;
	double closestsDist = 1000000;
	int closestVertex = 1000000;

	for (size_t i = 0; i < vertices.size(); i++)
	{
		//std::cout << "vNr" << i << std::endl;

		if (isPathClear(posX, posY, (vertices[i] / 1000), (vertices[i] % 1000))) {
			dist = sqrt(pow(posX - (vertices[i] / 1000), 2) + pow(posY - (vertices[i] % 1000), 2)); // Pythagoras

			if (dist < closestsDist) {
				closestsDist = dist;
				closestVertex = i;
			}
		}
	}

	return closestVertex;
}

void MyMap::createPath(int startX, int startY, int goalX, int goalY)
{
	int startVertex = findClosestValidVertex(startX, startY);
	if (startVertex == 1000000) {
		std::cout << "No path from start to a node in map" << std::endl;
		return;
	}

	int goalVertex = findClosestValidVertex(goalX, goalY);
	if (goalVertex == 1000000) {
		std::cout << "No path from goal to a node in map" << std::endl;
		return;
	}

	while (!path.empty())
		path.pop();

	createGraph(startVertex);

	int pointer = goalVertex;

	path.push({ goalX, goalY });

	while (pointer != startVertex) {
		path.push({ vertices[pointer] / 1000, vertices[pointer] % 1000 });
		pointer = graph[pointer][1];
	}

	path.push({ vertices[pointer] / 1000, vertices[pointer] % 1000 });

	printPath(startX, startY, goalX, goalY);
}

void MyMap::printPath(int startX, int startY, int goalX, int goalY)
{
	pathMap = map->copyFlip(0, 0);
	std::cout << "Path is: " << std::endl;
	drawStraightLine(startX, startY, path.top()[0], path.top()[1]);
	int prevX = 0;
	int prevY = 0;
	int prev2X = 0;
	int prev2Y = 0;
	while (!path.empty()) {
		std::cout << path.top()[0] << ", " << path.top()[1] << std::endl;
		pathMap->setPixel8U(path.top()[0], path.top()[1], 100);
		prev2X = prevX;
		prev2Y = prevY;
		prevX = path.top()[0];
		prevY = path.top()[1];
		path.pop();
	}
	drawStraightLine(goalX, goalY, prev2X, prev2Y);
	std::cout << "--End of path--" << std::endl;
	pathMap->saveAsPGM("pathMap.pgm");
}

void MyMap::createRoadMap()
{
	createBrushfireCollisionDetection();
	findVertices();
	findEdges();
}

int MyMap::findIndexOfVertex(int vertex)
{
	for (int i = 0; i < vertices.size(); i++) {
		if (vertices[i] == vertex)
			return i;
	}
}

void MyMap::drawStraightLine(int x0, int y0, int x1, int y1)
{
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

			for (int x = x0; x < x1; x++)
			{
				pathMap->setPixel8U(x, y, 100);

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
				for (int y = y1; y < y0; y++)
				{
					pathMap->setPixel8U(x, y, 100);

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
				for (int y = y0; y < y1; y++)
				{
					pathMap->setPixel8U(x, y, 100);

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
			for (int x = x1; x < x0; x++)
			{

				pathMap->setPixel8U(x, y, 100);

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
				for (int y = y1; y < y0; y++)
				{
					pathMap->setPixel8U(x, y, 100);

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
				for (int y = y0; y < y1; y++)
				{
					pathMap->setPixel8U(x, y, 100);

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

bool MyMap::isPathClear(int x0, int y0, int x1, int y1)
{
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

			for (int x = x0; x < x1; x++)
			{
				if (map->getPixelValuei(x, y, 0) != 255)
					return 0;

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
				for (int y = y1; y < y0; y++)
				{
					if (map->getPixelValuei(x, y, 0) != 255)
						return 0;

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
				for (int y = y0; y < y1; y++)
				{
					if (map->getPixelValuei(x, y, 0) != 255)
						return 0;

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
			for (int x = x1; x < x0; x++)
			{

				if (map->getPixelValuei(x, y, 0) != 255)
					return 0;

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
				for (int y = y1; y < y0; y++)
				{
					if (map->getPixelValuei(x, y, 0) != 255)
						return 0;

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
				for (int y = y0; y < y1; y++)
				{
					if (map->getPixelValuei(x, y, 0) != 255)
						return 0;

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

	return 1;
}