#include "MyMap.h"



MyMap::MyMap()
{
}

MyMap::MyMap(Image * inputMap)
{
	map = inputMap;
	brushfireMap = nullptr;
	gvdMap = nullptr;
}


MyMap::~MyMap()
{
}

void MyMap::createBrushfire()
{
	brushfireMap = map->copyFlip(0,0); // Create copy of map

	for (int range = 0; range < 255; range++) { // Runs through every possible brushfire value
		for (int width = 0; width < brushfireMap->getWidth(); width++) { // Runs through all widths
			for (int height = 0; height < brushfireMap->getHeight(); height++) { // Runs through all heights for specific width 
				
				if (brushfireMap->getPixelValuei(width, height, 0) == 255) { // Test for blank space
					if (isNextTo4Way(brushfireMap, width, height, range) == 1) { // Test for change of space
						brushfireMap->setPixel8U(width, height, range + 1); // Set blank space to new value

					}
				}
			}
		}
	}

	brushfireMap->saveAsPGM("brushfire.pgm"); // Save output
}

void MyMap::createGVD()
{
	if (brushfireMap == nullptr) // If no brushfire was done, do brushfire
		createBrushfire();

	gvdMap = map->copyFlip(0,0); // Create copy of map

	for (int width = 0; width < brushfireMap->getWidth(); width++) { // Runs through all widths
		for (int height = 0; height < brushfireMap->getHeight(); height++) { // Runs through all heights for specific width 

			if (isPartOfGVD(brushfireMap, width, height)) // Check if point is on GVD
				gvdMap->setPixel8U(width, height, 150); // Mark GVD point

		}
	}

	gvdMap->saveAsPGM("gvd.pgm"); // Save output
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



	if (map->getPixelValuei(posX - 1, posY + 1, 0) < map->getPixelValuei(posX, posY, 0))
		if (map->getPixelValuei(posX + 1, posY - 1, 0) <= map->getPixelValuei(posX, posY, 0))
			if (map->getPixelValuei(posX + 1, posY - 1, 0) < map->getPixelValuei(posX, posY, 0))
				if (map->getPixelValuei(posX - 1, posY + 1, 0) <= map->getPixelValuei(posX, posY, 0))
					return 1;




	if (map->getPixelValuei(posX + 1, posY + 1, 0) < map->getPixelValuei(posX, posY, 0))
		if (map->getPixelValuei(posX - 1, posY - 1, 0) <= map->getPixelValuei(posX, posY, 0))
			if (map->getPixelValuei(posX - 1, posY - 1, 0) < map->getPixelValuei(posX, posY, 0))
				if (map->getPixelValuei(posX + 1, posY + 1, 0) <= map->getPixelValuei(posX, posY, 0))
					return 1;

	return 0;
}

