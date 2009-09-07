#ifndef BASICOBJECTMANAGER_H
#define BASICOBJECTMANAGER_H
#include <vector>

using namespace std;

class BasicObjectManager
{
	private:
	// Members
	vector<BasicObjectManager*> stateList;
    //*** List<GameState2D> stateList;
	int stateCount;
	int numStates;

public:
	BasicObjectManager();
	    
	~BasicObjectManager();

	bool Update();
	bool Draw();
};

#endif