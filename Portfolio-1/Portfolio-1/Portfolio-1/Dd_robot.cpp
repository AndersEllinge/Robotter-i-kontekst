#include "Dd_robot.h"
#include <cmath>
#include <iostream>

#define PI 3.14159265

Dd_robot::Dd_robot()
{
}

Dd_robot::Dd_robot(double or_x1, double or_x2, double or_y1, double or_y2, int pos_x, int pos_y)
{
	orientation_x1 = or_x1;
	orientation_x2 = or_x2;
	orientation_y1 = or_y1;
	orientation_y2 = or_y2;
	position_x = pos_x;
	position_y = pos_y;
	prev_orientation_x1 = 0;
	prev_orientation_x2 = 0;
	prev_orientation_y1 = 0;
	prev_orientation_y2 = 0;
}

Dd_robot::~Dd_robot()
{
}

void Dd_robot::move(double x1, double x2, double y1, double y2, int distance_x, int distance_y)
{
	translate(distance_x, distance_y);

	if (orientation_x1 == x1 && orientation_x2 == x2 && orientation_y1 == y1 && orientation_y2 == y2)
	{
		return;
	}
	rotate(acos(x1), asin(x2), asin(y1), acos(y2));
}

void Dd_robot::whereAreYou()
{
	std::cout << "x-position is: " << position_x << std::endl;
	std::cout << "y-position is: " << position_y << std::endl;
	std::cout << "Orientation x1 is: " << orientation_x1 << std::endl;
	std::cout << "Orientation x2 is: " << orientation_x2 << std::endl;
	std::cout << "Orientation y1 is: " << orientation_y1 << std::endl;
	std::cout << "Orientation y2 is: " << orientation_y2 << std::endl;
}

void Dd_robot::rotate(double x1, double x2, double y1, double y2)
{
	double temp1, temp2, temp3, temp4;

	temp1 = acos(orientation_x1);
	orientation_x1 = cos(temp1 + x1);
	temp2 = asin(orientation_x2);
	orientation_x2 = sin(temp2 + x2);
	temp3 = asin(orientation_y1);
	orientation_y1 = sin(temp3 + y1);
	temp4 = acos(orientation_y2);
	orientation_y2 = cos(temp4 + y2);

	if (orientation_x1 < 0.001 && orientation_x1 > -0.001)
	{
		orientation_x1 = 0;
	}
	if (orientation_x2 < 0.001 && orientation_x2 > -0.001)
	{
		orientation_x2 = 0;
	}
	if (orientation_y1 < 0.001 && orientation_y1 > -0.001)
	{
		orientation_y1 = 0;
	}
	if (orientation_y2 < 0.001 && orientation_y2 > -0.001)
	{
		orientation_y2 = 0;
	}
}

void Dd_robot::globalRotate(double x1, double x2, double y1, double y2)
{
	prev_orientation_x1 = orientation_x1;
	prev_orientation_x2 = orientation_x2;
	prev_orientation_y1 = orientation_y1;
	prev_orientation_y2 = orientation_y2;

	orientation_x1 = cos(x1);
	orientation_x2 = sin(x2);
	orientation_y1 = 0 - sin(y1);
	orientation_y2 = cos(y2);

	if (orientation_x1 < 0.001 && orientation_x1 > -0.001)
	{
		orientation_x1 = 0;
	}
	if (orientation_x2 < 0.001 && orientation_x2 > -0.001)
	{
		orientation_x2 = 0;
	}
	if (orientation_y1 < 0.001 && orientation_y1 > -0.001)
	{
		orientation_y1 = 0;
	}
	if (orientation_y2 < 0.001 && orientation_y2 > -0.001)
	{
		orientation_y2 = 0;
	}
}

void Dd_robot::setPreviousRotation()
{
	orientation_x1 = prev_orientation_x1;
	orientation_x2 = prev_orientation_x2;
	orientation_y1 = prev_orientation_y1;
	orientation_y2 = prev_orientation_y2;
}

void Dd_robot::translate(int distance_x, int distance_y)
{
	translate_x(distance_x);
	translate_y(distance_y);
	
}

void Dd_robot::translate_x(int distance_x)
{
	if (distance_x == 0)
	{
		return;
	}
	if (distance_x >= 0)
	{
		if (orientation_x1 == 1 && orientation_x2 == 0
			&& orientation_y1 == 0 && orientation_y2 == 1)
		{
			position_x += distance_x;
		}
		else
		{
			globalRotate(0, 0, 0, 0);
			translate_x(distance_x);
			setPreviousRotation();
		}
	}
	if (distance_x <= 0)
	{
		if (orientation_x1 == -1 && orientation_x2 == 0
			&& orientation_y1 == 0 && orientation_y2 == -1)
		{
			position_x -= distance_x;
		}
		else
		{
			globalRotate(180 * PI / 180, 180 * PI / 180, 180 * PI / 180, 180 * PI / 180);
			translate_x(distance_x);
			setPreviousRotation();
		}
	}
}

void Dd_robot::translate_y(int distance_y)
{
	if (distance_y == 0)
	{
		return;
	}
	if (distance_y >= 0)
	{
		if (orientation_x1 == 0 && orientation_x2 == 1
			&& orientation_y1 == -1 && orientation_y2 == 0)
		{
			position_y += distance_y;
		}
		else
		{
			globalRotate(90 * PI / 180, 90 * PI / 180, 90 * PI / 180, 90 * PI / 180);
			translate_y(distance_y);
			setPreviousRotation();
		}
	}
	if (distance_y <= 0)
	{
		if (orientation_x1 == 0 && orientation_x2 == -1
			&& orientation_y1 == 1 && orientation_y2 == 0)
		{
			position_y += distance_y;
		}
		else
		{
			globalRotate(270 * PI / 180, 270 * PI / 180, 270 * PI / 180, 270 * PI / 180);
			translate_y(distance_y);
			setPreviousRotation();
		}
	}
}



