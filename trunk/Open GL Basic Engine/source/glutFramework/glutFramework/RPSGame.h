#include "GameState.h"
#include "oglTexture2D.h"
#include "GameStateManager.h"
#include "RPSEntity.h"
#include "oglGameVars.h"

class RPSGame : public GameState
{

public:
	RPSGame(GameStateManager &Parent, int newID) : GameState(Parent, newID)
	{
		init();
	}
	~RPSGame();

	void RPSGame::processMouse(int x, int y);
	void RPSGame::processMouseClick(int button, int state, int x, int y);
	void RPSGame::keyboardInput(unsigned char c, int x, int y);
private:
	void init();
	bool Draw();
	bool Update();

	RPSHand lastPlayerHand;
	RPSHand lastCPUHand;
	int playerScore;
	int cpuScore;
	int victor;

	RPSEntity* CPUPlayer;
};