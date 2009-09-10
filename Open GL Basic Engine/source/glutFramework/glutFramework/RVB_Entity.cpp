#include "RVB_entity.h" 
#include "RVB_Map.h"

//		FUN NAME  rvbWeapon(dmg, range, clip, ammo)
#define RVBPISTOL rvbWeapon(10, 6, 10, 10);
#define RVBSHOTTY rvbWeapon(30, 3, 4, 4);
#define RVBRIFFLE rvbWeapon(20, 12, 1, 1);


void RVB_Entity::Update()
{
	// Query how long it's been since our last update
	timeSinceLastUpdate = clock() - timeOfLastUpdate;

	// We want this for time stepping.
	if(timeSinceLastUpdate > (timeStep*0)) // (timestep == number of seconds)
	{
		// first we process higher order brainpower!
		performBrainFunction();

		// then we do shit
		//enum entityState { ATTACKING, TAKING_COVER, MOVING, IDLE, SCANNING };
		switch(myLowerState)
		{
		case ATTACKING:
			break;

		case TAKING_COVER:
			break;

		case MOVING:
			doMove();
			break;

		case IDLE:
			break;

		case SCANNING:
			break;

		default:
			// you just lost the game
			break;
		}

	}
	//////////////////////////////
	// Log our update
	timeOfLastUpdate = clock();
		
}

void RVB_Entity::performBrainFunction()
{
	// this will be replaced with something way cooler than a simple switch statement and if/thens

	//enum higherState { CHASING, EVADING, ATTACKMOVE, HIGHERMOVING, HIGHERATTACKING, HIGHERIDLE };
	myLowerState = IDLE;

	switch(myHigherState)
	{
	case CHASING:
		// is the selected entity target still at our target spot?
		//this->myEntityTarget
		if(myEntityTarget != NULL)
		{
			// if not, move our target spot to the entity's location
			if( (myEntityTarget->getXPos() != targetX) ||
				(myEntityTarget->getYPos() != targetY))
			{
				setTarget(myEntityTarget->getXPos(), myEntityTarget->getYPos());
			}
			myLowerState = MOVING;
			
			// see if our selected entity target is a good guy or a bad guy

			// if a bad guy...
			double distanceToTarget = GameVars->getDistanceToTarget(xPos, yPos, targetX, targetY);
			if(myEntityTarget->getType() != type)
			{
				// see if we are in any weapon range of our enemy
				

				// for now this will just check weapon 1
				if(distanceToTarget < myWeapon1->getRange())
				{
					// if so, set state to attacking
					myLowerState = ATTACKING;
					// and break
					break;
				}
			}
			else
			{
				if(distanceToTarget < 2)
				{
					myLowerState = IDLE;
					break;
				}
			}
		}
		
		// otherwise set state to moving
		myLowerState = MOVING;
		break;

	case EVADING:
		break;

	case ATTACKMOVE:
		break;

	case HIGHERMOVING:
		if(myPath != NULL)
		{
			myLowerState = MOVING;
		}
		else
		{
			myLowerState = IDLE;
		}
		break;

	case HIGHERATTACKING:
		break;

	case HIGHERIDLE:
		break;

	default:
		// you just lost the game
		break;
	}
}

void RVB_Entity::doMove()
{
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
			targetX = -1;
			targetY = -1;
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
}

void RVB_Entity::setState(higherState newState)
{
	myHigherState = newState;
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

	// if i have anyone selected, lets draw a little circle around them
	if(myEntityTarget != NULL)
	{
		GameVars->rvbHeyYouWithTheFace->drawImage((int)(tileWidth*scaleFactor),				// Width
								(int)(tileWidth*scaleFactor),				// Height
								((myEntityTarget->getXPos()*tileWidth)*scaleFactor)+mapOffsetX,  // X
								((myEntityTarget->getYPos()*tileWidth)*scaleFactor)+mapOffsetY); // Y
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

	myAmmoPack.pistolAmmo = 0;
	myAmmoPack.rifleAmmo = 0;
	myAmmoPack.shotgunAmmo = 0;
	
	myWeapon1 = new RVBSHOTTY;
	myWeapon2 = new RVBRIFFLE;
	currentWeapon = myWeapon2;

	myEntityTarget = NULL;

	myHigherState = HIGHERIDLE;
	myLowerState = IDLE;
}

entityType RVB_Entity::getType()
{
	return type;
}

void RVB_Entity::setEnemyTarget(RVB_Entity *newTarget)
{
	myEntityTarget = newTarget;
}