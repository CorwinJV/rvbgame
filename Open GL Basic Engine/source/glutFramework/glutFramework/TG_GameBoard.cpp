#include "TG_GameBoard.h"

TG_GameBoard::TG_GameBoard(
		boardPosition zeroZero, boardPosition oneZero,	boardPosition twoZero,	boardPosition threeZero, boardPosition fourZero, boardPosition zeroOne,
		boardPosition oneOne,	boardPosition twoOne,	boardPosition threeOne, boardPosition zeroTwo,
		boardPosition oneTwo,	boardPosition twoTwo,	boardPosition zeroThree,
		boardPosition oneThree, boardPosition zeroFour, TG_GameBoard* parent)
		// Membership Initialization List
		: parentBoard(NULL), selectedBoard(NULL), hintBoard(NULL),
		  playerHasSelectedMove(false), leadsToWinCondition(false)
{
	// Clear out the board
	for(int i = 0; i < 5; i++)
	{
		for(int j = 0; j < 5; j++)
		{
			mBoard[i][j] = BP_NULL;
		}
	}

	// Set our parent board
	parentBoard = parent;

	// Initialize our triangle 
	mBoard[0][0] = zeroZero;
	mBoard[1][0] = oneZero;
	mBoard[2][0] = twoZero;
	mBoard[3][0] = threeZero;
	mBoard[4][0] = fourZero;
	mBoard[0][1] = zeroOne;
	mBoard[1][1] = oneOne;
	mBoard[2][1] = twoOne;
	mBoard[3][1] = threeOne;
	mBoard[0][2] = zeroTwo;
	mBoard[1][2] = oneTwo;
	mBoard[2][2] = twoTwo;
	mBoard[0][3] = zeroThree;
	mBoard[1][3] = oneThree;
	mBoard[0][4] = zeroFour;

	timeToReset = false;

	if(this->howManyPiecesAreLeft() == 1)
	{
		setLeadsToWinCondition();
	}

	//if(this->howManyPiecesAreLeft() == 14)
	//cout << "Game board being created, pieces left: " << this->howManyPiecesAreLeft() << endl;

	if(!this->leadsToWinCondition)
	{
		generateChildren();
	}
}

TG_GameBoard::~TG_GameBoard()
{

}

void TG_GameBoard::Update()
{

}

void TG_GameBoard::Draw()
{
	// Tom, this is your new place. I hope you don't mind paying utilities. 
	// Also, that stain will probably wash out.

	int offsetX = 290;
	int offsetY = 185;

	double halfHeight = (imageSize/2);
	double halfWidth = (imageSize/2)*1.1;
	
	int drawX = 0;
	int drawY = 0;

	for(int y = 0; y < 5; y++)
	{
		for(int x = 0; x <= (4 - y); x++)
		{
			drawX = offsetX + (x * halfWidth - (y * halfWidth) + (halfWidth));
			drawY = offsetY + ((y * imageSize) + (x * imageSize));
			//calculate drawX and Y here soon!
			GameVars->emptyImage->drawImage(imageSize, imageSize, drawX, drawY);

			if(getBoardValueAt(x, y) == BP_POPULATED)
			{
				GameVars->marbleImage->drawImage(imageSize, imageSize, drawX, drawY);
			}
		}
	}
}

