#ifndef LOADGAMESTATE_H
#define LOADGAMESTATE_H

#include ".\..\GameState.h" 
#include "MainMenuState.h"
#include "LevelSelectState.h"
#include "..\playGame.h"
#include ".\..\oglTexture2D.h"
#include<fstream>
#include<iostream>
#include <string>

using namespace std;

class LoadGameState : public GameState
{
public:
	LoadGameState() {};
	LoadGameState(GameStateManager &Parent, int newID) : GameState(Parent, newID)
	{
		myMenu = new MenuSys((int)1024/2 - (int)600/2, (int)768/2 - (int)475/2 + 25, "blankmenu.png", Auto);
		myMenu->addButton("buttons\\returntomainmenu.png", "buttons\\returntomainmenuhover.png", "buttons\\returntomainmenuhover.png", CreateFunctionPointer0R(this, &LoadGameState::MainMenuCallback));
		myMenu->addButton("buttons\\playgame.png", "buttons\\playgamehover.png", "buttons\\playgamehover.png", CreateFunctionPointer0R(this, &LoadGameState::playGameCallback));
		myMenu->addButton("buttons\\levelselect.png", "buttons\\levelselecthover.png", "buttons\\levelselecthover.png", CreateFunctionPointer0R(this, &LoadGameState::levelSelectCallback));

		finished = 0;

		Update();
	}

	void processMouse(int x, int y);
	void processMouseClick(int button, int state, int x, int y);
	void LoadGameState::keyboardInput(unsigned char c, int x, int y);
	bool LoadGameState::Update();
	bool LoadGameState::Draw();
	bool MainMenuCallback();
	bool playGameCallback();
	bool levelSelectCallback();
	bool loadGame(string);

private:
	std::string tempString;
	int finished;
	// add private stuff here

};

#endif