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

	void createBrushfire();
	void createGVD();


private:
	Image* map;
	Image* brushfireMap;
	Image* gvdMap;

	bool isNextTo4Way(Image* map, int posX, int posY, int target);
	bool isNextTo8Way(Image* map, int posX, int posY, int target);
	bool isPartOfGVD(Image* map, int posX, int posY);
};