void TG_GameBoard::generateChildren()
{
	{
		// Regular Child Creation
		for(int y = 0; y < 5; y++)
		{
			for(int x = 0; x <= (4 - y); x++)
			{
				if(!this->leadsToWinCondition)
				{
					if(mBoard[x][y] == BP_POPULATED)
					{
						// Determine if there are any valid possible jump locations
						// then add children for each jump possibility

						// checking left of destination
						if(x >= 2)
						{
							if((mBoard[x-1][y] == BP_POPULATED) &&
								(mBoard[x-2][y] == BP_EMPTY))
							{
								// Set current position to empty (marble is leaving this space)
								mBoard[x][y] = BP_EMPTY;
								// Set adjacent position to empty (marble has captured this space)
								mBoard[x-1][y] = BP_EMPTY;
								// Set far position to populated (marble is occupying this space)
								mBoard[x-2][y] = BP_POPULATED;

								// Create child
								TG_GameBoard* newChild = new TG_GameBoard(mBoard[0][0], mBoard[1][0], mBoard[2][0],
											mBoard[3][0], mBoard[4][0], mBoard[0][1], mBoard[1][1], mBoard[2][1],
											mBoard[3][1], mBoard[0][2], mBoard[1][2], mBoard[2][2], mBoard[0][3],
											mBoard[1][3], mBoard[0][4],this);
								// Add Child to list
								children.push_back(newChild);

								// Reset board positions
								mBoard[x][y] = BP_POPULATED;
								mBoard[x-1][y] = BP_POPULATED;
								mBoard[x-2][y] = BP_EMPTY;
							}
						}

						// checking right of destination
						if(x <= 2)
						{
							if((mBoard[x+1][y] == BP_POPULATED) &&
								(mBoard[x+2][y] == BP_EMPTY))
							{
								// Set current position to empty (marble is leaving this space)
								mBoard[x][y] = BP_EMPTY;
								// Set adjacent position to empty (marble has captured this space)
								mBoard[x+1][y] = BP_EMPTY;
								// Set far position to populated (marble is occupying this space)
								mBoard[x+2][y] = BP_POPULATED;

								// Create child
								TG_GameBoard* newChild = new TG_GameBoard(mBoard[0][0], mBoard[1][0], mBoard[2][0],
											mBoard[3][0], mBoard[4][0], mBoard[0][1], mBoard[1][1], mBoard[2][1],
											mBoard[3][1], mBoard[0][2], mBoard[1][2], mBoard[2][2], mBoard[0][3],
											mBoard[1][3], mBoard[0][4],this);
								// Add Child to list
								children.push_back(newChild);

								// Reset board positions
								mBoard[x][y] = BP_POPULATED;
								mBoard[x+1][y] = BP_POPULATED;
								mBoard[x+2][y] = BP_EMPTY;
							}
						}

						// checking up from destination
						if(y >= 2)
						{
							if((mBoard[x][y-1] == BP_POPULATED) &&
								(mBoard[x][y-2] == BP_EMPTY))
							{
								// Set current position to empty (marble is leaving this space)
								mBoard[x][y] = BP_EMPTY;
								// Set adjacent position to empty (marble has captured this space)
								mBoard[x][y-1] = BP_EMPTY;
								// Set far position to populated (marble is occupying this space)
								mBoard[x][y-2] = BP_POPULATED;

								// Create child
								TG_GameBoard* newChild = new TG_GameBoard(mBoard[0][0], mBoard[1][0], mBoard[2][0],
											mBoard[3][0], mBoard[4][0], mBoard[0][1], mBoard[1][1], mBoard[2][1],
											mBoard[3][1], mBoard[0][2], mBoard[1][2], mBoard[2][2], mBoard[0][3],
											mBoard[1][3], mBoard[0][4],this);
								// Add Child to list
								children.push_back(newChild);

								// Reset board positions
								mBoard[x][y] = BP_POPULATED;
								mBoard[x][y-1] = BP_POPULATED;
								mBoard[x][y-2] = BP_EMPTY;
							}
						}

						// checking down of destination
						if(y <= 2)
						{
							if((mBoard[x][y+1] == BP_POPULATED) &&
								(mBoard[x][y+2] == BP_EMPTY))
							{
								// Set current position to empty (marble is leaving this space)
								mBoard[x][y] = BP_EMPTY;
								// Set adjacent position to empty (marble has captured this space)
								mBoard[x][y+1] = BP_EMPTY;
								// Set far position to populated (marble is occupying this space)
								mBoard[x][y+2] = BP_POPULATED;

								// Create child
								TG_GameBoard* newChild = new TG_GameBoard(mBoard[0][0], mBoard[1][0], mBoard[2][0],
											mBoard[3][0], mBoard[4][0], mBoard[0][1], mBoard[1][1], mBoard[2][1],
											mBoard[3][1], mBoard[0][2], mBoard[1][2], mBoard[2][2], mBoard[0][3],
											mBoard[1][3], mBoard[0][4],this);
								// Add Child to list
								children.push_back(newChild);

								// Reset board positions
								mBoard[x][y] = BP_POPULATED;
								mBoard[x][y+1] = BP_POPULATED;
								mBoard[x][y+2] = BP_EMPTY;
							}
						}

						// check downleft from destination
						// x - 2
						// y + 2
						if((x >=2) && (y <=2))
						{
							if((mBoard[x-1][y+1] == BP_POPULATED) &&
								(mBoard[x-2][y+2] == BP_EMPTY))
							{
								// Set current position to empty (marble is leaving this space)
								mBoard[x][y] = BP_EMPTY;
								// Set adjacent position to empty (marble has captured this space)
								mBoard[x-1][y+1] = BP_EMPTY;
								// Set far position to populated (marble is occupying this space)
								mBoard[x-2][y+2] = BP_POPULATED;

								// Create child
								TG_GameBoard* newChild = new TG_GameBoard(mBoard[0][0], mBoard[1][0], mBoard[2][0],
											mBoard[3][0], mBoard[4][0], mBoard[0][1], mBoard[1][1], mBoard[2][1],
											mBoard[3][1], mBoard[0][2], mBoard[1][2], mBoard[2][2], mBoard[0][3],
											mBoard[1][3], mBoard[0][4],this);
								// Add Child to list
								children.push_back(newChild);

								// Reset board positions
								mBoard[x][y] = BP_POPULATED;
								mBoard[x-1][y+1] = BP_POPULATED;
								mBoard[x-2][y+2] = BP_EMPTY;
							}
						}

						// check upright from destination
						// x + 2
						// y - 2
						if((x <= 2) && (y >= 2))
						{
							if((mBoard[x+1][y-1] == BP_POPULATED) &&
								(mBoard[x+2][y-2] == BP_EMPTY))
							{
								// Set current position to empty (marble is leaving this space)
								mBoard[x][y] = BP_EMPTY;
								// Set adjacent position to empty (marble has captured this space)
								mBoard[x+1][y-1] = BP_EMPTY;
								// Set far position to populated (marble is occupying this space)
								mBoard[x+2][y-2] = BP_POPULATED;

								// Create child
								TG_GameBoard* newChild = new TG_GameBoard(mBoard[0][0], mBoard[1][0], mBoard[2][0],
											mBoard[3][0], mBoard[4][0], mBoard[0][1], mBoard[1][1], mBoard[2][1],
											mBoard[3][1], mBoard[0][2], mBoard[1][2], mBoard[2][2], mBoard[0][3],
											mBoard[1][3], mBoard[0][4],this);
								// Add Child to list
								children.push_back(newChild);

								// Reset board positions
								mBoard[x][y] = BP_POPULATED;
								mBoard[x+1][y-1] = BP_POPULATED;
								mBoard[x+2][y-2] = BP_EMPTY;
							}
						}
					}
				}
			//////////////////////////////
				else
				{
					//cout << "We skipped a generation of a node because this pathway already leads to a victory." << endl;
				}
			}
		}
	}
}

