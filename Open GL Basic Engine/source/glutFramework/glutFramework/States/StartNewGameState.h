#ifndef STARTNEWGAMESTATE_H
#define STARTNEWGAMESTATE_H

#include ".\..\GameState.h" 
//#include ".\..\pixmap.h"
#include "MainMenuState.h"
#include "..\playGame.h"
#include ".\..\oglTexture2D.h"
#include<fstream>
#include<iostream>
#include <string>

using namespace std;

class StartNewGameState : public GameState
{
public:
	StartNewGameState() {};
	StartNewGameState(GameStateManager &Parent, int newID) : GameState(Parent, newID)
	{
		finished = false;
		checked = false;
		Update();
	}

	bool StartNewGameState::Update();
	bool StartNewGameState::Draw();

	void processMouse(int x, int y);
	void processMouseClick(int button, int state, int x, int y);
	void keyboardInput(unsigned char c, int x, int y);
	/*bool PreGameCallback();
	bool tutorialCallback();*/
	void setPlayerName(string);
	bool doesNameAlreadyExists(string);
	
	string tempString;

private:
	bool finished;
	bool checked;
	// add private stuff here
};

#endif