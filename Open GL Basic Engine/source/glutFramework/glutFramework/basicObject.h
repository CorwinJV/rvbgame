#ifndef BASICOBJECT_H
#define BASICOBJECT_H

enum direction
{
	ne, se, sw, nw;
}

class basicObject
{
private:
	double strength;
	double health;
	int movementSpeed;
	int jumpDistance;
	int climbHeight;
	direction facing;
	int xpos, ypos;

public:
	basicObject();
	~basicObject();
	bool moveForward();
	bool turnLeft();
	bool turnRight();
	bool crouch();
	bool jump();
	bool punch();
	bool activate();
	bool wait();
}

#endif