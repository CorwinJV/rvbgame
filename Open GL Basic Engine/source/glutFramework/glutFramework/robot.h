#ifndef ROBOT_H
#define ROBOT_H

#include "oglTexture2D.h"
#include "aiInstructionEnums.h"
#include <vector>
#include "objectEnums.h"
#include "object.h"

using namespace std;

class robot : public object
{
public:
	robot();
	robot(int x, int y, int newDirection = 0, objectEnum newType = ORobot) 
		: object(x, y, newDirection, newType)
	{
		alive = true;
	}
};

#endif