#ifndef CREDITSSTATE_H
#define CREDITSSTATE_H

#include "GameState.h" 
#include "GameStateManager.h"
#include "oglTexture2D.h"
#include "oglGameVars.h"
#include <cstring>

class CreditsState : public GameState
{
public:
	CreditsState() {};
	CreditsState(GameStateManager &Parent, int newID) : GameState(Parent, newID)
	{
		init();
	}

	void processMouse(int x, int y);
	void processMouseClick(int button, int state, int x, int y);
	bool CreditsState::Update();
	bool CreditsState::Draw();
	void keyboardInput(unsigned char c, int x, int y);

private:
	vector<string*> creditsList;
	void init();
	int numlines;
	int offsetY;
	int lineSpacing;
	int offsetYLimit;
	void drawCredits();
	int scrollSpeed;
	int scrollAmount;

};

#endif