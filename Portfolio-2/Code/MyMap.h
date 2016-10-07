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

	void brushfire();


private:
	Image* map;

	bool isNextTo4Way(int posX, int posY, int target);

};

