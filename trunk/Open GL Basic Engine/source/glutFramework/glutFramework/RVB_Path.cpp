#include "RVB_Path.h"
#include "RVB_Map.h"

rvbPath::rvbPath(int startX_n, int startY_n, int endX_n, int endY_n, int width, int height, RVB_Map *newBoard)
{
	startX = startX_n;
	startY = startY_n;
	endX = endX_n;
	endY = endY_n;

	boardWidth = width;
	boardHeight = height;

	board = newBoard;

	calculatedPath.resize(0);

	// make an empty node list array
	nodeList.resize(boardWidth);

	for(int x = 0; x < boardWidth; x++)
	{
		nodeList[x].resize(boardHeight);
	}

	// populate the array with empty nodes
	for(int x = 0; x < boardWidth; x++)
	{
		for(int y = 0; y < boardHeight; y++)
		{
			nodeList[x][y] = new aStarNode(x, y, endX, endY, board->isTileValidMove(x, y));
		}
	}

	// puss the start node onto the nodelist hurrah
	nodeList[startX][startY]->setParent(NULL, 0);
	openList.push_back(nodeList[startX][startY]);
	processPath();

	iCalcPath = calculatedPath.begin();
}

rvbPath::~rvbPath()
{
	for(unsigned int x = 0; x < nodeList.size(); x++)
	{
		for(unsigned int y = 0; y < nodeList[x].size(); y++)
		{
			delete nodeList[x][y];
			nodeList[x][y] = NULL;
		}
		nodeList[x].clear();
	}
	nodeList.clear();
}
	
void rvbPath::processPath()
{
	// this will process an entire path
	bool doneChecking = false;

	aStarNode* bestNodeSoFar;
	
	while(!doneChecking)
	{
		// first check to make sure there is a node in the open list
		if(openList.size() == 0)
		{
			doneChecking = true;
		}
		else
		{
			// find the lowest heuristic in the open list
			bestNodeSoFar = openList[0];
			for(int x = 0; x < (int)openList.size(); x++)
			{
				if(openList[x]->getHueristicValue() < bestNodeSoFar->getHueristicValue())
				{
					bestNodeSoFar = openList[x];
				}
			}

			// this may or may not work?
			vector<aStarNode*>::iterator iNode = openList.begin();

			bool foundWhatIWasLookingFor = false;

			for(; !foundWhatIWasLookingFor; iNode++)
			{
				if((*iNode) == bestNodeSoFar)
				{
					foundWhatIWasLookingFor = true;
				}
			}
			iNode--;

			openList.erase(iNode);

			// add any available tiles adjacent to it that are available to the open list
			bool end = false;

			// up
			if(!end) 	end = nodeCheck(0, -1, STRAIGHT_COST, bestNodeSoFar);
			if(end)		{ generatePath();	return;	}
			
			// right
			if(!end) 	end = nodeCheck(1, 0, STRAIGHT_COST, bestNodeSoFar);
			if(end)		{ generatePath();	return;	}

			// down
			if(!end) 	end = nodeCheck(0, 1, STRAIGHT_COST, bestNodeSoFar);
			if(end)		{ generatePath();	return;	}

			// left
			if(!end) 	end = nodeCheck(-1, 0, STRAIGHT_COST, bestNodeSoFar);
			if(end)		{ generatePath();	return;	}


			// upright
			if(!end) 	end = nodeCheck(1, -1, DIAG_COST, bestNodeSoFar);
			if(end)		{ generatePath();	return;	}

			//down right
			if(!end) 	end = nodeCheck(1, 1, DIAG_COST, bestNodeSoFar);
			if(end)		{ generatePath();	return;	}

			// down left
			if(!end) 	end = nodeCheck(-1, 1, DIAG_COST, bestNodeSoFar);
			if(end)		{ generatePath();	return;	}

			// up left
			if(!end) 	end = nodeCheck(-1, -1, DIAG_COST, bestNodeSoFar);
			if(end)		{ generatePath();	return;	}
		}
		// loooooooooooooop until complete
	}
	// if we get to this spot, we cannot reach the target, in theory
	// so lets recalculate the best possible spot and try again

	doneChecking = false;

	bestNodeSoFar = NULL;
	// first lets find the lowest manhattan distance on the board that is not -1
	int boardWidth = board->getBoardWidth();
	int boardHeight = board->getBoardHeight();

	int bestManhattan = boardWidth + boardHeight + 1;

	int bestManhattanX = -1;
	int bestManhattanY = -1;
	
	for(int x = 0; x < boardWidth; x++)
	{
		for(int y = 0; y < boardHeight; y++)
		{
			if((nodeList[x][y]->getManhattanDistance() < bestManhattan) &&
			   (nodeList[x][y]->getManhattanDistance() > -1))
			{
				bestManhattan = nodeList[x][y]->getManhattanDistance();
				bestManhattanX = x;
				bestManhattanY = y;
			}
		}
	}

	if(bestManhattanX != -1)
	{
		endX = bestManhattanX;
		endY = bestManhattanY;
		generatePath();
	}
}


