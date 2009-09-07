#ifndef PLAYGAME_H
#define PLAYGAME_H

#include "GameState.h" 
#include "oglUtility.h"
#include "gameBoard.h"
#include "LogicInterface.h"
#include "oglGameVars.h"
#include "Button.h"
#include "States\\PauseGameState.h"
#include "States\\DevLogoState.h"
#include "States\\SaveGameState.h"
#include "States\\MainMenuState.h"

class playGame : public GameState
{
private:
	gameBoard *gamePlay;
	int timer;
	int spintimer;
	int spintimerStart;
	vector<levelData*> levelList;
	vector<oglTexture2D*> endGamePics;
	vector<oglTexture2D*>::iterator endGameAnimation;
	MenuSys* myMenu;
	MenuSys* compass;
	MenuSys* skipMenu;
	int compassOffsetX;
	int compassOffsetY;
	oglTexture2D* img;
	oglTexture2D* blackImage;
	oglTexture2D* youDiedImage;
	GameBoardState curState;
	vector<Button*> buttonList;
	bool gameSaved;
	bool pregameRunning;

	bool finishNow;
	bool finishing;

	// pregame text locations
	int preGameTextOffsetX;
	int preGameTextOffsetY;
	int preGameTextSpacing;
	int preGameTitleDescSpacing;
	int playerScore;
	void drawLevelInfo();
	bool doneDead;

	bool panup();
	bool panleft();
	bool panright();
	bool pandown();
	bool panupleft();
	bool panupright();
	bool pandownleft();
	bool pandownright();
	bool zoomout();
	bool zoomin();
	bool rotatemapleft();
	bool rotatemapright();
	bool center();
	bool speedUp();
	bool slowDown();

	bool launchHelpState();

public:
	playGame()
	{
		initialize();
	}
	playGame(GameStateManager &Parent, int newID) : GameState(Parent, newID)
	{
		initialize();		
	}
	~playGame();

	bool initialize();
	bool playGame::Update();
	bool playGame::Draw();
	bool exitGame();
	bool advance();
	bool replayLevel();
	void processMouse(int x, int y);
	void processMouseClick(int button, int state, int x, int y);
	void keyboardInput(unsigned char c, int x, int y);
	void doEndGameDraw();
	
	LogicInterface mInterface;

};


#endif

