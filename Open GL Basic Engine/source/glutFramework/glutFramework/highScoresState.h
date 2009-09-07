#ifndef HIGHSCORESSTATE_H
#define HIGHSCORESSTATE_H

#include "GameState.h" 
#include ".\States\MainMenuState.h"
#include "oglTexture2D.h"
#include "oglGameVars.h"
#include <string>

using namespace std;

struct playerScoring
{
public:
	std::string name;
	int score;
};

struct levelScoring
{
public:
	vector<playerScoring*> levelScores;
};

class highScoresState : public GameState
{
public:
	highScoresState();
	highScoresState(GameStateManager &Parent, int newID) : GameState(Parent, newID)
	{
		myMenu = new MenuSys((int)1024/2 - (int)600/2, (int)768/2 - (int)475/2 + 25, "blankmenu.png", None);
		myMenu->addButton("buttons\\yes.png", "buttons\\yeshover.png", "buttons\\yeshover.png", CreateFunctionPointer0R(this, &highScoresState::nextCallback));
		myMenu->setLastButtonDimensions(100, 50);
		myMenu->setLastButtonPosition((int)1024/2 - 150, 530);

		myMenu->addButton("buttons\\no.png", "buttons\\nohover.png", "buttons\\nohover.png", CreateFunctionPointer0R(this, &highScoresState::previousCallback));
		myMenu->setLastButtonDimensions(100, 50);

		myMenu->addButton("buttons\\no.png", "buttons\\nohover.png", "buttons\\nohover.png", CreateFunctionPointer0R(this, &highScoresState::exitCallback));
		myMenu->setLastButtonDimensions(100, 50);

		myMenu->setLastButtonPosition((int)1024/2 + 50, 530);
		Update();
		buildHighScoresList();
		numScores = 10;
	}

	void processMouse(int x, int y);
	void processMouseClick(int button, int state, int x, int y);
	bool Update();
	bool Draw();
	bool nextCallback();
	bool previousCallback();
	bool exitCallback();


private:
	std::string tempString;
	void buildHighScoresList();
	vector <levelScoring*> allLevelScores;
	int numScores;
	
		
	// add private stuff here

};

#endif