bool rvbPath::nodeCheck(int offX, int offY, int moveCost, aStarNode* potentialParentNode)
{
	//offX =  0;
	//offY = -1;
	//moveCost = STRAIGHT_COST;
	int		currentX = potentialParentNode->getX();
	int		currentY = potentialParentNode->getY();

	int		destX = currentX + offX;
	int		destY = currentY + offY;

	// if we are on the board somewhere! hurrah
	if(	((destX) >= 0) &&
		((destX) < boardWidth) &&
		((destY) >= 0) &&
		((destY) < boardHeight)	)
	{
		// lets check the two adjacent squares if we're moving diagonal
		// as a rule, if we are moving diagonal then both the x and y offsets will be nonzero
		bool keepGoing = true;

		if( (offX != 0) && (offY != 0) )
		{
			if( (board->getTileTypeAt(destX, currentY) == TT_OBSTACLE) ||
				(board->getTileTypeAt(currentX, destY) == TT_OBSTACLE) )
			{
				keepGoing = false;
			}
		}
			
		// if the tile at the destination square is avaialble
		if(nodeList[destX][destY]->getAvailable() && keepGoing)
		{
			// lets add it to the open list
			openList.push_back(nodeList[destX][destY]);
			// then lets make ourself the parent and pass in some info
			if(potentialParentNode->getSteps() > 0)
			{
				nodeList[destX][destY]->setParent(potentialParentNode, potentialParentNode->getSteps() + moveCost);
			}
			else
			{
				nodeList[destX][destY]->setParent(potentialParentNode, moveCost);
			}

			if((destX == endX) && (destY == endY))
			{
				// look ma, we found the end square
				return true;
			}
		}
	}
	return false;
}

//=======================================
//	Returns the next queued up
// path node
//
aStarNode* rvbPath::getCurrentPathNode()
{
	if(calculatedPath.size() != 0)
	{
		return (*iCalcPath);
	}
	return NULL;
}

//=======================================
//	Increments the path node & returns it
//
aStarNode* rvbPath::advancePathNode()
{
	if(calculatedPath.size() != 0)
	{
		if((*iCalcPath) != calculatedPath.back())
		{
			return (*(++iCalcPath));
		}
	}
	return NULL;
}


//=======================================
//	Checks every position in the path
// from the current position (itr) in the path
// RETURNS: TRUE if it's clear
//			FALSE if it's obstructed
//
bool rvbPath::isPathStillValid()
{
	if(calculatedPath.size() > 0)
	{
		if(((*iCalcPath) != NULL) && ((*iCalcPath) != calculatedPath.back()))
		{
			std::vector<aStarNode*>::iterator itr = iCalcPath;
			itr++;

			for(; itr != calculatedPath.end(); itr++)
			{
				if(!board->isTileValidMove((*itr)->getX(), (*itr)->getY()))
				{
					return false;
				}
			}
			return true;
		}
		else 
		{
			return false;
		}
	}
	return true;
}

void rvbPath::recalcPath(int startX_n, int startY_n, int endX_n, int endY_n)
{
	// maybe so shit won't warp back when their path gets obstructed
	//calculatedPath.clear();

	startX = startX_n;
	startY = startY_n;

	bool sameEnd = false;
	if((endX == endX_n) &&
		(endY == endY_n))
	{
		sameEnd = true;
	}
	else
	{
		endX = endX_n;
		endY = endY_n;
	}
	// cout << "Recalculating path from " << startX << ", " << startY << " to "<< endX << ", " << endY << endl;
	

	//=========================
	// Resets the board 
	if(sameEnd)
	{
		for(int x = 0; x < boardWidth; x++)
		{
			for(int y = 0; y < boardHeight; y++)
			{
				nodeList[x][y]->resetSameTarget(board->isTileValidMove(x, y));
			}
		}	
	}
	else
	{
		for(int x = 0; x < boardWidth; x++)
		{
			for(int y = 0; y < boardHeight; y++)
			{
				nodeList[x][y]->resetNewTarget(endX, endY, (board->isTileValidMove(x, y)));
			}
		}
	}

	nodeList[startX][startY]->setParent(NULL, 0);
	openList.clear();
	openList.push_back(nodeList[startX][startY]);

	processPath();
	iCalcPath = calculatedPath.begin();
}


