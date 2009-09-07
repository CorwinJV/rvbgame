#include"christianState.h"
christianState::christianState(GameStateManager &Parent, int newID)
: GameState()
{
	GSM = &Parent;
	stateID = newID;
}

bool christianState::Update()
{
	MyObjectM.Update();
	return true;
}

bool christianState::Draw()
{	
	MyObjectM.Draw();
	return true;
}