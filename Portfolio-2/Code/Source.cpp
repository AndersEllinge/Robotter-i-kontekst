#include <iostream>
#include "Image.hpp"
#include "MyMap.h"

int main() {

	Image* img = PPMLoader::load("map.pgm");

	MyMap map(img);

	map.brushfire();

	return 1;
}