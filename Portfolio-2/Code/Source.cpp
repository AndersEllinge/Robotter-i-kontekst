#include <iostream>
#include "Image.hpp"
#include "MyMap.h"

int main() {

	Image* img = PPMLoader::load("map.pgm");

	MyMap map(img);

	map.createBrushfireCollisionDetection();
	map.createRoadMap();
	map.createPath(10, 10, 190, 190);

	std::cout << "DONE" << std::endl;
}