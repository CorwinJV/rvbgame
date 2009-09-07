#ifndef RVB_MAINSTATE_H
#define RVB_MAINSTATE_H

#include "GameState.h"
#include "oglGameVars.h"
#include "RVB_Map.h"
#include <iostream>


class RVB_MainState : public GameState
{
public:
	RVB_MainState(GameStateManager &Parent, int newID) : GameState(Parent, newID)
	{
		init();
	}
	~RVB_MainState();

private:
	static const int mapSize = 20;
	void init();
	RVB_Map* currentMap;

	int panRate;
	int clickedX;
	int clickedY;
	int curMouseX;
	int curMouseY;

	RVB_Entity* selectedEntity;

public:
	bool Update();
	bool Draw();

	void RVB_MainState::processMouse(int x, int y);
	void RVB_MainState::processMouseClick(int button, int state, int x, int y);
	void RVB_MainState::keyboardInput(unsigned char c, int x, int y);
};


#endif // RVB_MAINSTATE_H