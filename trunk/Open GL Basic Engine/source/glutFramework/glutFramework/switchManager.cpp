#include "switchManager.h"

// clear the list
void switchManager::clearSwitchList()
{
	switchList.clear();
}

// building the list

////////////////////////////
// addSwitch
void switchManager::addSwitch(int mapXPos, int mapYPos, int numTargets)
{
	if(lastSwitchAddedTargets != lastSwitchAddedTargetsAdded)
	{
		std::cout << "MAP ERROR: A switch was added without a full target list" << endl;
	}
	Oswitch* tempSwitch = new Oswitch(mapXPos, mapYPos);
	lastSwitchAddedTargets = numTargets;
	lastSwitchAddedTargetsAdded = 0;
	switchList.push_back(tempSwitch);
}

////////////////////////////
// addTargetToLastSwitch
void switchManager::addTargetToLastSwitch(int targetX, int targetY)
{
	switchList.back()->addTarget(targetX, targetY);
	lastSwitchAddedTargetsAdded++;
}
	
// getting info

////////////////////////////
// isThereASwitchAt
bool switchManager::isThereASwitchAt(int mapXPos, int mapYPos)
{
	// lets iterate through the list and see if the requested position has a switch
	vector<Oswitch*>::iterator switchIter = switchList.begin();

	for(;switchIter < switchList.end(); switchIter++)
	{
		if(((*switchIter)->getXPos() == mapXPos) && ((*switchIter)->getYPos() == mapYPos))
		{
			// we have a match
			return true;
		}
	}
	// oh noes
	return false;
}

////////////////////////////
// getNumTargets
int switchManager::getNumTargets(int mapXPos, int mapYPos)
{
	// lets iterate through the list, see what's at our position and find out how many targets this sucker has
	vector<Oswitch*>::iterator switchIter = switchList.begin();

	for(;switchIter < switchList.end(); switchIter++)
	{
		if(((*switchIter)->getXPos() == mapXPos) && ((*switchIter)->getYPos() == mapYPos))
		{
			// houston we have a match
			// now lets return the number of targets this puppy has
			return (*switchIter)->getNumTargets();
		}
	}
	return -1;
}

////////////////////////////
// getCurrentTargetX
int switchManager::getCurrentTargetX(int mapXPos, int mapYPos)
{
	// lets iterate through the list, see what's at our position and find out it's current targetx
	vector<Oswitch*>::iterator switchIter = switchList.begin();

	for(;switchIter < switchList.end(); switchIter++)
	{
		if(((*switchIter)->getXPos() == mapXPos) && ((*switchIter)->getYPos() == mapYPos))
		{
			return (*switchIter)->getTargetX();
		}
	}
	return -1;
}

////////////////////////////
// getCurrentTargetY
int switchManager::getCurrentTargetY(int mapXPos, int mapYPos)
{
	// lets iterate through the list, see what's at our position and find out it's current targetx
	vector<Oswitch*>::iterator switchIter = switchList.begin();

	for(;switchIter < switchList.end(); switchIter++)
	{
		if(((*switchIter)->getXPos() == mapXPos) && ((*switchIter)->getYPos() == mapYPos))
		{
			return (*switchIter)->getTargetY();
		}
	}
	return -1;
}

// management sorta

////////////////////////////
// advanceTarget
void switchManager::advanceTarget(int mapXPos, int mapYPos)
{
	// lets iterate through the list, see what's at our position and cycle its targets
	vector<Oswitch*>::iterator switchIter = switchList.begin();

	for(;switchIter < switchList.end(); switchIter++)
	{
		if(((*switchIter)->getXPos() == mapXPos) && ((*switchIter)->getYPos() == mapYPos))
		{
			(*switchIter)->cycleTargets();
			targetsProcessed++;
			////
			//std::cout << "targets processed = " << targetsProcessed << " targetstoprocess = " << targetsToProcess << endl;
		}
	}
}

////////////////////////////
// startProcessing
void switchManager::startProcessing(int mapXPos, int mapYPos)
{
	// lets iterate through the list, see what's at our position and find out it's current targets
	vector<Oswitch*>::iterator switchIter = switchList.begin();

	for(;switchIter < switchList.end(); switchIter++)
	{
		if(((*switchIter)->getXPos() == mapXPos) && ((*switchIter)->getYPos() == mapYPos))
		{
			targetsToProcess = (*switchIter)->getNumTargets();
			targetsProcessed = 0;
			/////
			//std::cout << "started processing switch with " << targetsToProcess << " targets" <<endl;
		}
	}
	targetsProcessed = 0;
}

////////////////////////////
// doneProcessing
bool switchManager::doneProcessing()
{
	if(targetsProcessed >= targetsToProcess)
	{
		return true;
	}
	else
	{
		return false;
	}
}

////////////////////////////
// resetAllSwitches
void switchManager::resetAllSwitches()
{
	vector<Oswitch*>::iterator switchIter = switchList.begin();

	for(;switchIter < switchList.end(); switchIter++)
	{
		(*switchIter)->reset();
	}
}

////////////////////////////
// rotateLeft
void switchManager::rotateLeft(int mapWidth, int mapHeight)
{
	vector<Oswitch*>::iterator switchIter = switchList.begin();

	int tempX;
	int tempY;
	int tempTargetX;
	int tempTargetY;
	int numControlled;

	for(;switchIter < switchList.end(); switchIter++)
	{
		// rotate the switch
		tempX = (*switchIter)->getXPos();
		tempY = (*switchIter)->getYPos();
		(*switchIter)->setXPos(tempY);
		(*switchIter)->setYPos(mapWidth-tempX-1);

		numControlled = (*switchIter)->getNumTargets();
		// now rotate the targets
		for(int x = 0; x < numControlled; x++)
		{
			tempTargetX = (*switchIter)->getTargetX();
			tempTargetY = (*switchIter)->getTargetY();
			(*switchIter)->setTargetX(tempTargetY);
			(*switchIter)->setTargetY(mapWidth-tempTargetX-1);
			(*switchIter)->cycleTargets();
		}
	}
}

////////////////////////////
// rotateRight
void switchManager::rotateRight(int mapWidth, int mapHeight)
{
	vector<Oswitch*>::iterator switchIter = switchList.begin();

	int tempX;
	int tempY;
	int tempTargetX;
	int tempTargetY;
	int numControlled;

	for(;switchIter < switchList.end(); switchIter++)
	{
		// rotate the switch
		tempX = (*switchIter)->getXPos();
		tempY = (*switchIter)->getYPos();
		(*switchIter)->setXPos(mapHeight - tempY-1);
		(*switchIter)->setYPos(tempX);

		numControlled = (*switchIter)->getNumTargets();
		// now rotate the targets
		for(int x = 0; x < numControlled; x++)
		{
			tempTargetX = (*switchIter)->getTargetX();
			tempTargetY = (*switchIter)->getTargetY();
			(*switchIter)->setTargetX(mapHeight - tempTargetY - 1);
			(*switchIter)->setTargetY(tempTargetX);
			(*switchIter)->cycleTargets();
		}
	}

}