bool TG_GameBoard::getLeadsToWinCondition()
{
	return leadsToWinCondition;
}

void TG_GameBoard::setLeadsToWinCondition()
{
	leadsToWinCondition = true;
	if(parentBoard != NULL)
	{
		parentBoard->setLeadsToWinCondition();
	}
}

void TG_GameBoard::reset()
{
	timeToReset = true;
}

void TG_GameBoard::resetReset()
{
	timeToReset = false;
}

void TG_GameBoard::hint(int *destX, int *destY, int *sourceX, int *sourceY)
{
	// first just for safety's sake, lets make sure that there are children, there should be but lets just make sure
	if(children.size() > 0)
	{
		std::vector<TG_GameBoard*>::iterator iChildren;
		iChildren = children.begin();

		bool foundSomething = false;

		for(;iChildren < children.end(); iChildren++)
		{
			if((*iChildren)->getLeadsToWinCondition())
			{
				foundSomething = true;
				hintBoard = (*iChildren);
			}
		}

		if(foundSomething)
		{
			int hintDestX = -1;
			int hintDestY = -1;
			int hintSourceX = -1;
			int hintSourceY = -1;
			// show the appropriate hint...
			// compare the differences between the two current board and the hint board
			// look for a single spot that has a new marble on the destination board
			// that is not on the current board, this is the suggested move position
			for(int y = 0; y < 5; y++)
			{
				for(int x = 0; x < 5 - y; x++)
				{
					// if hintboard is populated and this one is not populated...
					if ( (hintBoard->getBoardValueAt(x, y) == BP_POPULATED) && 
							  (this->getBoardValueAt(x, y) != BP_POPULATED) )
					{
						hintDestX = x;
						hintDestY = y;
					}
				}
			}

			// now that we have our destination square, we just need to check the 
			// four directions to see if any of those match up as well
			// we should have two occurances occuring in a row
			// #1 (on this board 1 square away from destx/y should be a piece &&
			//     on destination board that square 1 away from destx/y should have no piece)
			// #2 (on this board 2 squares away from destx/y should be a piece &&
			//     on destination board that square 2 away from destx/y should have no piece)
			// such that you have this situation...
			// this board = * * o
			// dest board = o o *
			
			// so lets just go through the directions
			
			// checking left of destination
			if(hintDestX >=2)
			{
				if(      (this->getBoardValueAt(hintDestX-1, hintDestY) == BP_POPULATED) && 
					     (this->getBoardValueAt(hintDestX-2, hintDestY) == BP_POPULATED) &&
					(hintBoard->getBoardValueAt(hintDestX-1, hintDestY) == BP_EMPTY) &&
					(hintBoard->getBoardValueAt(hintDestX-2, hintDestY) == BP_EMPTY) )
				{
					// we have ourselves a winner!
					hintSourceX = hintDestX-2;
					hintSourceY = hintDestY;
				}
			}

			// checking right of destination
			if(hintDestX <=2)
			{
				if(      (this->getBoardValueAt(hintDestX+1, hintDestY) == BP_POPULATED) && 
					     (this->getBoardValueAt(hintDestX+2, hintDestY) == BP_POPULATED) &&
					(hintBoard->getBoardValueAt(hintDestX+1, hintDestY) == BP_EMPTY) &&
					(hintBoard->getBoardValueAt(hintDestX+2, hintDestY) == BP_EMPTY) )
				{
					// we have ourselves a winner!
					hintSourceX = hintDestX+2;
					hintSourceY = hintDestY;
				}
			}

			// checking up from destination
			if(hintDestY >=2)
			{
				if(      (this->getBoardValueAt(hintDestX, hintDestY-1) == BP_POPULATED) && 
					     (this->getBoardValueAt(hintDestX, hintDestY-2) == BP_POPULATED) &&
					(hintBoard->getBoardValueAt(hintDestX, hintDestY-1) == BP_EMPTY) &&
					(hintBoard->getBoardValueAt(hintDestX, hintDestY-2) == BP_EMPTY) )
				{
					// we have ourselves a winner!
					hintSourceX = hintDestX;
					hintSourceY = hintDestY-2;
				}
			}


			// checking down from destination
			if(hintDestY <=2)
			{
				if(      (this->getBoardValueAt(hintDestX, hintDestY+1) == BP_POPULATED) && 
					     (this->getBoardValueAt(hintDestX, hintDestY+2) == BP_POPULATED) &&
					(hintBoard->getBoardValueAt(hintDestX, hintDestY+1) == BP_EMPTY) &&
					(hintBoard->getBoardValueAt(hintDestX, hintDestY+2) == BP_EMPTY) )
				{
					// we have ourselves a winner!
					hintSourceX = hintDestX;
					hintSourceY = hintDestY+2;
				}
			}

			// check downleft from destination
			// x - 2
			// y + 2
			if((hintDestX >=2) && (hintDestY <=2))
			{
				if(      (this->getBoardValueAt(hintDestX-1, hintDestY+1) == BP_POPULATED) && 
					     (this->getBoardValueAt(hintDestX-2, hintDestY+2) == BP_POPULATED) &&
					(hintBoard->getBoardValueAt(hintDestX-1, hintDestY+1) == BP_EMPTY) &&
					(hintBoard->getBoardValueAt(hintDestX-2, hintDestY+2) == BP_EMPTY) )
				{
					// we have ourselves a winner!
					hintSourceX = hintDestX-2;
					hintSourceY = hintDestY+2;
				}
			}

			// check upright from destination
			// x + 2
			// y - 2
			if((hintDestX <=2) && (hintDestY >=2))
			{
				if(      (this->getBoardValueAt(hintDestX+1, hintDestY-1) == BP_POPULATED) && 
					     (this->getBoardValueAt(hintDestX+2, hintDestY-2) == BP_POPULATED) &&
					(hintBoard->getBoardValueAt(hintDestX+1, hintDestY-1) == BP_EMPTY) &&
					(hintBoard->getBoardValueAt(hintDestX+2, hintDestY-2) == BP_EMPTY) )
				{
					// we have ourselves a winner!
					hintSourceX = hintDestX+2;
					hintSourceY = hintDestY-2;
				}
			}

			// do something with hintSourceX, hintSourceY, hintDestX, hintDestY
			(*destX) = hintDestX;
			(*destY) = hintDestY;
			(*sourceX) = hintSourceX;
			(*sourceY) = hintSourceY;
		}
		else
		{
			// this should be the failure state
			(*destX) = -1;
			(*destY) = -1;
			(*sourceX) = -1;
			(*sourceY) = -1;
		}
	}
	else
	{
		// this should be the failure state
		(*destX) = -1;
		(*destY) = -1;
		(*sourceX) = -1;
		(*sourceY) = -1;
	}
}

