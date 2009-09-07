#ifndef SAVEGAMESTATE_H
#define SAVEGAMESTATE_H

#include ".\..\GameState.h" 
#include "..\GameStateManager.h"
#include ".\..\oglTexture2D.h"
#include ".\..\oglGameVars.h"

class SaveGameState : public GameState
{
public:
	SaveGameState() {};
	SaveGameState(GameStateManager &Parent, int newID) : GameState(Parent, newID)
	{

		myMenu = new MenuSys(250, 50, "blankmenu.png", Auto);
		myMenu->addButton("buttons\\returntogame.png", "buttons\\returntogamehover.png", "buttons\\returntogamehover.png", CreateFunctionPointer0R(this, &SaveGameState::PauseGameCallback));
		Update();
	}

	void processMouse(int x, int y);
	void processMouseClick(int button, int state, int x, int y);
	void SaveGameState::keyboardInput(unsigned char c, int x, int y);
	bool SaveGameState::Update();
	bool SaveGameState::Draw();
	bool PauseGameCallback();
	bool SaveGameState::SavePlayerGame();
	bool saved;

private:

};

#endif