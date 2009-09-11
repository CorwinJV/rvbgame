#include "RVB_entity.h" 
#include "RVB_Map.h"

//		FUN NAME  rvbWeapon(dmg, range, clip, ammo, type)
#define RVBPISTOL rvbWeapon(10, 6, 10, 10, WEAPON_PISTOL);
#define RVBSHOTTY rvbWeapon(30, 3, 4, 4, WEAPON_SHOTTY);
#define RVBRIFFLE rvbWeapon(20, 12, 1, 1, WEAPON_RIFFLE);


void RVB_Entity::Update()
{
	// Query how long it's been since our last update
	timeSinceLastUpdate = clock() - timeOfLastUpdate;

	// We want this for time stepping.
	if(timeSinceLastUpdate > (timeStep)) // (timestep == number of seconds)
	{
		// first we process higher order brainpower!
		performBrainFunction();

		// then we do shit
		//enum entityState { ATTACKING, TAKING_COVER, MOVING, IDLE, SCANNING };
		switch(myLowerState)
		{
		case ATTACKING:
			//cout << "bang, bang, BAAAAAAAANG, you dead?" << endl;
			board->makeBullet(currentWeapon->shotFired(xPos, yPos, fireAtX, fireAtY));
			break;

		case TAKING_COVER:
			break;

		case MOVING:
			doMove();
			break;

		case IDLE:
			break;

		case SCANNING:
			doEnemyScan();
			break;

		default:
			break;
		}

	}
	//////////////////////////////
	// Log our update
	timeOfLastUpdate = clock();
}

void RVB_Entity::doEnemyScan()
{
	int numEntities = (*objectList).size();

	// find the closest enemy to myself
	double closestDistance = 100000000000;
	for(int x = 0; x < numEntities; x++)
	{
		if((*objectList)[x]->getType() != type)
		{
			// we've found an enemy!, lets check its distance
			int enemyX = (*objectList)[x]->getXPos();
			int enemyY = (*objectList)[x]->getYPos();
			double distanceToTarget = GameVars->getDistanceToTarget(xPos, yPos, enemyX, enemyY);
			if(distanceToTarget < closestDistance)
			{
				// we have found the closest enemy, lets see if we can see him...
				for(int innerX = 0; innerX < numEntities; innerX++)
				{
					if((*objectList)[innerX]->getType() == type)
					{
						int friendlyX = (*objectList)[innerX]->getXPos();
						int friendlyY = (*objectList)[innerX]->getYPos();
						double distanceFriendlyToEnemy = GameVars->getDistanceToTarget(friendlyX, friendlyY, enemyX, enemyY);
						// we've found a friendly, lets see if its within vision radius of the enemy
						if(distanceFriendlyToEnemy <= entityVisionRadius)
						{
							//cout << "found an enemy that's close and visible" << endl;
							// ok now we have the closest enemy so far and its within a friendly entity's vision radius
							// *********************************************
							// RIGHT HERE IS WHERE LINE OF SIGHT GOES, FOR NOW WE IGNORE IT
							// *********************************************
							myEntityTarget = board->getSelectableEntityAtGridCoord(enemyX, enemyY);
							closestDistance = distanceToTarget;
						}
					}
				}
			}
		}
	}
}

bool RVB_Entity::canStillSeeEnemy()
{
	int enemyX = myEntityTarget->getXPos();
	int enemyY = myEntityTarget->getYPos();
	int numEntities = (*objectList).size();

	for(int innerX = 0; innerX < numEntities; innerX++)
	{
		if((*objectList)[innerX]->getType() == type)
		{
			int friendlyX = (*objectList)[innerX]->getXPos();
			int friendlyY = (*objectList)[innerX]->getYPos();
			double distanceFriendlyToEnemy = GameVars->getDistanceToTarget(friendlyX, friendlyY, enemyX, enemyY);
			// we've found a friendly, lets see if its within vision radius of the enemy
			if(distanceFriendlyToEnemy <= entityVisionRadius)
			{
			//	cout << "found an enemy that's close and visible" << endl;
				// ok now we have the closest enemy so far and its within a friendly entity's vision radius
				// *********************************************
				// RIGHT HERE IS WHERE LINE OF SIGHT GOES, FOR NOW WE IGNORE IT
				// *********************************************
				return true;
			}
		}
	}
	return false;
}



