#pragma once
#include "Image.hpp"
#include "PPMLoader.hpp"

using namespace rw::sensor;
using namespace rw::loaders;

class MyMap
{
public:
	MyMap();
	MyMap(Image* inputMap);
	~MyMap();

	void createBrushfirePotentialField();
	void createBrushfireCollisionDetection();
	void siftGVD();
	void createGVD();
	void findVertices();
	void findEdges();
	bool isEdgesInTheList(int id1, int id2);

	std::vector<int> vertices;
	std::vector<std::vector<int>> edges;

private:
	Image* map;
	Image* potentialFieldMap;
	Image* collisionDetectionMap;
	Image* gvdMap;
	Image* siftedMap;

	bool isNextTo4Way(Image* map, int posX, int posY, int target);

	bool isNextTo4WayTop(Image* map, int posX, int posY, int target);
	bool isNextTo4WayBottom(Image* map, int posX, int posY, int target);
	bool isNextTo4WayLeft(Image* map, int posX, int posY, int target);
	bool isNextTo4WayRight(Image* map, int posX, int posY, int target);
	
	bool isNextTo8Way(Image* map, int posX, int posY, int target);
	bool isPartOfGVD(Image* map, int posX, int posY);


};

