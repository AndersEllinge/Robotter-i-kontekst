#include <vector>
#include <queue>
#include <iostream>
#include "TestMap.h"


int main() {

	TestMap map;

	int start[2] = { 0, 0 };
	int goal[2] = { 100, 100 };

	std::cout << "Closest vertex to start is: " << map.findClosestValidVertex(start[0], start[1]) << std::endl;
	std::cout << "Closest vertex to goal is: " << map.findClosestValidVertex(goal[0], goal[1]) << std::endl;

	std::cout << std::endl;

	map.createPath(start[0], start[1], goal[0], goal[1]);

	map.printGraph();

	std::cout << std::endl;

	map.printPath();

	std::cout << std::endl;

	return 1;
}
