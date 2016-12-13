#pragma once
#include "Image.hpp"
#include "PPMLoader.hpp"

using namespace rw::sensor;
using namespace rw::loaders;

class Map
{
public:
	Map(Image* inputMapObstacle, Image* inputMapItems);
	~Map();


private:
	Image* obstacleMap;
	Image* itemMap;
	Image* pathTakenMap;

	void lineSweep();

};

