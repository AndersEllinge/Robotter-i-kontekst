#include "Dd_robot.h"
#include "Image.hpp"
#include "PPMLoader.hpp"
#include <cmath>
#include <iostream>

#define PI 3.14159265359

using namespace rw::sensor;
using namespace rw::loaders;

Dd_robot::Dd_robot()
{
}

Dd_robot::Dd_robot(double or_x1, double or_x2, double or_y1, double or_y2, int pos_x, int pos_y)
{
	curr_arr[0][0] = or_x1;
	curr_arr[0][1] = or_y1;
	curr_arr[0][2] = pos_x;
	curr_arr[1][0] = or_x2;
	curr_arr[1][1] = or_y2;
	curr_arr[1][2] = pos_y;
	curr_arr[2][0] = 0;
	curr_arr[2][1] = 0;
	curr_arr[2][2] = 1;

	new_arr[0][0] = or_x1;
	new_arr[0][1] = or_y1;
	new_arr[0][2] = pos_x;
	new_arr[1][0] = or_x2;
	new_arr[1][1] = or_y2;
	new_arr[1][2] = pos_y;
	new_arr[2][0] = 0;
	new_arr[2][1] = 0;
	new_arr[2][2] = 1;

	//robotOrientation = 0;  // thetha for the robot to the work space

	time_in_motion = 0;
	velocity_left = 4;			// pixels pr. sec
	velocity_right = 4;
	wheelRadius = 0.1;		// units in pixels
	distanceBetweenWheels = 0.4; 
	circumference = PI * distanceBetweenWheels; // units in pixels
	drawPoint(pos_x, pos_y, 100);
}

Dd_robot::~Dd_robot()
{
}

void Dd_robot::matrixMulti(double a1, double a2, double b1, double b2, double c1, double c2)
{
	if (a1 < 0.001 && a1 > -0.001)
		{
			a1= 0;
		}
		if (a2 < 0.001 && a2 > -0.001)
		{
			a2 = 0;
		}
		if (b1 < 0.001 && b1 > -0.001)
		{
			b1 = 0;
		}
		if (b2 < 0.001 && b2 > -0.001)
		{
			b2 = 0;
		}

	double temp[3][3] =
	{
		{ a1, b1, c1 },
		{ a2, b2, c2 },
		{ 0,   0,  1 },
	};
	for (int i = 0; i<3; ++i)
		for (int j = 0; j<3; ++j)
		{
			new_arr[i][j] = 0;
		}


	for (int i = 0; i<3; ++i)
		for (int j = 0; j<3; ++j)
			for (int k = 0; k<3; ++k)
			{
				new_arr[i][j] += curr_arr[i][k] * temp[k][j];
			}

	/*for (size_t i = 0; i < 3; i++)
	{
		for (size_t k = 0; k < 3; k++)
		{
			new_arr[i][k] = res[i][k];
		}

	}*/
	if (new_arr[0][0] < 0.001 && new_arr[0][0] > -0.001)
		{
			new_arr[0][0] = 0;
		}
		if (new_arr[1][0] < 0.001 && new_arr[1][0] > -0.001)
		{
			new_arr[1][0] = 0;
		}
		if (new_arr[0][1] < 0.001 && new_arr[0][1] > -0.001)
		{
			new_arr[0][1] = 0;
		}
		if (new_arr[1][1] < 0.001 && new_arr[1][1] > -0.001)
		{
			new_arr[1][1] = 0;
		}
}

void Dd_robot::move(double x1, double x2, double y1, double y2, int distance_x, int distance_y)  // Input as transferfucntion with radians
{
	std::cout << std::endl;
	std::cout << "Reading new transformation ..." << '\n'<< std::endl;

	saveCurrentTransformation();
	matrixMulti(x1, x2, y1, y2, distance_x, distance_y);
	//getRotationalSpeed();
	if (testWorkSpace(new_arr[0][2],new_arr[1][2]))
	{
		translate();
		std::cout << "Rotating to given end orientation " << std::endl;
		
		globalRotateEnd(new_arr[0][0], new_arr[1][0], new_arr[0][1], new_arr[1][1]);
	}
	std::cout << "The whole motion took: " << time_in_motion << " seconds." << std::endl;
	std::cout << std::endl;
	time_in_motion = 0;
}

