#include "RVB_entity.h" 
#include "RVB_Map.h"

void RVB_Entity::Update()
{
	// Query how long it's been since our last update
	timeSinceLastUpdate = clock() - timeOfLastUpdate;

	// We want this for time stepping.
	if(timeSinceLastUpdate > (timeStep*1000)) // (timestep == number of seconds)
	{
		// DEBUG
		//cout << "It's been " << (float)(timeStep*1000) << "ms for objType: ";
		//if(type == RED)			{ cout << "RED" << endl; }
		//else if(type == BLUE)	{ cout << "BLUE" << endl; } 
		//else					{ cout << "UNKNOWN" << endl; }
		
		
		//////////////////////////////
		// Recalculate our path
		// 
		
		// if we have a path
		if(myPath != NULL)
		{
			// if we haven't reached our target position...
			if(((xPos != targetX) || (yPos != targetY)) && (targetX != -1) && (targetY != -1))
			{
				// lets make sure its still valid
				if(!(myPath->isPathStillValid()))
				{
					delete myPath;
					myPath = new rvbPath(xPos, yPos, targetX, targetY, board->getBoardWidth(), board->getBoardHeight(), board);
					//myPath->recalcPath(xPos, yPos, targetX, targetY);
				}
				/*else if(!(myPath->isThereACalculatedPath()))
				{
					myPath->recalcPath(xPos, yPos, targetX, targetY);
				}*/
			}
			else
			{
				delete myPath;
				myPath = NULL;
			}
		}
		

		//setTarget(targetX, targetY);

		//////////////////////////////
		if(myPath != NULL)
		{
			aStarNode* destNode = NULL;
			destNode = myPath->advancePathNode();
			if(destNode != NULL)
			{
				if(board->isTileValidMove(destNode->getX(), destNode->getY()))
				{
					xPos = destNode->getX();
					yPos = destNode->getY();
				}
			}
		}

		//////////////////////////////
		// Log our update
		timeOfLastUpdate = clock();
	}
}
	
void RVB_Entity::Draw(int tileWidth, double scaleFactor, int mapOffsetX, int mapOffsetY)
{
	oglTexture2D* tempImage = NULL;

	// first draw myself
	switch(type)
	{
	case RED:
		tempImage = GameVars->rvbEntityRed;
		break;
	case BLUE:
		tempImage = GameVars->rvbEntityBlue;
		break;
	default:
		tempImage = NULL;
		break;
	}

	tempImage->drawImage((int)	(tileWidth*scaleFactor),			 // Width
								(int)(tileWidth*scaleFactor),			 // Height
								((xPos*tileWidth)*scaleFactor)+mapOffsetX,  // X
								((yPos*tileWidth)*scaleFactor)+mapOffsetY); // Y

	// if i have a path, draw the path
	if(myPath != NULL)
	{
		myPath->drawPath(scaleFactor, mapOffsetX, mapOffsetY, tileWidth, type);
	}
}

void RVB_Entity::generatePath()
{

}
	
//RVB_Entity::RVB_Entity()
//{
//}
	
RVB_Entity::~RVB_Entity()
{
}

int RVB_Entity::getXPos()
{
	return xPos;
}

int RVB_Entity::getYPos()
{
	return yPos;
}

int RVB_Entity::getTargetX()
{
	return targetX;
}
int RVB_Entity::getTargetY()
{
	return targetY;
}

void RVB_Entity::clearTarget()
{
	targetX = -1;
	targetY = -1;

	delete myPath;
	myPath = NULL;
}

void RVB_Entity::setTarget(int newX, int newY)
{
	//if((targetX != newX) || (targetY != newY))
	{
		targetX = newX;
		targetY = newY;


		/*if(myPath != NULL)
		{
			delete myPath;
		}*/
		if (myPath == NULL)
		{
			myPath = new rvbPath(xPos, yPos, targetX, targetY, board->getBoardWidth(), board->getBoardHeight(), board);
		}
		else
		{
			delete myPath;
			myPath = new rvbPath(xPos, yPos, targetX, targetY, board->getBoardWidth(), board->getBoardHeight(), board);
			//myPath->recalcPath(xPos, yPos, targetX, targetY);
		}
	}
}

void RVB_Entity::setPosition(int newX, int newY)
{
	xPos = newX;
	yPos = newY;
}

RVB_Entity::RVB_Entity(entityType newType, int newX, int newY, entityDirection newDirection, RVB_Map* parentBoard, vector<RVB_Entity*>* boardObjectList)
:timeOfLastUpdate(0), timeSinceLastUpdate(0), timeStep(0.5)
{
	xPos = newX;
	yPos = newY;
	targetX = -1;
	targetY = -1;
	health = 100;
	type = newType;
	board = parentBoard;
	objectList = boardObjectList;
	myPath = NULL;

	timeOfLastUpdate = clock();
}