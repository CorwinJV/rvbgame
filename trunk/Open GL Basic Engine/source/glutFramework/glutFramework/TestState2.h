#ifndef TESTSTATE2_H
#define TESTSTATE2_H
#include "oglUtility.h"
#include "GameState.h"
#include "pixmap.h"

class TestState2 : public GameState
{
public:
	TestState2();
	TestState2(GameStateManager &Parent, int newID);
	~TestState2();
	bool Update();
	bool Draw();

private:
	pixmap* img;
};

#endif