void Dd_robot::whereAreYou()
{
	std::cout << std::endl << "Frame for Robot updated relative to it's start position: " << std::endl;
	for (int i = 0; i < 3; ++i)
	{
		std::cout << " |";
		for (int j = 0; j < 3; ++j)
		{
			std::cout << "    " << new_arr[i][j];
			if (j == 3 - 1)
				std::cout << "   |" << std::endl;
		}
	}
	std::cout << std::endl;
}

void Dd_robot::globalRotate(double x1, double x2, double y1, double y2, double curr1, double curr2)
{
	double dotProduct = -curr_arr[0][0] * x2 + x1 * curr_arr[1][0];
	if (dotProduct > 0)
	{
		setRobotVelocity(abs(getRobotVelocityL()), -abs(getRobotVelocityR()));	//right turn
	}
	else if (dotProduct < 0)
	{
		setRobotVelocity(-abs(getRobotVelocityL()), abs(getRobotVelocityL()));	//left turn
	}
	printRotationalSpeed();

	std::cout << "Spent " << 
			calcRotationTime(angleBetweenVectors(curr_arr[0][0], curr_arr[1][0], x1, x2)) 
			<< " seconds rotating" << std::endl;
	time_in_motion += calcRotationTime(angleBetweenVectors(curr_arr[0][0], curr_arr[1][0], x1, x2));
	
	curr_arr[0][0] = x1;
	curr_arr[1][0] = x2;
	curr_arr[0][1] = y1;
	curr_arr[1][1] = y2;
}

void Dd_robot::globalRotateEnd(double x1, double x2, double y1, double y2)
{
	if (x1 == curr_arr[0][0] && x2 == curr_arr[1][0])
	{
		curr_arr[0][0] = x1;
		curr_arr[1][0] = x2;
		curr_arr[0][1] = y1;
		curr_arr[1][1] = y2;
		return;
	}
	double dotProduct = -curr_arr[0][0] * x2 + x1 * curr_arr[1][0];
	if (dotProduct > 0)
	{
		setRobotVelocity(abs(getRobotVelocityL()), -abs(getRobotVelocityR()));	//right turn
	}
	else if (dotProduct < 0)
	{
		setRobotVelocity(-abs(getRobotVelocityL()), abs(getRobotVelocityR()));	//left turn
	}
	printRotationalSpeed();
	std::cout << "Spent " <<
		calcRotationTime(angleBetweenVectors(curr_arr[0][0], curr_arr[1][0], x1, x2))
		<< " seconds rotating" << std::endl;
	time_in_motion += calcRotationTime(angleBetweenVectors(curr_arr[0][0], curr_arr[1][0], x1, x2));

	curr_arr[0][0] = x1;
	curr_arr[1][0] = x2;
	curr_arr[0][1] = y1;
	curr_arr[1][1] = y2;
}

void Dd_robot::saveCurrentTransformation()   // Update our current transformation
{											
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t k = 0; k < 3; k++)
		{
			curr_arr[i][k] = new_arr[i][k];     //Thie new is actually the last known rotation. (have not set the new yet)
		}
	}
}

void Dd_robot::translate()
{
	translate_x();
	translate_y();
	drawPoint(new_arr[0][2], new_arr[1][2], 100);		// this is where the robot is stationary! Gets its own colour!
}

