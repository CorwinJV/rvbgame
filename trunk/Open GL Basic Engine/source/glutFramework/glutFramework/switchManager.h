#include "Oswitch.h"


class switchManager
{
private:
	vector<Oswitch*> switchList;
	
	int lastSwitchAddedTargets;
	int lastSwitchAddedTargetsAdded;

	int targetsToProcess;
	int targetsProcessed;
	bool stillProcessing;

public:
	switchManager()
	{
		lastSwitchAddedTargets = 0;
		lastSwitchAddedTargetsAdded = 0;
	};
	~switchManager()
	{
		clearSwitchList();
	};
	// clear the list
	void clearSwitchList();

	// building the list
	void addSwitch(int mapXPos, int mapYPos, int numTargets);
	void addTargetToLastSwitch(int targetX, int targetY);
	
	// getting info
	bool isThereASwitchAt(int mapXPos, int mapYPos);
	int getNumTargets(int mapXPos, int mapYPos);
	int getCurrentTargetX(int mapXPos, int mapYPos);
	int getCurrentTargetY(int mapXPos, int mapYPos);

	// management sorta
	void advanceTarget(int mapXPos, int mapYPos);
	
	// processing management
	void startProcessing(int mapXPos, int mapYPos);
	bool doneProcessing();

	// resetting
	void resetAllSwitches();

	// map rotation support!
	void rotateLeft(int mapWidth, int mapHeight);
	void rotateRight(int mapWidth, int mapHeight);
};