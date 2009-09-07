#ifndef deletePROFILESTATE_H
#define deletePROFILESTATE_H

#include "GameState.h" 
#include "GameStateManager.h"
#include "oglTexture2D.h"
#include "playGame.h"
#include "clickOKState.h"
#include "oglGameVars.h"
#include "MenuSystem.h"
#include <ctime>


class deleteProfileState : public GameState
{
public:
	deleteProfileState() {};
	deleteProfileState(GameStateManager &Parent, int newID) : GameState(Parent, newID)
	{
		profileViewing = 0;
		clicked = false;
		if((GameVars->PM->getMaxRecords() - 1) >= 0)
		{
			GameVars->PM->setCurrentRecord(profileViewing);
		}
	//	myMenu = new MenuSys(250, 250, "blankmenu.png", Auto);
		myMenu = new MenuSys((int)1024/2 - (int)600/2, (int)768/2 - (int)475/2 + 25, "blank.png", None);
		myMenu->addButton("arrow_left.png", "arrow_lefthover.png", "arrow_lefthover.png", CreateFunctionPointer0R(this, &deleteProfileState::decrement));
		myMenu->setLastButtonDimensions(100, 100);
		myMenu->setLastButtonPosition(150, 600);
		myMenu->addButton("arrow_right.png", "arrow_righthover.png", "arrow_righthover.png", CreateFunctionPointer0R(this, &deleteProfileState::increment));
		myMenu->setLastButtonDimensions(100, 100);
		myMenu->setLastButtonPosition(775, 600);
		myMenu->addButton("buttons\\deleteprofile.png", "buttons\\deleteprofilehover.png", "buttons\\deleteprofilehover.png", CreateFunctionPointer0R(this, &deleteProfileState::deleteProfile));
		myMenu->setLastButtonDimensions(475, 100);
		myMenu->setLastButtonPosition(275, 600);
		myMenu->addButton("buttons\\back.png", "buttons\\backhover.png", "buttons\\backhover.png", CreateFunctionPointer0R(this, &deleteProfileState::back));
		myMenu->setLastButtonDimensions(100, 50);
		myMenu->setLastButtonPosition(750, 250);
		
		Update();
	}

	void processMouse(int x, int y);
	void processMouseClick(int button, int state, int x, int y);
	void keyboardInput(unsigned char c, int x, int y);
	bool deleteProfileState::Update();
	bool deleteProfileState::Draw();
	bool increment();
	bool decrement();
	bool back();
	bool deleteProfile();
	
	
private:
	int profileViewing;
	int maxNumProfiles;
	int totScore;
	int highestLevel;
	std::string playerName; 
	clock_t timer;
	clock_t startTime;
	bool clicked;
	// add private stuff here

};

#endif