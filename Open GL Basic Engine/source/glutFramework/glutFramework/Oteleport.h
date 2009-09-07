#ifndef OTELEPORT_H
#define OTELEPORT_H

#include "oglTexture2D.h"
#include "aiInstructionEnums.h"
#include <vector>
#include "objectEnums.h"
#include "object.h"

using namespace std;

class Oteleport : public object
{
	int targetX;
	int targetY;
	int selfX;
	int selfY;

public:
	Oteleport();
	Oteleport(int x, int y, int newDirection = 0, objectEnum newType = OTeleport) 
		: object(x, y, newDirection, newType)
	{
		selfX = x;
		selfY = y;
		//std::cout << "switch created at " << x << ", " << y << endl;
	}
	void setPosition(int x, int y)
	{
		selfX = x;
		selfY = y;
	}

	void setTarget(int x, int y)
	{
		targetX = x;
		targetY = y;
	}
	int getTargetX()
	{
		return targetX;
	}
	int getTargetY() 
	{
		return targetY;
	}
};

#endif