void RVB_Entity::performBrainFunction()
{
	// this will be replaced with something way cooler than a simple switch statement and if/thens

	//enum higherState { CHASING, EVADING, ATTACKMOVE, HIGHERMOVING, HIGHERATTACKING, HIGHERIDLE };
	myLowerState = IDLE;
	int scanDelayBy = 5;

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
				if(distanceToTarget < currentWeapon->getRange())
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

		// we have two possibilites, we either have a target or we don't have a target
		
		// if we don't have a target
		// and we haven't scanned
		// start scanning
		// otherwise, move


		// if we have a target, start attacking it, 
		if((myEntityTarget != NULL) && (canStillSeeEnemy()))
		{
			if(GameVars->getDistanceToTarget(xPos, yPos, myEntityTarget->getXPos(), myEntityTarget->getYPos()) <= currentWeapon->getRange())
			{
				myLowerState = ATTACKING;
			}
			else
			{
				if((!scanned) && (scanDelay > scanDelayBy))
				{
					scanDelay = 0;
					scanned = true;
					myLowerState = SCANNING;
				}
				else
				{
					scanned = false;
					scanDelay++;
					if(myPath != NULL)
					{
						myLowerState = MOVING;
					}
					else
					{
						myHigherState = HIGHERIDLE;
						myLowerState = IDLE;
					}
				}
			}
		}
		else
		{
			if((!scanned) && (scanDelay > scanDelayBy))
			{
				scanDelay = 0;
				scanned = true;
				myLowerState = SCANNING;
			}
			else
			{
				scanDelay++;
				scanned = false;
				if(myPath != NULL)
				{
					myLowerState = MOVING;
				}
				else
				{
					myHigherState = HIGHERIDLE;
					myLowerState = IDLE;
				}
			}
		}
		break;

	case HIGHERMOVING:
		if(myEntityTarget != NULL)
		{
			myEntityTarget = NULL;
		}

		if(myPath != NULL)
		{
			myLowerState = MOVING;
		}
		else
		{
			myHigherState = HIGHERIDLE;
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
	// Smooth Movement Calcs
	// 
	double additionAmmount = defaultMovementIncr;
	if(myPath != NULL)
	{ 
		if(myPath->getCurrentPathNode() != NULL)
		{
			if(myPath->getCurrentPathNode()->getMovementCost() != 0)
			{
				double mMovementCost =  myPath->getCurrentPathNode()->getMovementCost();
				additionAmmount = (additionAmmount * (10.0 / mMovementCost));
			}
		}
	} 
	completionPercentage += additionAmmount;
	//cout << "completionPercentage: " << completionPercentage << endl;
	//cout << "additionAmmount: " << additionAmmount << endl;


	//////////////////////////////
	// Recalculate our path
	// 
	
	// if we have a path
	if(myPath != NULL)
	{
		if(completionPercentage > 1.0)
		{
			// if we haven't reached our target position...
			if(((xPos != targetX) || (yPos != targetY)) && (targetX != -1) && (targetY != -1))
			{
				// lets make sure its still valid
				if(!(myPath->isPathStillValid()))
				{
					delete myPath;
					myPath = new rvbPath(xPos, yPos, targetX, targetY, board->getBoardWidth(), board->getBoardHeight(), board);
				}
			}
			else
			{
				delete myPath;
				myPath = NULL;
				targetX = -1;
				targetY = -1;
				movementSourceNodeX = -1;
				movementSourceNodeY = -1;
			}
		}
	}
	else
	{
		movementSourceNodeX = -1;
		movementSourceNodeY = -1;
	}

	
	// 	storing source & destination
	// Set percentage of completion to 0.0 at init or when landing in new spot
	// When in moving state...
		// Incr percentage of completion
		// 
	// draw
	// 128 / (percentage of completion)
	//


	
	//////////////////////////////
	// Smooth Movement weee
	// 	
	if(myPath != NULL)
	{
		if(completionPercentage > 1.0)
		{
			// Store the current position in the path for reference
			movementSourceNodeX = myPath->getCurrentPathNode()->getX();
			movementSourceNodeY = myPath->getCurrentPathNode()->getY();

			// Advance the path node, and set the destination node.
			aStarNode* destNode = NULL;
			destNode = myPath->advancePathNode();
			if(destNode != NULL)
			{
				// Set the entity's position to the new node
				if(board->isTileValidMove(destNode->getX(), destNode->getY()))
				{
					xPos = destNode->getX();
					yPos = destNode->getY();
				}
			}
			// reset percentageCompletion, we aren't going anywhere yet
			completionPercentage = 0.0;
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

	int drawXPos = ((xPos*tileWidth)*scaleFactor)+mapOffsetX;
	int drawYPos = ((yPos*tileWidth)*scaleFactor)+mapOffsetY;
	if(myLowerState == MOVING && (movementSourceNodeX != -1 && movementSourceNodeY != -1) && myPath->getCurrentPathNode() != NULL)
	{
		double xDiff = movementSourceNodeX - myPath->getCurrentPathNode()->getX();
		double yDiff = movementSourceNodeY - myPath->getCurrentPathNode()->getY();

		cout << "movementSourceNodeX: " << movementSourceNodeX << endl;
		cout << "movementSourceNodeY: " << movementSourceNodeY << endl;

		cout << "currentPathNodeX: " << myPath->getCurrentPathNode()->getX() << endl;
		cout << "currentPathNodeY: " << myPath->getCurrentPathNode()->getY() << endl;

		cout << "xDiff: " << xDiff << endl;
		cout << "yDiff: " << yDiff << endl;
		tempImage->drawImage((int)(tileWidth*scaleFactor),			 // Width
									(int)(tileWidth*scaleFactor),			 // Height
									(double)drawXPos + (xDiff * ((128.0 * (1.0 - completionPercentage))*scaleFactor)),  // X
									(double)drawYPos + (yDiff * ((128.0 * (1.0 - completionPercentage))*scaleFactor))); // Y
	}
	else
	{
		tempImage->drawImage((int)	(tileWidth*scaleFactor),			 // Width
									(int)(tileWidth*scaleFactor),			 // Height
									drawXPos,  // X
									drawYPos); // Y
	}

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

	// now lets draw a health bar
	// if health <= 0... they be dead
	if(health <= 0)
	{
		// draw dead icon
		GameVars->deadIcon->drawImage((int)(tileWidth*scaleFactor),			 // Width
									(int)(tileWidth*scaleFactor),			 // Height
									(double)(xPos * tileWidth * scaleFactor) + mapOffsetX,  // X
									(double)(yPos * tileWidth * scaleFactor) + mapOffsetY); // Y
	}
	else
	{
		// otherwise, draw a health bar
		GameVars->healthBorder->drawImage((int)(tileWidth*scaleFactor),			 // Width
									(int)(tileWidth*scaleFactor),			 // Height
									(double)(xPos * tileWidth * scaleFactor) + mapOffsetX,  // X
									(double)(yPos * tileWidth * scaleFactor) + mapOffsetY); // Y
		
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

int RVB_Entity::getHealth()
{
	return health;
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
	:timeOfLastUpdate(0), timeSinceLastUpdate(0), timeStep(0.5),
	movementSourceNodeX(-1), movementSourceNodeY(-1), completionPercentage(0.0),
	defaultMovementIncr(.1)
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
	scanned = false;
	scanDelay = 0;
}

entityType RVB_Entity::getType()
{
	return type;
}

void RVB_Entity::setEnemyTarget(RVB_Entity *newTarget)
{
	myEntityTarget = newTarget;
}

void RVB_Entity::setHealth(int health_n)
{
	// add the amount of health sent in, by a spawn or health pack
	health += health_n;

	// make sure we aren't above full health
	if(health > 100)
	{
		health = 100;
	}
}

void RVB_Entity::applyDamage(int damage_n)
{
	health -= damage_n;

	if(health < 1)
	{
		myHigherState = DEAD;
	}
}