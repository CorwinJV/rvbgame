#ifndef CHRISTIANSTATE_H
#define CHRISTIANSTATE_H
#include "GameState.h"
#include "oglUtility.h"
#include"BasicObjectManager.h"

class christianState : public GameState
{
public:
	christianState();
	christianState(GameStateManager &Parent, int newID);
	~christianState();

	bool Update();
	bool Draw();
	
private:
	BasicObjectManager MyObjectM;
};

#endif