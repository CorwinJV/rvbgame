#ifndef SG400KGA1_H
#define SG400KGA1_H

#include "GameState.h"
#include "oglTexture2D.h"
#include "GameStateManager.h"
#include "oglGameVars.h"
#include <vector>

//class token
//{
//private :
//	std::string tokenName;
//	vector<token*> subTokenList;
//	int numModifiers;	
//
//
//public:
//	void addToken(std::string newToken, numModifiers)
//	{
//		token* tempToken;
//		tempToken = new token(newToken, numModifiers);
//		subTokenList.push_back(tempToken);
//	};
//
//};

class SG400KGA1 : public GameState
{
public:
	SG400KGA1() {};
	SG400KGA1(GameStateManager &Parent, int newID) : GameState(Parent, newID)
	{
		imgSG400 = new oglTexture2D();
		imgSG400->loadImage("buttons\\bacardi.png", 256, 256);
		imgSG400->dX = 256;
		imgSG400->dY = 256;
		imgSG400->mX = 300;
		imgSG400->mY = 50;
		alpha = 1;
	}

	bool SG400KGA1::Update();
	bool SG400KGA1::Draw();
	void SG400KGA1::keyboardInput(unsigned char c, int x, int y);
	void processMouse(int x, int y);
	void processMouseClick(int button, int state, int x, int y);

private:
	vector<string*> commandList;
	oglTexture2D* imgSG400;
	std::string* tString;
	std::string tempString;
	int finished;
	void interpret(std::string*);
	double alpha;
	// add private stuff here

};

#endif