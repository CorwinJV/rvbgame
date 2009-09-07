#ifndef SAOBJECT_H
#define SAOBJECT_H
#include <vector>

using namespace std;

class SAobject
{
	private:
	// Members
	vector<SAobject*> stateList;
    //*** List<GameState2D> stateList;
	int stateCount;
	int numStates;

public:
	SAobject();
	    
	~SAobject();

	bool Update();
	bool Draw();
};

#endif