boardPosition TG_GameBoard::getBoardValueAt(int x, int y)
{
	if((x >= 0) && (x <= 4) && (y >= 0) && (y <= 4))
	{
		return mBoard[x][y];
	}
	else 
	{
		cout << "Smooth move exlax, you tried to call position " << x << " " << y << endl;
		return BP_NULL;
	}
		
}


void TG_GameBoard::undo()
{
	if(parentBoard != NULL)
	{
		selectedBoard = parentBoard;
		playerHasSelectedMove = true;
	}
}

bool TG_GameBoard::checkIsWinCondition()
{
	int numPiecesLeft = 0;
	for(int x = 0; x <=4; x++)
	{
		for(int y = 0; y <= 4; y++)
		{
			if(this->getBoardValueAt(x, y) == BP_POPULATED)
			{
				numPiecesLeft++;
			}
		}
	}
	if(numPiecesLeft == 1)
	{
		return true;
	}

	return false;
}

bool TG_GameBoard::getHasPlayerSelectedMove()
{
	return playerHasSelectedMove;
}

TG_GameBoard* TG_GameBoard::getSelectedBoard()
{
	if(selectedBoard != NULL)
	{
		return selectedBoard;
	}
	else
	{
		return NULL;
	}
}

TG_GameBoard* TG_GameBoard::getPreviousBoard()	// gets the parent board if it exists..
{
	return parentBoard;
}

