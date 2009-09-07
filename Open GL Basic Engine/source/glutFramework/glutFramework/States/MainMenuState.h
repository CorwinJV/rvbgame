#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H

#include "..\GameState.h"
#include "..\FunctionPointer.h"
#include <iostream>
#include "..\oglTexture2D.h"
#include "..\helpScreenState.h"
using namespace BE;

class MainMenuState : public GameState
{
public:
	MainMenuState() {};
	MainMenuState(GameStateManager &Parent, int newID) : GameState(Parent, newID)
	{

		myMenu = new MenuSys((int)1024/2 - (int)600/2, (int)768/2 - (int)475/2 + 25, "blankmenu.png", Auto);
		myMenu->addButton("buttons\\startnewgame.png", "buttons\\startnewgamehover.png", "buttons\\startnewgamehover.png", CreateFunctionPointer0R(this, &MainMenuState::StartNewGameCallback));
		myMenu->addButton("buttons\\profilemanagement.png", "buttons\\profilemanagementhover.png", "buttons\\profilemanagementhover.png", CreateFunctionPointer0R(this, &MainMenuState::ProfileManagerCallback));
		myMenu->addButton("buttons\\loadgame.png", "buttons\\loadgamehover.png", "buttons\\loadgamehover.png", CreateFunctionPointer0R(this, &MainMenuState::LoadGameCallback));
		myMenu->addButton("buttons\\helpwide.png", "buttons\\helpwidehover.png", "buttons\\helpwidehover.png", CreateFunctionPointer0R(this, &MainMenuState::HelpCallback));
		myMenu->addButton("buttons\\credits.png", "buttons\\creditshover.png", "buttons\\creditshover.png", CreateFunctionPointer0R(this, &MainMenuState::Credits));
		myMenu->addButton("buttons\\quit.png", "buttons\\quithover.png", "buttons\\quithover.png", CreateFunctionPointer0R(this, &MainMenuState::Quit));
	}

	bool Update();
	bool Draw();
	bool HandleClick();
	void MainMenuState::processMouse(int x, int y);
	void MainMenuState::processMouseClick(int button, int state, int x, int y);
	bool StartNewGameCallback();
	bool ProfileManagerCallback();
	bool LoadGameCallback();
	bool HelpCallback();
	bool Quit();
	bool Credits();

private:

	
};

#endif

