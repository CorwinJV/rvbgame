#include "bioWrapper.h"

#define buttonHeight 64				
#define	buttonWidth 128
#define buttonX 700
#define buttonY 25

// init
void bioWrapper::init()
{
	// create a new gameboard, populate it all full and stuff
	startingBoard = new TG_GameBoard(BP_POPULATED, 
									   BP_POPULATED, BP_POPULATED, 
									   BP_POPULATED, BP_POPULATED, BP_POPULATED, 
									   BP_POPULATED, BP_POPULATED, BP_POPULATED, BP_POPULATED, 
									   BP_POPULATED, BP_POPULATED, BP_POPULATED, BP_POPULATED, BP_POPULATED, 
									   NULL);
	
	// Testing, should have two children
	//startingBoard = new TG_GameBoard(BP_EMPTY, BP_POPULATED, BP_EMPTY, BP_POPULATED, BP_POPULATED,
	//								 BP_EMPTY, BP_POPULATED, BP_EMPTY, BP_POPULATED,
	//								 BP_EMPTY, BP_EMPTY, BP_EMPTY,
	//								 BP_EMPTY, BP_EMPTY,
	//								 BP_EMPTY, 
	//								 NULL);

	
	currentBoard = startingBoard;
	nextBoard = NULL;
	childLabor = NULL;

	clearUI();

	// that is all, end of line
}

bool bioWrapper::Draw()
{
	// make the background white
	glClearColor(255, 255, 255, 0);
	int drawX = 0;
	int drawY = 0;

	int offsetX = 290;
	int offsetY = 185;
	double halfWidth = (imageSize/2)*1.1;
	// first things first, lets draw stuff
	// draw will do the following
	// draw the pyrmaid
	GameVars->boardImage->drawImage(768, 768, 10, 10);
	//draw the buttons
	GameVars->undoButton->drawImage(buttonWidth, buttonHeight, buttonX, buttonY);
	GameVars->hintButton->drawImage(buttonWidth, buttonHeight, buttonX, buttonY + buttonHeight);
	GameVars->resetButton->drawImage(buttonWidth, buttonHeight, buttonX, buttonY + (buttonHeight*2));
	// tell the current board to draw itself
	currentBoard->Draw();

	// then we need to draw any UI elements (selected, possibliites, hints, buttons, dancing chipmnunks, etc)
	for(int x = 0; x < 5; x++)
	{
		for(int y = 0; y < 5; y++)
		{
			drawX = offsetX + (x * halfWidth - (y * halfWidth) + (halfWidth));
			drawY = offsetY + ((y * imageSize) + (x * imageSize));
			if(MUI_Board[x][y] == MUI_SELECTED)
			{
				GameVars->selectedImage->drawImage(imageSize, imageSize, drawX, drawY);
			}
			else if(MUI_Board[x][y] == MUI_POSSIBILITY)
			{
				GameVars->possibilityImage->drawImage(imageSize, imageSize, drawX, drawY);
			}
			else if(currentBoard->getBoardValueAt(x, y) == BP_POPULATED)
			{
				GameVars->marbleImage->drawImage(imageSize, imageSize, drawX, drawY);
			}
		}
	}
	
	return true;
}

