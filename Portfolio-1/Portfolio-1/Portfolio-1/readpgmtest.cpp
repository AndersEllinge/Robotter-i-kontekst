#include <iostream>
#include "Image.hpp"
#include "PPMLoader.hpp"
#include "Dd_robot.h"

#define PI 3.14159265359

using namespace rw::sensor;
using namespace rw::loaders;

int main()
{
	Dd_robot jens(1, 0, 0, 1, 0, 0); // Vores robot hedder Jens
	
	jens.move(1, 0, 0, 1, 100, 0);
	jens.whereAreYou();
	jens.move(1, 0, 0, 1, 0, 200);
	jens.whereAreYou();
	jens.move(cos(30 * PI / 180), sin(30 * PI / 180), -sin(30 * PI / 180), cos(30 * PI / 180), 0, 0);
	jens.whereAreYou();
	jens.move(cos(60 * PI / 180), sin(60 * PI / 180), -sin(60 * PI / 180), cos(60 * PI / 180), 200, 500);
	jens.whereAreYou();
	
	jens.findMinWorkSpace();
}