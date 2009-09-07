#ifndef PROFILEMGRSTATE_H
#define PROFILEMGRSTATE_H

#include ".\..\GameState.h" 
#include ".\..\GameStateManager.h"
#include ".\..\oglTexture2D.h"
#include "MainMenuState.h"
#include "StartNewGameState.h"
#include "LoadGameState.h"
#include ".\..\playGame.h"
#include ".\..\clickOKState.h"
#include ".\..\selectProfileState.h"
#include ".\..\createProfileState.h"
#include ".\..\deleteProfileState.h"


class ProfileMgrState : public GameState
{
public:
	ProfileMgrState() {};
	ProfileMgrState(GameStateManager &Parent, int newID) : GameState(Parent, newID)
	{
		creatingProfile = false; 
		checked = 0;
		int numProfiles = GameVars->PM->getMaxRecords();
		myMenu = new MenuSys((int)1024/2 - (int)600/2, (int)768/2 - (int)475/2 + 25, "blankmenu.png", Auto);
		myMenu->addButton("buttons\\createprofile.png", "buttons\\createprofilehover.png", "buttons\\createprofilehover.png", CreateFunctionPointer0R(this, &ProfileMgrState::CreateProfile));
		if(numProfiles > 0)
		{
			myMenu->addButton("buttons\\selectprofile.png", "buttons\\selectprofilehover.png", "buttons\\selectprofilehover.png", CreateFunctionPointer0R(this, &ProfileMgrState::SelectProfile));
			myMenu->addButton("buttons\\deleteprofile.png", "buttons\\deleteprofilehover.png", "buttons\\deleteprofilehover.png", CreateFunctionPointer0R(this, &ProfileMgrState::DeleteProfile));
		}
		myMenu->addButton("buttons\\returntomainmenu.png", "buttons\\returntomainmenuhover.png", "buttons\\returntomainmenuhover.png", CreateFunctionPointer0R(this, &ProfileMgrState::MainMenuStateCallback));
		Update();
	}

	void processMouse(int x, int y);
	void processMouseClick(int button, int state, int x, int y);
	/*void ProfileMgrState::keyboardInput(unsigned char c, int x, int y);*/
	bool ProfileMgrState::Update();
	bool ProfileMgrState::Draw();
	bool MainMenuStateCallback();
	bool DeleteProfile();
	bool CreateProfile();
	bool SelectProfile();
	//bool ProfileMgrState::doesNameAlreadyExists(std::string);
	//void ProfileMgrState::setPlayerInfo(std::string name, int, int, int);
	
	int checked;
	std::string tempString; 

private:
	int maxLevels;
	int numProfiles;
	bool creatingProfile;
	// add private stuff here

};

#endif