void bioWrapper::processMouseClick(int button, int state, int x, int y)
{
	
	// we need to process mouse clicking etc
	// if mouse is clicked, 
	if((button == GLUT_LEFT) && (state == GLUT_DOWN))
	{	
		int offsetX = 290;
		int offsetY = 185;

		double halfHeight = (imageSize/2);
		double halfWidth = (imageSize/2)*1.1;
		
		int drawX = 0;
		int drawY = 0;

		// check to see if we've clicked any of the buttons
		if((x > buttonX)&&(x < (buttonX + buttonWidth))
			&& (y > buttonY) && (y < (buttonY + buttonHeight)))
		{
			undoClicked();
			//cout << "Undo button was clicked " << endl;
		}
		else if((x > buttonX)&&(x < (buttonX + buttonWidth))
			&& (y > buttonY + buttonHeight) && (y < (buttonY + buttonHeight*2)))
		{
			hintClicked();
			//cout << "Hint button was clicked " << endl;
		}
		else if((x > buttonX)&&(x < (buttonX + buttonWidth))
			&& (y > buttonY + (buttonHeight*2)) && (y < (buttonY + buttonHeight*3)))
		{
			resetClicked();
			//cout << "Reset button was clicked " << endl;
		}


		// check to see if any of the positions on the board have been clicked
		for(int my = 0; my < 5; my++)
		{
			for(int mx = 0; mx <= (4 - my); mx++)
			{
				drawX = offsetX + (mx * halfWidth - (my * halfWidth) + (halfWidth));
				drawY = offsetY + ((my * imageSize) + (mx * imageSize));
			
				if((x > drawX) && (x < (drawX + imageSize)) 
					&& (y > drawY) && y < (drawY + imageSize))
				{
					// Debug 
					//cout << "mx: " << mx << "my: " << my << endl;
					
					
					// if no piece is currently selected, check if they clicked on a piece of some sort
					if(currentBoard->getBoardValueAt(mx, my) == BP_POPULATED)
					{
						clearUI();
						// if a piece was clicked, select the piece
						MUI_Board[mx][my] = MUI_SELECTED;
						
						//and determine if there are any valid possible jump locations
						// checking left of destination
						if(mx >= 2)
						{
							if((currentBoard->getBoardValueAt(mx - 1, my) == BP_POPULATED) &&
								(currentBoard->getBoardValueAt(mx - 2, my) == BP_EMPTY))
							{
								MUI_Board[mx-2][my] = MUI_POSSIBILITY;
							}
						}

						// checking right of destination
						if(mx <= 2)
						{
							if((currentBoard->getBoardValueAt(mx + 1, my) == BP_POPULATED) &&
								(currentBoard->getBoardValueAt(mx + 2, my) == BP_EMPTY))
							{
								MUI_Board[mx+2][my] = MUI_POSSIBILITY;
							}
						}

						// checking up from destination
						if(my >= 2)
						{
							if((currentBoard->getBoardValueAt(mx, my - 1) == BP_POPULATED) &&
								(currentBoard->getBoardValueAt(mx , my - 2) == BP_EMPTY))
							{
								MUI_Board[mx][my-2] = MUI_POSSIBILITY;
							}
						}

						// checking down of destination
						if(my <= 2)
						{
							if((currentBoard->getBoardValueAt(mx , my + 1) == BP_POPULATED) &&
								(currentBoard->getBoardValueAt(mx, my + 2) == BP_EMPTY))
							{
								MUI_Board[mx][my+2] = MUI_POSSIBILITY;
							}
						}

						// check downleft from destination
						// x - 2
						// y + 2
						if((mx >=2) && (my <=2))
						{
							if((currentBoard->getBoardValueAt(mx - 1, my + 1) == BP_POPULATED) &&
								(currentBoard->getBoardValueAt(mx - 2, my + 2) == BP_EMPTY))
							{
								MUI_Board[mx-2][my+2] = MUI_POSSIBILITY;
							}
						}

						// check upright from destination
						// x + 2
						// y - 2
						if((mx <= 2) && (my >= 2))
						{
							if((currentBoard->getBoardValueAt(mx + 1, my - 1) == BP_POPULATED) &&
								(currentBoard->getBoardValueAt(mx + 2, my - 2) == BP_EMPTY))
							{
								MUI_Board[mx+2][my-2] = MUI_POSSIBILITY;
							}
						}
					}		

					//// Additionally, the first time the board is initialized, there will be no open spaces, so any piece can be 
					//// removed and the child that has that piece missing will be loaded in.
					//if(currentBoard == startingBoard)
					//{
					//	if((mx == 0)&&(my == 0))
					//	{
					//		nextBoard = currentBoard->getNextBoard(mx, my, 1, 0);
					//	}
					//	else
					//	{
					//		nextBoard = currentBoard->getNextBoard(mx, my, 0, 0);
					//	}
					//}

					// If this is the first board we're going to create 
					// the next board based on the selected piece
					if(currentBoard == startingBoard)
					{
						boardPosition bpArray[5][5];
						for(int bpX = 0; bpX < 5; bpX++)
						{
							for(int bpY = 0; bpY < 5; bpY++)
							{
								bpArray[bpX][bpY] = BP_POPULATED;
							}
						}

						bpArray[mx][my] = BP_EMPTY;

						// Create child
						nextBoard = new TG_GameBoard(bpArray[0][0], bpArray[1][0], bpArray[2][0],
									bpArray[3][0], bpArray[4][0], bpArray[0][1], bpArray[1][1], bpArray[2][1],
									bpArray[3][1], bpArray[0][2], bpArray[1][2], bpArray[2][2], bpArray[0][3],
									bpArray[1][3], bpArray[0][4], currentBoard);
						
						childLabor = nextBoard;
					}

					// check if they clicked on a valid possible jump location
					else if(MUI_Board[mx][my] == MUI_POSSIBILITY)
					{
						int selectedX = 0;
						int selectedY = 0;
						// determine the piece that was selected
						for(int cx = 0; cx < 5; cx++)
						{
							for(int cy = 0; cy < 5; cy++)
							{
								if(MUI_Board[cx][cy] == MUI_SELECTED)
								{
									selectedX = cx;
									selectedY = cy;
								}
							}
						}

						// if they did, ask the current gameboard for the next board and pass in the source and destination information
						nextBoard = currentBoard->getNextBoard(selectedX, selectedY, mx, my);

						// if nextBoard does not exist, we need to create it
						if(nextBoard == NULL)
						{
							// so first lets create a dummy board that is a straight up copy and paste of this board
							boardPosition bpArray[5][5];
							for(int bpX = 0; bpX < 5; bpX++)
							{
								for(int bpY = 0; bpY < 5; bpY++)
								{
									bpArray[bpX][bpY] = this->currentBoard->getBoardValueAt(bpX, bpY);
								}
							}
								
							// ok so let's set the board accordingly
							// first where we were = empty
							bpArray[selectedX][selectedY] = BP_EMPTY;

							// and now where we're going = populated
							bpArray[mx][my] = BP_POPULATED;					
							

							// now the piece in between these = empty
							bpArray[((selectedX + mx)/2)][((selectedY + my)/2)] = BP_EMPTY;

							// Create child
							nextBoard = new TG_GameBoard(bpArray[0][0], bpArray[1][0], bpArray[2][0],
										bpArray[3][0], bpArray[4][0], bpArray[0][1], bpArray[1][1], bpArray[2][1],
										bpArray[3][1], bpArray[0][2], bpArray[1][2], bpArray[2][2], bpArray[0][3],
										bpArray[1][3], bpArray[0][4], currentBoard);
							
							currentBoard->adoptChild(nextBoard);
						}
					}
				
					// "The reason this is there is that it is the trailing end of the..
					// anything below that does not have access to mx and my"
					//				- Dave (as transcribed by Corwin)
					/////////////////////////////////////////////////////////
				}
			}
		}
		 
		// load in the next board
		if(nextBoard != NULL)
		{
			currentBoard = nextBoard;
			nextBoard = NULL;
			clearUI();
		}
		
	
	// if no piece is selected and no valid location was selected, tell them they're a moron and not to click there again

	// if undo button exists and was clicked, 
		// ask the current board for its parent
		// load in the parent

	// if the hint button exists and was clicked
		// ask the gameboard for a hint (and pass it in source and dest x/y values to hold when its done)
		// update tom's new array thing accordingly

	// if the reset button exists and was clicked
		// set currentBoard to startingBoard
	}
}

