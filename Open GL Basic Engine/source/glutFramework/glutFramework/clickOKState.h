#ifndef CLICKOKSTATE_H
#define CLICKOKSTATE_H

#include "GameState.h" 
#include ".\States\MainMenuState.h"
#include "oglTexture2D.h"
#include "oglGameVars.h"
#include<fstream>
#include<iostream>
#include <string>

using namespace std;

enum clickOKenum{
				selected,		// 0
				deleted,		// 1
				created,  		// 2
				noProfile, 		// 3
				saved, 			// 4
				reprogram};		// 5

class clickOKState : public GameState
{
public:
	clickOKState();
	clickOKState(GameStateManager &Parent, int newID) : GameState(Parent, newID)
	{
		myMenu = new MenuSys((int)1024/2 - (int)600/2, (int)768/2 - (int)475/2 + 25, "blankmenu.png", None);
		myMenu->addButton("buttons\\ok.png", "buttons\\okhover.png", "buttons\\okhover.png", CreateFunctionPointer0R(this, &clickOKState::clickOKCallback), (int)1024/2 - 50, 385, 100, 50);
		//myMenu->setLastButtonDimensions(100, 50);
		//myMenu->setLastButtonPosition((int)1024/2 - 50, 385);

		myMenu->addButton("buttons\\help.png", "buttons\\helphover.png", "buttons\\helphover.png", CreateFunctionPointer0R(this, &clickOKState::helpCallback), (int)1024/2 - 50, 530, 100, 50);
		//myMenu->setLastButtonDimensions(100, 50);
		//myMenu->setLastButtonPosition((int)1024/2 - 50, 530);
	
		Update();
	}

	void processMouse(int x, int y);
	void processMouseClick(int button, int state, int x, int y);
	bool clickOKState::Update();
	bool clickOKState::Draw();
	bool clickOKCallback();
	bool helpCallback();
	clickOKenum check;


private:
	std::string tempString;
	
	// add private stuff here

};

#endif
