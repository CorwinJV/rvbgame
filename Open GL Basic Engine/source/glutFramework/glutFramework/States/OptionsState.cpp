#include "OptionsState.h"

bool OptionsState::Update()
{
	if(myMenu != NULL)
		myMenu->Update();
	return true;
}

bool OptionsState::Draw()
{
	if(img != NULL)
		img->drawImage();

	if(myMenu != NULL)
		myMenu->Draw();
	return false;
}

bool OptionsState::MainMenuStateCallback()
{
	GSM->addGameState<MainMenuState>();
	this->setStatus(DeleteMe);
	return true;
}

void OptionsState::processMouse(int x, int y)
{
	if(myMenu != NULL)
		myMenu->processMouse(x, y);
}

void OptionsState::processMouseClick(int button, int state, int x, int y)
{
	if(myMenu != NULL)
		myMenu->processMouseClick(button, state, x, y);
}