#ifndef OPTIONSSTATE_H
#define OPTIONSSTATE_H

#include ".\..\GameState.h"
//#include ".\..\pixmap.h"
#include "MainMenuState.h"
#include "..\playGame.h"
#include ".\..\oglTexture2D.h"
#include "..\GameStateManager.h"

class OptionsState : public GameState
{
public:
	OptionsState() {};
	OptionsState(GameStateManager &Parent, int newID) : GameState(Parent, newID)
	{
		img = new oglTexture2D();
		if(img != NULL)
			img->loadImage("statescreens\\options.png", 1024, 120);
		img->mY = 618;

		myMenu = new MenuSys(250, 50, "blankmenu.png", Auto);
		myMenu->addButton("buttons\\returntomainmenu.png", "button1down.png", "button1over.png", CreateFunctionPointer0R(this, &OptionsState::MainMenuStateCallback));
		Update();
	}

	bool OptionsState::Update();
	bool OptionsState::Draw();
	void processMouse(int x, int y);
	void processMouseClick(int button, int state, int x, int y);
	bool MainMenuStateCallback();

private:
	oglTexture2D* img;
	// add private stuff here

};

#endif