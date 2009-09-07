#ifndef TG_GAMEBOARD_H
#define TG_GAMEBOARD_H
#include <vector>
#include "oglTexture2D.h"
#include "oglGameVars.h"

enum boardPosition
{
	BP_NULL = -1,
	BP_POPULATED,
	BP_EMPTY
};

class TG_GameBoard
{
public: // class management stuff, constructors, etc.
	TG_GameBoard(
		boardPosition zeroZero, boardPosition oneZero,	boardPosition twoZero,	boardPosition threeZero, boardPosition fourZero, boardPosition zeroOne,
		boardPosition oneOne,	boardPosition twoOne,	boardPosition threeOne, boardPosition zeroTwo,
		boardPosition oneTwo,	boardPosition twoTwo,	boardPosition zeroThree,
		boardPosition oneThree, boardPosition zeroFour, TG_GameBoard* parent);
	~TG_GameBoard();
	void Update();
	void Draw(); // tom's stuff thingie

private: // Private Member Data
	TG_GameBoard* parentBoard;
	TG_GameBoard* selectedBoard;
	TG_GameBoard* hintBoard;

	std::vector<TG_GameBoard*> children;
	boardPosition mBoard[5][5];

	// When a player has selected a move
	// the bool is set to true and the 
	// selectedBoard is set to the selected
	// move.
	bool playerHasSelectedMove;

	bool leadsToWinCondition;

	bool timeToReset;

public: // Public Class Interfaces
	bool checkIsWinCondition();		// returns true if there is only one piece left
	bool checkIsLoseCondition();	// returns true if there is no possible valid moves left
	int  howManyPiecesAreLeft();	// returns how many pieces are left on the board

	bool getLeadsToWinCondition();	// returns the value stored in leadsToWinCondition
	void setLeadsToWinCondition();	// sets leadsToWinCondition	to true and calls this function on its parent

	void reset();					// sets timeToReset to true, wrapper will pickup on this...
	void resetReset();				// sets timeToReset to false

	void hint(int *destX, int *destY, 
			int *sourceX, int *sourceY);	// determines a source and destination to display for a hint

	void undo();					// sets playerHasSelectedMove to true
									// sets selectedBoard to parent if parent != null

	bool getHasPlayerSelectedMove();// returns playerHasSelectedMove

	boardPosition getBoardValueAt(int x, int y);	// returns the board value at a location
	TG_GameBoard* getSelectedBoard(); // Remember to check for null before
									  // using the return value of this function

	TG_GameBoard* getPreviousBoard();	// gets the parent board if it exists..
	TG_GameBoard* getNextBoard(int sX, int sY, int dX, int dY); // you pass in a sourcex/y and destx/y
																// and it returns the appropriate board

	void processMouse(int x, int y);
	void processMouseClick(int button, int state, int x, int y);
	void adoptChild(TG_GameBoard *adoptedChild);

	
	
	
	
	void generateChildren();		// corwins thingie
	// Tuesday...   no progress
	// Wednesday... no progress
	// Thursday.... no progress
	
	

private: // Private Utility Functions
	bool checkForLegalMove(int x, int y);
};

#endif //TG_GAMEBOARD_H