void bioWrapper::processMouse(int x, int y)
{
}

void bioWrapper::keyboardInput(unsigned char c, int x, int y)
{
}

bool bioWrapper::Update()
{
	// if there is a board to load and the board to load is not NULL
		// set current board to the board to load
		// clear the UI selected/etc elements as we are in a new state now
	
	// check if the current board is a winning board
		// if it is, inform the player they rule
	
	// check if the current board is a losing condition
		// if it is, find out how many pieces are left
		// let the player know how badly they suck
	return true;
}

void bioWrapper::clearUI()
{
	for(int x = 0; x < 5; x++)
	{
		for(int y = 0; y < 5; y++)
		{
			MUI_Board[x][y] = MUI_NULL;
		}
	}
}

void bioWrapper::undoClicked()
{
	nextBoard = currentBoard->getPreviousBoard();
	if(nextBoard == NULL)
	{
		nextBoard = startingBoard;
	}
}

void bioWrapper::resetClicked()
{
	currentBoard = startingBoard;
}

void bioWrapper::hintClicked()
{
	clearUI();
	
	//currentBoard->getLeadsToWinCondition();
	int targetX, targetY, sourceX, sourceY;

	currentBoard->hint(&sourceX, &sourceY, &targetX, &targetY);

	if(sourceX != -1)
	{
		MUI_Board[targetX][targetY] = MUI_SELECTED;
		MUI_Board[sourceX][sourceY] = MUI_POSSIBILITY;
	}

}