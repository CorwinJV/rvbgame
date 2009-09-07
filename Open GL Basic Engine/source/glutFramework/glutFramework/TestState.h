#ifndef TESTSTATE_H
#define TESTSTATE_H
#include "oglUtility.h"
#include "GameState.h"
#include "pixmap.h"
#include "TestState2.h"

class TestState : public GameState
{
public:
	TestState();
	TestState(GameStateManager &Parent, int newID);
	~TestState();
	bool Update();
	bool Draw();

private:
	pixmap* img;
};

#endif