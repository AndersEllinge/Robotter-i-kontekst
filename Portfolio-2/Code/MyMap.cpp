#include "MyMap.h"



MyMap::MyMap()
{
}

MyMap::MyMap(Image * inputMap)
{
	map = inputMap;
	brushfireMap = nullptr;
	GvdMap = nullptr;
}


MyMap::~MyMap()
{
}

void MyMap::createBrushfire()
{
	brushfireMap = map->copyFlip(0,0);

	for (int range = 0; range < 255; range++) { // Runs through every possible brushfire value
		for (int width = 0; width < brushfireMap->getWidth(); width++) { // Runs through all widths
			for (int height = 0; height < brushfireMap->getHeight(); height++) { // Runs through all heights for specific width 
				
				if (brushfireMap->getPixelValuei(width, height, 0) == 255) { // test for blank space
					if (isNextTo4Way(brushfireMap, width, height, range) == 1) { // Test for change of space
						brushfireMap->setPixel8U( width, height, range + 1); // Set blank space to new value
					}
				}
			}
		}
	}

	brushfireMap->saveAsPGM("brushfire.pgm");
}

void MyMap::createGVD()
{

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

