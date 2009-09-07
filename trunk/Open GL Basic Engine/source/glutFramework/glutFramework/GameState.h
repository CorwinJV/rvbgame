#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <ctime>
#include "oglTexture2D.h"

class GameStateManager;
class GameState
{
protected:
	// Current State
	State currentStatus;
	int stateID;

	bool displayCollisionDebug;
    int screenHeight;
    int screenWidth;
	
	GameStateManager *GSM;
	clock_t startTime;
	clock_t timer;

public:
	GameState();
	GameState(GameStateManager &Parent, int newID);
	~GameState();
	
	virtual bool setStatus(State newStatus);
	virtual State getStatus();
	virtual int getID();
	virtual bool initialize();
	virtual bool Update();
    virtual bool Draw();
	virtual void processMouse(int x, int y);
	virtual void processMouseClick(int button, int state, int x, int y);
	virtual void keyboardInput(unsigned char c, int x, int y);
	virtual void clearBackground();
};

#endif