#ifndef TEMPSTATE_H
#define TEMPSTATE_H

#include "GameState.h" 
#include "oglUtility.h"
//#include "pixmap.h"
#include "oglTexture2D.h"


class tempstate : public GameState
{
private:
	oglTexture2D* logo;
	int timer;


public:

	tempstate() {};
	tempstate(GameStateManager &Parent, int newID) : GameState(Parent, newID), logo(NULL)
	{
		logo = new oglTexture2D();
		if(logo != NULL)
			logo->loadImage("button4over.png", 1024, 768);

		//logo->mX = -500;
		//logo->mY = 500;
		timer = 0;
	}

	bool tempstate::Update();
	bool tempstate::Draw();

};


#endif

