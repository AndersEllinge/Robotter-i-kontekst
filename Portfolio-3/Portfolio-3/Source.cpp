
#include "Map.h"
#include "Image.hpp"

int main() {

	Image* obstacleMap = PPMLoader::load("map.pgm");
	Image* itemMap = PPMLoader::load("targets.pgm");
	Map map(obstacleMap, itemMap, 10, 10);

	map.searchMap();

}

