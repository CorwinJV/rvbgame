#ifndef PAUSEGAMESTATE_H
#define PAUSEGAMESTATE_H

#include ".\..\GameState.h" 
#include "..\GameStateManager.h"
#include ".\..\oglTexture2D.h"
#include "SaveGameState.h"
#include "LevelSelectState.h"
#include "MainMenuState.h"
#include ".\..\clickOKState.h"

class PauseGameState : public GameState
{
public:
	PauseGameState() {};
	PauseGameState(GameStateManager &Parent, int newID) : GameState(Parent, newID)
	{
		
		myMenu = new MenuSys(212, 100, "blankmenu.png", Auto);
		myMenu->addButton("buttons\\savegame.png", "buttons\\savegamehover.png", "buttons\\savegamehover.png", CreateFunctionPointer0R(this, &PauseGameState::SaveCallback));
		myMenu->addButton("buttons\\saveandquit.png", "buttons\\saveandquithover.png", "buttons\\saveandquithover.png", CreateFunctionPointer0R(this, &PauseGameState::SaveQuitCallback));
		myMenu->addButton("buttons\\levelselect.png", "buttons\\levelselecthover.png", "buttons\\levelselecthover.png", CreateFunctionPointer0R(this, &PauseGameState::LevelSelectCallback));
		myMenu->addButton("buttons\\helpwide.png", "buttons\\helpwidehover.png", "buttons\\helpwidehover.png", CreateFunctionPointer0R(this, &PauseGameState::HelpCallback));
		myMenu->addButton("buttons\\returntomainmenu.png", "buttons\\returntomainmenuhover.png", "buttons\\returntomainmenuhover.png", CreateFunctionPointer0R(this, &PauseGameState::MainMenuCallback));
		myMenu->addButton("buttons\\returntogame.png", "buttons\\returntogamehover.png", "buttons\\returntogamehover.png", CreateFunctionPointer0R(this, &PauseGameState::ReturnToGameCallback));
		myMenu->addButton("buttons\\quit.png", "buttons\\quithover.png", "buttons\\quithover.png", CreateFunctionPointer0R(this, &PauseGameState::QuitCallback));

	}

	void processMouse(int x, int y);
	void processMouseClick(int button, int state, int x, int y);
	bool PauseGameState::Update();
	bool PauseGameState::Draw();
	bool SaveCallback();
	bool SaveQuitCallback();
	bool QuitCallback();
	bool LevelSelectCallback();
	bool MainMenuCallback();
	bool ReturnToGameCallback();
	bool HelpCallback();
	void keyboardInput(unsigned char c, int x, int y);

private:

};

#endif
