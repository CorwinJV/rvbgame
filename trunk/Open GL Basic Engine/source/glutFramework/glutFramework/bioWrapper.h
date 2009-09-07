#include "GameState.h"
#include "oglTexture2D.h"
#include "GameStateManager.h"
#include "TG_GameBoard.h"
#include "oglGameVars.h"

class bioWrapper : public GameState
{

public:
	bioWrapper(GameStateManager &Parent, int newID) : GameState(Parent, newID)
	{
		init();
	}
	~bioWrapper();

	void bioWrapper::processMouse(int x, int y);
	void bioWrapper::processMouseClick(int button, int state, int x, int y);
	void bioWrapper::keyboardInput(unsigned char c, int x, int y);
private:
	void init();
	bool Draw();
	bool Update();
	void clearUI();
	void undoClicked();
	void resetClicked();
	void hintClicked();

	TG_GameBoard* currentBoard;		// the current board we're using
	TG_GameBoard* startingBoard;	// the beginning of the tree
	TG_GameBoard* nextBoard;		// to check for NULL (preventative detarding)
	TG_GameBoard* childLabor;

	int buttonX;					// starting X location for the buttons
	int buttonY;					// starting Y location for the buttons
	int buttonHeight;					// height of the button images
	int buttonWidth ;					// width of the button images

	marbleUI MUI_Board[5][5];

};