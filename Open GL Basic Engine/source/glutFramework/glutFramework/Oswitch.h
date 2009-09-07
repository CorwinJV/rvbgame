#ifndef OSWITCH_H
#define OSWITCH_H

#include "oglTexture2D.h"
#include "aiInstructionEnums.h"
#include <vector>
#include "objectEnums.h"
#include "object.h"

using namespace std;

struct targetSquare
{
	int targetX;
	int targetY;
};

class Oswitch : public object
{
	vector<targetSquare*> targetList;
	vector<int> targetX;
	vector<int> targetY;
	vector<int>::iterator Xitr;
	vector<int>::iterator Yitr;
	int numTargets;
	
public:
	Oswitch();
	Oswitch(int x, int y, int newDirection = 0, objectEnum newType = OSwitch) 
		: object(x, y, newDirection, newType)
	{
		numTargets = 0;
		targetX.clear();
		targetY.clear();
		Xitr = targetX.begin();
		Yitr = targetY.begin();
		//std::cout << "switch created at " << x << ", " << y << endl;
	}
	void addTarget(int x, int y);
	int getNumTargets();
	void cycleTargets();
	int getNextX();
	int getNextY();
	int getTargetX();
	int getTargetY();
	void setTargetX(int newX);
	void setTargetY(int newY);
	void reset();

	
};

#endif