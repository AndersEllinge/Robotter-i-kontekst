#pragma once
class Dd_robot
{
public:
	Dd_robot();
	/**
	* Constructor for the differential drive robot.
	* Parameters are initial position (matrix)
	**/
	Dd_robot(double or_x1, double or_x2, double or_y1, double or_y2, int pos_x, int pos_y);
	~Dd_robot();

	/**
	* This function multiplies 2 matrices together
	* Result is written to new_arr
	**/	
	void matrixMulti(double a1, double a2, double b1, double b2, double c1, double c2);
	
	/**
	* This function reads the tranformation and performs the movement required, and finally rotates to correct global rotation
	**/	
	void move(double x1, double x2, double y1, double y2, int distance_x, int distance_y);
	
	/**
	* This function prints the new frame of the robot
	**/	
	void whereAreYou();
	
	/**
	* This function rotates the robot relative to global coordinates
	**/	
	void globalRotate(double x1, double x2, double y1, double y2);
	
	/**
	* This function saves the current tranformation
	* This updates the current to the previously new frame
	**/	
	void saveCurrentTransformation();
	
	/**
	* This function runs translate for x and y
	* The x and y function will also print the movement done
	* drawPoint is used here to leave a gray dot when the robot is done moving
	**/	
	void translate();
	
	/**
	* Translates for the x direction
	**/	
	void translate_x();
	
	/**
	* Translates for the y direction
	**/	
	void translate_y();
	
	/**
	* This function tests the movement of the robot, to see if it leaves the workspace
	**/	
	bool testWorkSpace(int distance_x, int distance_y);
	
	/**
	* Sets the velocity of the robot
	**/	
	void setRobotVelocity(int newVelocityLeft, int newVelocityRight);
	
	/**
	* Returns the angular velocity of the robots left wheel
	**/	
	double getRobotVelocityL();
	
	/**
	* Returns the angular velocity of the robots right wheel
	**/	
	double getRobotVelocityR();
	
	/**
	* Returns the angle between the 2 vectors input
	**/	
	double angleBetweenVectors(double x1, double x2, double y1, double y2);
	
	/**
	* This function prints the minimum required pixels for the robot to perform its movement
	* Can only be run after performing transformations
	**/	
	void findMinWorkSpace();
	
	/**
	* This function draws from (x0,y0) to (x1,y1) 
	* Starts on x-axis, then y-axis
	**/	
	void lineFunction(double x0, double y0, double x1, double y1);
	
	/**
	* This function returns the time the robot rotates, with radians as input
	**/	
	double calcRotationTime(double radians);
	
	/**
	* This function prints the current angular velocities to the command line
	**/	
	void printRotationalSpeed();
	
	/**
	* This functions draws a dot with coordinates x,y and color (int between 0-255)
	**/	
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

