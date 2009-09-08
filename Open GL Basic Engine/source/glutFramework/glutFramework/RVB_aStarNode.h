#ifndef RVB_ASTARNODE_H
#define RVB_ASTARNODE_H
#include <stdlib.h>

class aStarNode
{
private:
	bool available;					// 
	aStarNode* parent;				// how did we get to where we are now?
	int manhattanDistance;			// manhattan distance to target
	int steps;						// how many steps have been done so far?
	int hueristicValue;				// hueristic value
	int myX;						// my X
	int myY;						// my Y
	int endX;						// end tile's x
	int endY;						// end tile's y
	double movementCost;			// cost to move into this tile

public:
	aStarNode(int myX_n, int myY_n, int endX_n, int endY_n, bool available_n)
	{
		myX = myX_n;
		myY = myY_n;
		endX = endX_n;
		endY = endY_n;
		movementCost = 0;
		manhattanDistance = -1;
		parent = NULL;
		available = available_n;
	};

	~aStarNode()
	{

	}

	void resetSameTarget(bool available_n)
	{
		movementCost = 0;
		parent = NULL;
		available = available_n;
	}

	void resetNewTarget(int endX_n, int endY_n, bool available_n)
	{
		endX = endX_n;
		endY = endY_n;
		movementCost = 0;
		manhattanDistance = -1;
		parent = NULL;
		available = available_n;
	}

	aStarNode* getParent()
	{
		return parent;
	}

	void setParent(aStarNode* parentNode, int steps_n)
	{
		// if its available, go ahead and set parent
		if(available)
		{
			manhattanDistance = abs(myX - endX) + abs(myY - endY);
			parent = parentNode;
			steps = steps_n;
			hueristicValue = steps + manhattanDistance;
			available = false;
		}
		// otherwise do nothing!

	};

	bool getAvailable()
	{
		return available;
	}

	int getManhattanDistance()
	{
		return manhattanDistance;
	};

	int getSteps()
	{
		return steps;
	};

	int getHueristicValue()
	{
		return hueristicValue;
	};
	int getX()
	{
		return myX;
	};
	int getY()
	{
		return myY;
	};
};
#endif