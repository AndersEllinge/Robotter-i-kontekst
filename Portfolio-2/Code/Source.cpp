#include <iostream>
#include "Image.hpp"
#include "MyMap.h"

int main() {

	Image* img = PPMLoader::load("map.pgm");

	MyMap map(img);

	//map.createBrushfirePotentialField();
	map.createBrushfireCollisionDetection();
	map.siftGVD();
	//map.createGVD();
	//while(1)
	//{ }


	return 1;
}