TG_GameBoard* TG_GameBoard::getNextBoard(int sX, int sY, int dX, int dY)
{
	// we need to find a gameboard where sx/y is empty
	// and where dx/dy is full
	// we return when we find the matching situation
	// because there can be only one

	if(children.size() > 0)
	{
		std::vector<TG_GameBoard*>::iterator iChildren;
		iChildren = children.begin();

		for(;iChildren < children.end(); iChildren++)
		{
			if( ((*iChildren)->getBoardValueAt(sX, sY) == BP_EMPTY) && 
				((*iChildren)->getBoardValueAt(dX, dY) == BP_POPULATED) )
			{
				return (*iChildren);
			}
		}
	}
	return NULL;
}

bool TG_GameBoard::checkIsLoseCondition()	// returns true if there is no possible valid moves left
{
	if((children.size() == 0) && !leadsToWinCondition)
	{
		return true;
	}
	return false;
}

int  TG_GameBoard::howManyPiecesAreLeft()	// returns how many pieces are left on the board
{
	int numPieces = 0;
	for(int x = 0; x < 5; x++)
	{
		for(int y = 0; y < 5; y++)
		{
			if(this->getBoardValueAt(x, y) == BP_POPULATED)
			{
				numPieces++;
			}
		}
	}

	if(numPieces > 15)
	{
		cout << "WTF? we have " << numPieces << " on the board?" << endl;
		// not sure how this happened, lets return 15
	}

	return numPieces;
}

void TG_GameBoard::adoptChild(TG_GameBoard *adoptedChild)
{
	children.push_back(adoptedChild);	
}