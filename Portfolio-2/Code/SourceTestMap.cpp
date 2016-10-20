#include <vector>
#include <queue>
#include <iostream>
#include "TestMap.h"
#include "Image.hpp"
#include "PPMLoader.hpp"

using namespace rw::sensor;
using namespace rw::loaders;


int main() {

	Image* img = PPMLoader::load("map.ppm");

	TestMap map(img);

	int start[2] = { 10, 10 };
	int goal[2] = { 57, 80 };

	map.makeDot();

	//std::cout << "X = " << (map.vertices[1] / 1000) << std::endl;
	//std::cout << "Y = " << (map.vertices[1] % 1000) << std::endl;

	//std::cout << "Closest vertex to start is: " << map.findClosestValidVertex(start[0], start[1]) << std::endl;
	//std::cout << "Closest vertex to goal is: " << map.findClosestValidVertex(goal[0], goal[1]) << std::endl;

	//std::cout << std::endl;

	//std::cout << map.isPathClear(start[0], start[1], goal[0], goal[1]) << std::endl;

	//std::cout << map.findClosestValidVertex(goal[0], goal[1]) << std::endl;

	//map.createPath(start[0], start[1], goal[0], goal[1]);
	//map.printGraph();

	//std::cout << std::endl;

	//map.printPath();

	//std::cout << std::endl;

	return 1;
}
