#ifndef FONTTEST_H
#define FONTTEST_H

#include "GameState.h" 
#include ".\States\MainMenuState.h"
#include "oglTexture2D.h"
#include "oglGameVars.h"
#include<fstream>
#include<iostream>
#include <string>
#include "oglSpriteFont.h"

class fontTest: public GameState
{
public:
	fontTest()
	{		
	}
	~fontTest();
	fontTest(GameStateManager &Parent, int newID) : GameState(Parent, newID)
	{
		oglSpriteFont* courier;
		courier = new oglSpriteFont();
		courier->open("fonts\\courierSpritefontWhite.png", 12);
		fontList.push_back(courier);			
		courier = new oglSpriteFont();
		courier->open("fonts\\courierSpritefontWhite.png", 18);
		fontList.push_back(courier);
		courier = new oglSpriteFont();
		courier->open("fonts\\courierSpritefontWhite.png", 24);
		fontList.push_back(courier);
		courier = new oglSpriteFont();
		courier->open("fonts\\courierSpritefontWhite.png", 36);
		fontList.push_back(courier);			
		for(int x = 12; x < 100; x+=5)
		{
			courier = new oglSpriteFont();
			courier->open("fonts\\courierSpriteFontWhite.png", x);
			fontList.push_back(courier);			
		}

		fontSpacing = 1;
	}

	int x;
	bool fontTest::Draw()
	{
		vector<oglSpriteFont*>::iterator fitr = fontList.begin();
		for(int x = 0; x < 50; x++)
		{
			fontList[0]->drawText(15, fontSpacing*x*12, "This is a test.");
		}

		for(int x = 0; x < 50; x++)
		{
			fontList[1]->drawText(215, fontSpacing*x*18, "This is a test.");
		}

		for(int x = 0; x < 50; x++)
		{
			fontList[2]->drawText(455, fontSpacing*x*24, "This is a test.");
		}		

		for(int x = 0; x < 50; x++)
		{
			fontList[2]->drawText(755, fontSpacing*x*36, "This is a test.");
		}		
		return true;
	}

	
private:
	vector<oglSpriteFont*> fontList;
	double fontSpacing;
	double fontSpacingMultiplier;
	void keyboardInput(unsigned char c, int x, int y)
	{
		vector<oglSpriteFont*>::iterator fitr = fontList.begin();
		switch(c)
		{
		case '1':
			fontSpacing+= 0.1;
			std::cout << "font spacing " << fontSpacing << endl;
			break;
		case '2':
			fontSpacing-= 0.1;
			std::cout << "font spacing " << fontSpacing << endl;
			break;
		case '3':
			for(;fitr < fontList.end(); fitr++)
			{
				(*fitr)->lessb();
			}
			break;
		case '4':
			for(;fitr < fontList.end(); fitr++)
			{
				(*fitr)->moreb();
			}
			break;
		}
	}

};

#endif