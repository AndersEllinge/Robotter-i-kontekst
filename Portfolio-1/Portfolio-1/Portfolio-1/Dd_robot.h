#pragma once
class Dd_robot
{
public:
	Dd_robot();
	Dd_robot(double or_x1, double or_x2, double or_y1, double or_y2, int pos_x, int pos_y);
	~Dd_robot();

	void move(double x1, double x2, double y1, double y2, int distance_x, int distance_y);
	void whereAreYou();
	void rotate(double x1, double x2, double y1, double y2);
	void globalRotate(double x1, double x2, double y1, double y2);
	void setPreviousRotation();
	void translate(int distance_x, int distance_y);
	void translate_x(int distance_x);
	void translate_y(int distance_y);
protected:
	
	double orientation_x1;
	double orientation_x2;
	double orientation_y1;
	double orientation_y2;

	double prev_orientation_x1;
	double prev_orientation_x2;
	double prev_orientation_y1;
	double prev_orientation_y2;

	int position_x;
	int position_y;

};