void Dd_robot::translate_x()
{
	double distance_x = new_arr[0][2];
	double position_x = curr_arr[0][2];
	if (distance_x == position_x)
	{
		return;
	}
	if (distance_x >= position_x)
	{
		if (curr_arr[0][0] == 1 && curr_arr[1][0] == 0
			&& curr_arr[0][1] == 0 && curr_arr[1][1] == 1)
		{
			setRobotVelocity(getRobotVelocityL(), getRobotVelocityR());
			printRotationalSpeed();
			std::cout << "Moving towards pixel " << "(" <<new_arr[0][2] << ", " << curr_arr[1][2] << ") along x-axis" << std::endl;
			time_in_motion += (distance_x - position_x) / velocity_left;
			lineFunction(curr_arr[0][2], curr_arr[1][2], new_arr[0][2], curr_arr[1][2]);
		}
		else
		{
			std::cout << "Rotating towards postive x-axis to move along it " << std::endl;
			
			globalRotate(1, 0, 0, 1, curr_arr[0][0], curr_arr[1][0]);
			translate_x();
		}
	}
	
	if (distance_x <= position_x)
	{
		if (curr_arr[0][0] == -1 && curr_arr[1][0] == 0
			&& curr_arr[0][1] == 0 && curr_arr[1][1] == -1)
		{
			setRobotVelocity(getRobotVelocityL(), getRobotVelocityR());
			printRotationalSpeed();
			std::cout << "Moving towards pixel " << "(" << new_arr[0][2] << ", " << curr_arr[1][2] << ") along x-axis" << std::endl;
			time_in_motion += (position_x - distance_x) / velocity_left;
			lineFunction(curr_arr[0][2], curr_arr[1][2], new_arr[0][2], curr_arr[1][2]);
		}
		else
		{
			std::cout << "Rotating towards negative x-axis to move along it " << std::endl;
			
			globalRotate(-1, 0, 0, -1, curr_arr[0][0], curr_arr[1][0]);
			translate_x();
		}
	}
}   

void Dd_robot::translate_y()
{
	double distance_y = new_arr[1][2];
	double position_y = curr_arr[1][2];
	if (distance_y == position_y)
	{
		return;
	}
	if (distance_y >= position_y)
	{
		if (curr_arr[0][0] == 0 && curr_arr[1][0] == 1
			&& curr_arr[0][1] == -1 && curr_arr[1][1] == 0)
		{
			setRobotVelocity(abs(getRobotVelocityL()), abs(getRobotVelocityR()));
			printRotationalSpeed();
			std::cout << "Moving towards pixel " << "(" << new_arr[0][2] << ", " << new_arr[1][2] << ") along y-axis" << std::endl;
			time_in_motion += (distance_y - position_y) / velocity_left;
			lineFunction(new_arr[0][2], curr_arr[1][2], new_arr[0][2], new_arr[1][2]);
		}
		else
		{
			std::cout << "Rotating towards postive y-axis to move along it " << std::endl;
			globalRotate(0, 1, -1, 0, curr_arr[0][0], curr_arr[1][0]);
			translate_y();
		}
	}
	if (distance_y <= position_y)
	{
		if (curr_arr[0][0] == 0 && curr_arr[1][0] == -1
			&& curr_arr[0][1] == 1 && curr_arr[1][1] == 0)
		{
			setRobotVelocity(abs(getRobotVelocityL()), abs(getRobotVelocityL()));
			printRotationalSpeed();
			std::cout << "Moving towards pixel " << "(" << new_arr[0][2] << ", " << new_arr[1][2] << ") along y-axis" << std::endl;
			time_in_motion += (position_y - distance_y) / velocity_left;
			lineFunction(new_arr[0][2], curr_arr[1][2], new_arr[0][2], new_arr[1][2]);
		}
		else
		{
			std::cout << "Rotating towards negative y-axis to move along it " << std::endl;
			globalRotate(0, -1, 1, 0, curr_arr[0][0], curr_arr[1][0]);
			translate_y();
		}
	}
}

bool Dd_robot::testWorkSpace(int distance_x, int distance_y)
{
	std::string filename("Bane1.pgm");
	Image* img = PPMLoader::load(filename);
	signed int placeholder = img->getWidth();				//gitWidth does not work with signed
	if (distance_x > placeholder)
	{
		std::cout << "ABORT: Robot moving " << distance_x - placeholder
			<< " pixels beyond workspace in the direction of the x-axis." << std::endl;
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t k = 0; k < 3; k++)
			{
				new_arr[i][k] = curr_arr[i][k];     //Overwrite the new transformation to the old one
			}
		}
		delete img;
		return false;
	}
	if (distance_x < 0)
	{
		std::cout << "ABORT: Robot moving " << distance_x
			<< " pixels beyond workspace in the direction of the x-axis" << std::endl;
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t k = 0; k < 3; k++)
			{
				new_arr[i][k] = curr_arr[i][k];     //Overwrite the new transformation to the old one
			}
		}
		delete img;
		return false;
	}
	placeholder = img->getHeight();
	if (distance_y > placeholder)
	{
		std::cout << "ABORT: Robot moving " << distance_y - placeholder
			<< " pixels beyond workspace in the direction of the y-axis" << std::endl;
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t k = 0; k < 3; k++)
			{
				new_arr[i][k] = curr_arr[i][k];     //Overwrite the new transformation to the old one
			}
		}
		delete img;
		return false;
	}
	if (distance_y < 0)
	{
		std::cout << "ABORT: Robot moving " << distance_y
			<< " pixels beyond workspace in the direction of the y-axis" << std::endl;
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t k = 0; k < 3; k++)
			{
				new_arr[i][k] = curr_arr[i][k];     //Overwrite the new transformation to the old one
			}
		}
		delete img;
		return false;
	}
	delete img;
	return true;
}

