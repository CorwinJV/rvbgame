#ifndef SKIPTUTORIALSPOPUPSTATE_H
#define SKIPTUTORIALSPOPUPSTATE_H

#include "GameState.h" 
#include ".\States\MainMenuState.h"
#include "oglTexture2D.h"
#include "oglGameVars.h"
#include <string>

using namespace std;

class skipTutorialsPopupState : public GameState
{
public:
	skipTutorialsPopupState();
	skipTutorialsPopupState(GameStateManager &Parent, int newID) : GameState(Parent, newID)
	{
		myMenu = new MenuSys((int)1024/2 - (int)600/2, (int)768/2 - (int)475/2 + 25, "blankmenu.png", None);
		myMenu->addButton("buttons\\yes.png", "buttons\\yeshover.png", "buttons\\yeshover.png", CreateFunctionPointer0R(this, &skipTutorialsPopupState::yesCallback));
		myMenu->setLastButtonDimensions(100, 50);
		myMenu->setLastButtonPosition((int)1024/2 - 150, 530);

		myMenu->addButton("buttons\\no.png", "buttons\\nohover.png", "buttons\\nohover.png", CreateFunctionPointer0R(this, &skipTutorialsPopupState::noCallback));
		myMenu->setLastButtonDimensions(100, 50);
		myMenu->setLastButtonPosition((int)1024/2 + 50, 530);
		Update();
	}

	void processMouse(int x, int y);
	void processMouseClick(int button, int state, int x, int y);
	bool Update();
	bool Draw();
	bool yesCallback();
	bool noCallback();


private:
	std::string tempString;
	
	// add private stuff here

};

#endif