// draws the calculated path
bool rvbPath::drawPath(double scaleFactor, int offX, int offY, int width, entityType type)
{
	stringstream tempStream;
	string manhattan;
	string steps;
	string hueristic;

	// this draws just the single path assuming there is a full path
	int pathSize = calculatedPath.size();

	for(int x = 0; x < pathSize; x++)
	{
		int tempX = calculatedPath[x]->getX();
		int tempY = calculatedPath[x]->getY();

		oglTexture2D* tmpImg = NULL;
		switch(type)
		{
		case RED:
			{
				tmpImg = GameVars->redPathImg;
			break;
			}
		case BLUE:
			{
				tmpImg = GameVars->bluePathImg;
			break;
			}
		}

		tmpImg->drawImage((int)	(width*scaleFactor),			   // Width
								 (int)	(width*scaleFactor),	   // Height
								((tempX*width)*scaleFactor)+offX,  // X
								((tempY*width)*scaleFactor)+offY); // Y

		//// A* Debug Text Info
		//int tempInt = 0;
		//tempStream.str("");
		//tempInt = calculatedPath[x]->getHueristicValue();
		//if(tempInt >= 0)	{	tempStream << tempInt;	}
		//hueristic = tempStream.str();

		//tempStream.str("");
		//tempInt = calculatedPath[x]->getManhattanDistance();
		//if(tempInt >= 0)	{	tempStream << tempInt;	}
		//manhattan = tempStream.str();

		//tempStream.str("");
		//tempInt = calculatedPath[x]->getSteps();
		//if(tempInt >= 0)	{	tempStream << tempInt;	}
		//steps = tempStream.str();

		//// A* Debug Text Info
		//GameVars->fontArialRed12.drawText(((tempX*width)*scaleFactor)+ offX - (width * scaleFactor * .33) + (width * scaleFactor * 0.33),
		//							   ((tempY*width)*scaleFactor)+ offY - (width * scaleFactor * .33) + (width * scaleFactor * 0.33), steps, 10);

		//GameVars->fontArialRed12.drawText(((tempX*width)*scaleFactor)+ offX + (width * scaleFactor * .33) + (width * scaleFactor * 0.33),
		//							   ((tempY*width)*scaleFactor)+ offY - (width * scaleFactor * .33) + (width * scaleFactor * 0.33), manhattan, 10);		

		//GameVars->fontArialRed12.drawText(((tempX*width)*scaleFactor)+ offX - (width * scaleFactor * .33) + (width * scaleFactor * 0.33),
		//							   ((tempY*width)*scaleFactor)+ offY + (width * scaleFactor * .33) + (width * scaleFactor * 0.33), hueristic, 10);
	}
	return true;
}

// draws the board and color in open/closed list
bool rvbPath::drawAll(double scaleFactor)
{
	// this draws the entire state of the board
	return true;
}

void rvbPath::generatePath()
{
	bool foundStart = false;
	// find the end tile
	aStarNode* currentNode = nodeList[endX][endY];

	vector<aStarNode*>	tempPath;

	// make sure it has a parent			//tempPath
	if(currentNode->getParent() == NULL)
	{
		// if it doesn't have a parent, set the path to NULL!
		// we never quite reached it, lets leave this area
		tempPath.clear();
	}
	else
	{
		while(!foundStart)
		{
			// true or false?
			//(!(!(!(!(!(!(!(!(!(!(!(!true))))))))))));

			// lets push the current node onto the pathing...
			tempPath.push_back(currentNode);

			// first node (end tile) will always have a parent at this point, but eventually we get 
			// to the start node which doesn't, that's how we know we have reached the start...
			if(currentNode->getParent() != NULL)
			{
				// we haven't reached the start yet
				// lets push this node onto the path
				
				// lets update current node
				currentNode = currentNode->getParent();
			}
			else
			{
				// at this point, currentNode is pointing to the start tile so...
				foundStart = true;
			}
		}
	}

	// once we reach the start, lets just reverse the bitch so we can go start to end hurrah?
	calculatedPath.clear();
	
	for (int x = tempPath.size()-1; x >= 0; x--) 
	{   
		calculatedPath.push_back(tempPath[x]);
    }
}

bool rvbPath::isThereACalculatedPath()
{
	if(calculatedPath.size() > 0)
	{
		return true;
	}
	return false;
}