#ifndef LEVELSELECTSTATE_H
#define LEVELSELECTSTATE_H

#include ".\..\GameState.h" 
#include "..\GameStateManager.h"
#include ".\..\oglTexture2D.h"
#include ".\..\playGame.h"
#include "..\oglGameVars.h"
#include "MainMenuState.h"
#include <vector>

class LevelSelectState : public GameState
{
public:
	LevelSelectState() {};
	LevelSelectState(GameStateManager &Parent, int newID) : GameState(Parent, newID),
		textColumnWidth(48)
	{
		playerCurrentLevel = GameVars->getPlayerMaxLevel();
			
		//display a menu that shows info and contains advance and exit buttons		

		myMenu = new MenuSys(250, 50, "blank.png", None);
		myMenu->addButton("arrow_left.png", "arrow_lefthover.png", "arrow_lefthover.png", CreateFunctionPointer0R(this, &LevelSelectState::decrement));
		myMenu->setLastButtonDimensions(100, 100);
		myMenu->setLastButtonPosition(150, 600);
		myMenu->addButton("arrow_right.png", "arrow_righthover.png", "arrow_righthover.png", CreateFunctionPointer0R(this, &LevelSelectState::increment));
		myMenu->setLastButtonDimensions(100, 100);
		myMenu->setLastButtonPosition(775, 600);
		myMenu->addButton("buttons\\startthislevel.png", "buttons\\startthislevelhover.png", "buttons\\startthislevelhover.png", CreateFunctionPointer0R(this, &LevelSelectState::selectLevel));
		myMenu->setLastButtonDimensions(475, 100);
		myMenu->setLastButtonPosition(275, 600);
		Update();
	};

	void processMouse(int x, int y);
	void processMouseClick(int button, int state, int x, int y);
	void LevelSelectState::keyboardInput(unsigned char c, int x, int y);
	bool LevelSelectState::Update();
	bool LevelSelectState::Draw();
	bool LevelSelectState::selectLevel();
	bool LogicViewCallback();
	bool increment();
	bool decrement();
	int  getPlayerCurrentLevel();
	

private:
	int playerCurrentLevel;
	int textColumnWidth;
	vector<levelData*> levelList;
	// add private stuff here

};

#endif