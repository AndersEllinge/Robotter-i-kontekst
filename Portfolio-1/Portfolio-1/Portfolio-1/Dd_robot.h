#pragma once
class Dd_robot
{
public:
	Dd_robot();
	Dd_robot(double or_x1, double or_x2, double or_y1, double or_y2, int pos_x, int pos_y);
	~Dd_robot();

	void matrixMulti(double a1, double a2, double b1, double b2, double c1, double c2);
	void move(double x1, double x2, double y1, double y2, int distance_x, int distance_y);
	void whereAreYou();
	void globalRotate(double x1, double x2, double y1, double y2);
	void saveCurrentTransformation();
	void translate();
	void translate_x();
	void translate_y();
	bool testWorkSpace(int distance_x, int distance_y);
	void setRobotVelocity(int newVelocityLeft, int newVelocityRight);
	double getRobotVelocityL();
	double getRobotVelocityR();
	double angleBetweenVectors(double x1, double x2, double y1, double y2);
	void findMinWorkSpace();
	void lineFunction(double x0, double y0, double x1, double y1);
	double calcRotationTime(double radians);
	void printRotationalSpeed();
	void drawPoint(int x, int y, int colour);


protected:
	
	double robotOrientation;
	double wheelRadius;
	double distanceBetweenWheels;
	double velocity_left;
	double velocity_right;
	double circumference;
	double time_in_motion;
	double new_arr[3][3];
	double curr_arr[3][3];

};

