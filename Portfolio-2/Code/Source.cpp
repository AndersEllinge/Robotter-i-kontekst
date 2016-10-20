#include <iostream>
#include "Image.hpp"
#include "MyMap.h"

int main() {

	Image* img = PPMLoader::load("map.pgm");

	MyMap map(img);

	//map.createBrushfirePotentialField();
	//map.createBrushfireCollisionDetection();
	//map.siftGVD();
	//map.createGVD();
	//while(1)
	//{ }
	//map.findVertices();
	//map.findEdges();
	//for (int i = 0; i < map.edges.size(); i++) {
	//	std::cout << "id 1: " << map.edges[i][0] << " " << "id 2: " << map.edges[i][1] << std::endl;
	//}
	/*for (int i = 0; i < map.vertices.size(); i++) {
		std::cout << "verti: " << map.vertices[i] << std::endl;
	}*/

	map.createRoadMap();
	map.createPath(35, 67, 134, 158);

	std::cout << "DONE" << std::endl;
	while(1)
	{ }
	return 1;
}