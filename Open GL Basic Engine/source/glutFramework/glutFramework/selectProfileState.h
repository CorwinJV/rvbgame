#ifndef SELECTPROFILESTATE_H
#define SELECTPROFILESTATE_H

#include "GameState.h" 
#include "GameStateManager.h"
#include "oglTexture2D.h"
#include "playGame.h"
#include "clickOKState.h"
#include "oglGameVars.h"


class selectProfileState : public GameState
{
public:
	selectProfileState() {};
	selectProfileState(GameStateManager &Parent, int newID) : GameState(Parent, newID)
	{
		maxNumProfiles = GameVars->PM->getMaxRecords() - 1;
		profileViewing = 0;
		if(maxNumProfiles >= 0)
		{
			GameVars->PM->setCurrentRecord(profileViewing);
		}
	//	myMenu = new MenuSys((int)1024/2 - (int)600/2, (int)768/2 - (int)475/2 + 25, "blankmenu.png", Auto);
		myMenu = new MenuSys(256, 50, "blank.png", None);
		myMenu->addButton("arrow_left.png", "arrow_lefthover.png", "arrow_lefthover.png", CreateFunctionPointer0R(this, &selectProfileState::decrement));
		myMenu->setLastButtonDimensions(100, 100);
		myMenu->setLastButtonPosition(150, 600);
		myMenu->addButton("arrow_right.png", "arrow_righthover.png", "arrow_righthover.png", CreateFunctionPointer0R(this, &selectProfileState::increment));
		myMenu->setLastButtonDimensions(100, 100);
		myMenu->setLastButtonPosition(775, 600);
		myMenu->addButton("buttons\\selectprofile.png", "buttons\\selectprofilehover.png", "buttons\\selectprofilehover.png", CreateFunctionPointer0R(this, &selectProfileState::selectProfile));
		myMenu->setLastButtonDimensions(475, 100);
		myMenu->setLastButtonPosition(275, 600);
		myMenu->addButton("buttons\\back.png", "buttons\\backhover.png", "buttons\\backhover.png", CreateFunctionPointer0R(this, &selectProfileState::back));
		myMenu->setLastButtonDimensions(100, 50);
		myMenu->setLastButtonPosition(750, 250);
		
		Update();
	}

	void processMouse(int x, int y);
	void processMouseClick(int button, int state, int x, int y);
	void keyboardInput(unsigned char c, int x, int y);
	bool selectProfileState::Update();
	bool selectProfileState::Draw();
	void selectProfileState::setPlayerInfo(std::string name, int, int, int);
	bool increment();
	bool decrement();
	bool back();
	bool selectProfile();
	
	
private:
	int profileViewing;
	int maxNumProfiles;
	int totScore;
	int highestLevel;
	std::string playerName; 
	// add private stuff here

};

#endif