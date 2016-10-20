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
		for (int color = 0; color < 31; color+=5) { // Going through all the shades from white to black
			for (int width = 0; width < collisionDetectionMap->getWidth(); width++) { // Iterating through X
				for (int height = 0; height < collisionDetectionMap->getHeight(); height++) { // Iterating through Y
					if (collisionDetectionMap->getPixelValuei(width, height, 0) == 255 ) { // If the pixel is white
						if (isNextTo8Way(collisionDetectionMap, width, height, color)) { // If a pixel around the current pixel is the color
							pixelsToBeColored.push_back({ width, height, color }); // add pixel to the list of pixel to be colored on the collisionDetectionMap
							for (int j = 0; j < pixelsToBeColored.size(); j++) {

								if ((pixelsToBeColored[j][0] == width) && (pixelsToBeColored[j][1] == height) && (pixelsToBeColored[j][2] != color)) { // check if list is same as other pixel in list with diff color 
									gvdMap->setPixel8U(width, height, 1); // Add pixel to gvdMap


									//vertices.push_back((width * 1000) + height); // Adding the node to list of verticies
									//if (isNextTo4WayTop(gvdMap, width, height, 1))	// Checking for vertecies next to current vertex and making edges
									//	edges.push_back({ (width * 1000) + height, (width * 1000) + height - 1 });
									//if (isNextTo4WayBottom(gvdMap, width, height, 1))
									//	edges.push_back({ (width * 1000) + height, (width * 1000) + height + 1 });
									//if (isNextTo4WayLeft(gvdMap, width, height, 1))
									//	edges.push_back({ (width * 1000) + height, ((width - 1) * 1000) + height });
									//if (isNextTo4WayRight(gvdMap, width, height, 1))
									//	edges.push_back({ (width * 1000) + height, ((width + 1) * 1000) + height });


								}

								if (pixelsToBeColored[j][0] == width + 1) // check if list is neighbor of pixels of different colors
									if (pixelsToBeColored[j][1] == height)
										if (pixelsToBeColored[j][2] != color) {
											gvdMap->setPixel8U(width, height, 1);
											gvdMap->setPixel8U(pixelsToBeColored[j][0], pixelsToBeColored[j][1], 1);


											//vertices.push_back((width * 1000) + height); // Adding the node to list of verticies
											//if (isNextTo4WayTop(gvdMap, width, height, 1)) // Checking for vertecies next to current vertex and making edges
											//	edges.push_back({ (width * 1000) + height, (width * 1000) + height - 1 });
											//if (isNextTo4WayBottom(gvdMap, width, height, 1))
											//	edges.push_back({ (width * 1000) + height, (width * 1000) + height + 1 });
											//if (isNextTo4WayLeft(gvdMap, width, height, 1))
											//	edges.push_back({ (width * 1000) + height, ((width - 1) * 1000) + height });
											//if (isNextTo4WayRight(gvdMap, width, height, 1))
											//	edges.push_back({ (width * 1000) + height, ((width + 1) * 1000) + height });


										}

								if (pixelsToBeColored[j][0] == width - 1) // check if list is neighbor of pixels of different colors
									if (pixelsToBeColored[j][1] == height)
										if (pixelsToBeColored[j][2] != color) {
											gvdMap->setPixel8U(width, height, 1);
											gvdMap->setPixel8U(pixelsToBeColored[j][0], pixelsToBeColored[j][1], 1);


											//vertices.push_back((width * 1000) + height); // Adding the node to list of verticies
											//if (isNextTo4WayTop(gvdMap, width, height, 1)) // Checking for vertecies next to current vertex and making edges
											//	edges.push_back({ (width * 1000) + height, (width * 1000) + height - 1 });
											//if (isNextTo4WayBottom(gvdMap, width, height, 1))
											//	edges.push_back({ (width * 1000) + height, (width * 1000) + height + 1 });
											//if (isNextTo4WayLeft(gvdMap, width, height, 1))
											//	edges.push_back({ (width * 1000) + height, ((width - 1) * 1000) + height });
											//if (isNextTo4WayRight(gvdMap, width, height, 1))
											//	edges.push_back({ (width * 1000) + height, ((width + 1) * 1000) + height });



										}

								if (pixelsToBeColored[j][1] == height - 1) // check if list is neighbor of pixels of different colors
									if (pixelsToBeColored[j][0] == width)
										if (pixelsToBeColored[j][2] != color) {
											gvdMap->setPixel8U(width, height, 1);
											gvdMap->setPixel8U(pixelsToBeColored[j][0], pixelsToBeColored[j][1], 1);


											//vertices.push_back((width * 1000) + height); // Adding the node to list of verticies
											//if (isNextTo4WayTop(gvdMap, width, height, 1)) // Checking for vertecies next to current vertex and making edges
											//	edges.push_back({ (width * 1000) + height, (width * 1000) + height - 1 });
											//if (isNextTo4WayBottom(gvdMap, width, height, 1))
											//	edges.push_back({ (width * 1000) + height, (width * 1000) + height + 1 });
											//if (isNextTo4WayLeft(gvdMap, width, height, 1))
											//	edges.push_back({ (width * 1000) + height, ((width - 1) * 1000) + height });
											//if (isNextTo4WayRight(gvdMap, width, height, 1))
											//	edges.push_back({ (width * 1000) + height, ((width + 1) * 1000) + height });


										}

								if (pixelsToBeColored[j][1] == height + 1) // check if list is neighbor of pixels of different colors
									if (pixelsToBeColored[j][0] == width)
										if (pixelsToBeColored[j][2] != color) {
											gvdMap->setPixel8U(width, height, 1);
											gvdMap->setPixel8U(pixelsToBeColored[j][0], pixelsToBeColored[j][1], 1);


											//vertices.push_back((width * 1000) + height); // Adding the node to list of verticies
											//if (isNextTo4WayTop(gvdMap, width, height, 1)) // Checking for vertecies next to current vertex and making edges
											//	edges.push_back({ (width * 1000) + height, (width * 1000) + height - 1 });
											//if (isNextTo4WayBottom(gvdMap, width, height, 1))
											//	edges.push_back({ (width * 1000) + height, (width * 1000) + height + 1 });
											//if (isNextTo4WayLeft(gvdMap, width, height, 1))
											//	edges.push_back({ (width * 1000) + height, ((width - 1) * 1000) + height });
											//if (isNextTo4WayRight(gvdMap, width, height, 1))
											//	edges.push_back({ (width * 1000) + height, ((width + 1) * 1000) + height });



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