void Dd_robot::setRobotVelocity(int newVelocityLeft, int newVelocityRight)
{
	velocity_left = newVelocityLeft;
	velocity_right = newVelocityRight;
}

int Dd_robot::getRobotVelocityL()
{
	return velocity_left;
}

int Dd_robot::getRobotVelocityR()
{
	return velocity_right;
}



double Dd_robot::angleBetweenVectors(double x1, double x2,double y1, double y2)
{
	double dotProduct = x1 * y1 + x2 * y2;
	return acos(dotProduct);
}

void Dd_robot::findMinWorkSpace()
{
	std::string filename("Bane1.pgm");
	Image* img = PPMLoader::load(filename);
	int xHighest = 0;
	int yHighest = 0;
	int xLowest = img->getWidth();
	int yLowest = img->getHeight();
	for (int x = 0; x<img->getWidth(); x++) 
	{
		for (int y = 0; y<img->getHeight(); y++) 
		{
			int val = img->getPixelValuei(x, y, 0);
			if (val <=100) 
			{
				if (xHighest < x)
				{
					xHighest = x;
				}
				if (yHighest < y)
				{
					yHighest = y;
				}
			if (val <=100) 
			{
				if (xLowest > x)
				{
					xLowest = x;
				}
				if (yLowest > y)
				{
					yLowest = y;
				}
			}
			}
		}
	}
	delete img;
	std::cout << "Workspace max point " << "(" << xHighest << ", " << yHighest << ")" << std::endl;
	std::cout << "Workspace min point " << "(" << xLowest << ", " << yLowest << ")" << std::endl;
	std::cout << "Workspace required is therefore a square: " << xHighest - xLowest << " by " << yHighest - yLowest << std::endl;
}

void Dd_robot::lineFunction(double x0, double y0, double x1, double y1) // this function draws from (x,y) to (x1,y1) 
{																		// goes on x-axis, then y-axis
	// x-direction
	// positive
	if (x0 < x1)
	{
		for (x0; x0 < x1; x0++)
		{
			
			drawPoint(x0, y0, 0);
		}
	}
	// negative
	if (x0 > x1)
	{
		for (x0; x0 >= x1; x0--)
		{
			drawPoint(x0, y0, 0);
		}
	}

	// y-direction
	// positive
	if (y0 < y1)
	{
		for (y0; y0 < y1; y0++)
		{
			drawPoint(x1, y0, 0);
		}
	}
	// negative
	if (y0 > y1)
	{
		
		for (y0; y0 >= y1+1; y0--)
		{
			drawPoint(x1, y0, 0);
		}
	}
}

double Dd_robot::calcRotationTime(double radians)
{
	double timeSpent = 0;
	timeSpent = (radians * circumference) / (2 * PI*abs(velocity_left)*wheelRadius);
	return timeSpent;
}

void Dd_robot::printRotationalSpeed()
{
	std::cout << "Rotational speed for the left wheel is currently: " << velocity_left / wheelRadius << std::endl;
	std::cout << "Rotational speed for the right wheel is currently: " << velocity_right / wheelRadius << std::endl;
}

void Dd_robot::drawPoint(int x, int y, int colour)
{
	std::string filename("Bane1.pgm");
	Image* img = PPMLoader::load(filename);
	if (img->getPixelValuei(x,y,0) == 100)
	{
		delete img;
		return;
	}
	img->setPixel8U(x, y, colour);
	img->saveAsPGM("Bane1.pgm");
	delete img;
}
