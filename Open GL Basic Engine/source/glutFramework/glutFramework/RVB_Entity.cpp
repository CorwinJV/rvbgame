#include "RVB_entity.h" 
#include "RVB_Map.h"
#include "math.h"

//		FUN NAME  rvbWeapon(dmg, range, clip, ammo,       type,		firing rate, reload time)
#define RVBPISTOL rvbWeapon(10,      6,   10,   10, WEAPON_PISTOL,	1000,		 1000);
#define RVBSHOTTY rvbWeapon( 3,      3,    4,    4, WEAPON_SHOTTY,	3000,		 3000);
#define RVBRIFFLE rvbWeapon(20,     12,    1,    1, WEAPON_RIFFLE,	5000,		 2000);


void RVB_Entity::Update()
{
	if(health <= 0)
	{
		return;
	}
	// Query how long it's been since our last update
	timeSinceLastUpdate = clock() - timeOfLastUpdate;
	timeSinceLastFire   = clock() - timeOfLastFire;
	timeSinceLastReload = clock() - timeOfLastReload;

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
			
			// if we're trying to fire at something that isn't an entity
			if(myEntityTarget != NULL)
			{
				fireAtX = myEntityTarget->getXPos();
				fireAtY = myEntityTarget->getYPos();
			}
			
			// see if the firing rate of the gun we're using will let us fire again
			if(timeSinceLastFire > currentWeapon->getFireRate())
			{
				// see how many bullets we have left in the clip
				bulletsLeft = currentWeapon->getAmmoLeftInClip();

				// if we have bullets left to shoot
				if(bulletsLeft > 0)
				{
					// then fire the shot, deduct one from the ammo and set it as the new amount
					double fireFromX = xPos + 0.5;
					double fireFromY = yPos + 0.5;
					fireAtX += 0.5;
					fireAtY += 0.5;

					// this variable controls the number of bullets sprayed by the shotty
					int shotgunBullets = 5;

					// see where the target we're shooting at is, 
					// so we can adjust the spawn point of the bullet
					// so it doesn't hit us!
					if(fireAtX > fireFromX)
					{
						fireFromX += 0.5;
					}
					else if(fireAtX < fireFromX)
					{
						fireFromX -= 0.5;
					}
					if(fireAtY > fireFromY)
					{
						fireFromY += 0.5;
					}
					else if(fireAtY < fireFromY)
					{
						fireFromY -= 0.5;
					}

					// if we're not shooting the shotgun
					if(!(currentWeapon->getType() == WEAPON_SHOTTY))
					{
						// SHOOT!
						board->makeBullet(currentWeapon->shotFired(fireFromX , fireFromY , fireAtX, fireAtY));
						// log the time of last shot taken
						timeOfLastFire = clock();
					}
					else	//otherwise we are shooting the shotgun and have to make multiple shots
					{
						for(int x = 0; x < shotgunBullets; x++)
						{
							board->makeBullet(currentWeapon->shotFired(fireFromX , fireFromY , 
																	(rand() / 32768.0) - 0.5 + fireAtX, 
																	(rand() / 32768.0) - 0.5 + fireAtY) );
						}
						
						// log the time of last shot taken
						timeOfLastFire = clock();
					}

					bulletsLeft -= 1;
					currentWeapon->setAmmoLeftInClip(bulletsLeft);
				}
				//otherwise try to reload
				else
				{
					if(timeSinceLastReload > currentWeapon->getReloadTime())
					{
						// reset the reload ammo to 0
						reloadAmmo = 0;

						// see what kind of gun we're using
						switch(currentWeapon->getType())
						{
							// set the ammo we have for each gun to reload
							// then pass in the number to attempt to reload
							// and if there was ammo reloaded, subtract it from cache
						case WEAPON_PISTOL:
							reloadAmmo = myAmmoPack.pistolAmmo;
							reload(reloadAmmo);
							timeOfLastReload = clock();
							if(reloadAmmo > 0)
							{
								myAmmoPack.pistolAmmo -= currentWeapon->getAmmoLeftInClip();
							}
							break;
						case WEAPON_SHOTTY:
							reloadAmmo = myAmmoPack.shotgunAmmo;
							reload(reloadAmmo);
							timeOfLastReload = clock();
							if(reloadAmmo > 0)
							{
								myAmmoPack.shotgunAmmo -= currentWeapon->getAmmoLeftInClip();
							}
							break;
						case WEAPON_RIFFLE:
							reloadAmmo = myAmmoPack.rifleAmmo;
							reload(reloadAmmo);
							timeOfLastReload = clock();
							if(reloadAmmo > 0)
							{
								myAmmoPack.rifleAmmo -= currentWeapon->getAmmoLeftInClip();
							}
							break;
						default:
							cout << "something is broke, we returned a type of gun that doesnt exist in attacking state" << endl;
							break;
						}
					}
				}
			}
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
			// is it alive?
			if((*objectList)[x]->getHealth() > 0)
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
								if(entityCanSeeTargetAt(enemyX, enemyY))
								{
									myEntityTarget = board->getSelectableEntityAtGridCoord(enemyX, enemyY);
									closestDistance = distanceToTarget;
								}
							}
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
			if((distanceFriendlyToEnemy <= entityVisionRadius) &&
			   (entityCanSeeTargetAt(enemyX, enemyY)) &&
			   (myEntityTarget->getHealth() > 0))
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

		//cout << "movementSourceNodeX: " << movementSourceNodeX << endl;
		//cout << "movementSourceNodeY: " << movementSourceNodeY << endl;

		//cout << "currentPathNodeX: " << myPath->getCurrentPathNode()->getX() << endl;
		//cout << "currentPathNodeY: " << myPath->getCurrentPathNode()->getY() << endl;

		//cout << "xDiff: " << xDiff << endl;
		//cout << "yDiff: " << yDiff << endl;
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
		// draw a health bar
		double doubleHealth = (double)health;

		double barWidth = 89 * ((double)health / 100);
		double barHeight = 8;
		double barOffsetX = 19 * scaleFactor;
		double barOffsetY = 8 * scaleFactor;

		double redFade = 1;
		//double yellowFade = ((double)health*2) < 1 ? (double)health : 1;
		double greenFade = (double)health/100;


		// now lets draw our little colored bars
		GameVars->redPixel->drawImageFaded(redFade, barWidth * scaleFactor, barHeight * scaleFactor,
									(double)(xPos * tileWidth * scaleFactor) + mapOffsetX + barOffsetX,  // X
									(double)(yPos * tileWidth * scaleFactor) + mapOffsetY + barOffsetY); // Y

		//GameVars->yellowPixel->drawImageFaded(yellowFade, barWidth * scaleFactor, barHeight * scaleFactor,
		//							(double)(xPos * tileWidth * scaleFactor) + mapOffsetX + barOffsetX,  // X
		//							(double)(yPos * tileWidth * scaleFactor) + mapOffsetY + barOffsetY); // Y

		GameVars->greenPixel->drawImageFaded(greenFade, barWidth * scaleFactor, barHeight * scaleFactor,
									(double)(xPos * tileWidth * scaleFactor) + mapOffsetX + barOffsetX,  // X
									(double)(yPos * tileWidth * scaleFactor) + mapOffsetY + barOffsetY); // Y



		// draw a nice border for the health bar
		GameVars->healthBorder->drawImage((int)(tileWidth*scaleFactor),			 // Width
									(int)(tileWidth*scaleFactor),			 // Height
									(double)(xPos * tileWidth * scaleFactor) + mapOffsetX,  // X
									(double)(yPos * tileWidth * scaleFactor) + mapOffsetY); // Y
		
	}
}

void RVB_Entity::drawEntityTarget(int tileWidth, double scaleFactor, double mapOffsetX, double mapOffsetY)
{
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

void RVB_Entity::pickUpAmmo(RVB_WeaponType ammoType, int ammoAmt_n)
{
	int ammoAmt = ammoAmt_n;

	// make sure we sent in a positive number
	if(ammoAmt < 0)
	{
		ammoAmt = 0;
	}

	// see what type of ammo has been sent in and add it to the cache
	switch(ammoType)
	{
	case WEAPON_PISTOL:
		myAmmoPack.pistolAmmo += ammoAmt;
		break;
	case WEAPON_SHOTTY:
		myAmmoPack.shotgunAmmo += ammoAmt;
		break;
	case WEAPON_RIFFLE:
		myAmmoPack.rifleAmmo += ammoAmt;
		break;
	default:
		cout << "you sent in an invalid weapon type in RVB_Entity::pickUpAmmo" << endl;
		break;
	}
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

	myAmmoPack.pistolAmmo = 5;
	myAmmoPack.rifleAmmo = 2;
	myAmmoPack.shotgunAmmo = 10;
	
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
	// subtract the damage from the enitities health
	health -= damage_n;

	// if the health drops below 0, and the entity isn't already dead
	if((health <= 0) && (myHigherState != DEAD))
	{
		// set their state to dead
		health = 0;
		myHigherState = DEAD;
	}
}

void RVB_Entity::reload(int bullets)
{
	int reloadAmmo = bullets;

	// if we have ammo, then reload
	if(reloadAmmo > 0)
	{
		// send in whatever amount of ammo we have and what kind of gun we're using
		// **************************
		// ** the second parameter is hardcoded as 0 now, as we're only going to let the 
		// ** entities reload when their clip is empty
		// **************************
		currentWeapon->reload(reloadAmmo, 0, currentWeapon->getType());
	}
	else // well we have no more bullets, so lets go ahead and switch weapons
	{
		// first see if we're using weapon 1
		if(currentWeapon->getType() == myWeapon1->getType())
		{
			// if so switch to weapon 2
			currentWeapon = myWeapon2;
		}
		else
		{
			// otherwise we were using weapon 2, switch to weapon 1
			currentWeapon = myWeapon1;
		}
	}
}

bool RVB_Entity::entityCanSeeTargetAt(double targetXn, double targetYn)
{
	// if they ask if you can see yourself, just say yeah we can
	if((targetXn == xPos) && (targetYn == yPos))
	{
		return true;
	}
	// we are going to make two assumptions
	// that our xpos is based on our top left
	// that their target is based off of it's top left
	

	double storedTargetX = targetXn;// + 0.5;
	double storedTargetY = targetYn;// + 0.5;

	double movingCheckerX = xPos;// + 0.5;
	double movingCheckerY = yPos;// + 0.5;

	double incrementX = targetXn - xPos;
	double incrementY = targetYn - yPos;


	double magicNumber = max(GameVars->dAbs(incrementX), GameVars->dAbs(incrementY));

	if(magicNumber != 0)	// damned black holes
	{
		magicNumber = 1/magicNumber;
	}

	incrementX *= magicNumber;
	incrementY *= magicNumber;

	/*if(incrementX > 0)
	{
		movingCheckerX += 0.5;
	}
	if(incrementX < 0)
	{
		movingCheckerX -= 0.5;
	}
	if(incrementY > 0)
	{
		movingCheckerY += 0.5;
	}
	if(incrementY <0)
	{
		movingCheckerY -= 0.5;
	}*/

	// so now we have in an increment of 1 for our larger number and some percentage of this for the other number
	bool done = false;

	//cout << "Source = " << xPos << ", " << yPos << " and target = " << targetXn << ", " << targetYn << endl;
	//cout << "increment X/y = " << incrementX << ", " << incrementY << endl;

	while(!done)
	{
		if( (movingCheckerX >= 0) && (floor(movingCheckerX) < board->getBoardWidth()) &&
			(movingCheckerY >= 0) && (floor(movingCheckerY) < board->getBoardHeight()) )
		{
			if(board->isThereAnObstacleAt(movingCheckerX, movingCheckerY))
			{
				return false;
			}
		}

		if( ((movingCheckerX > storedTargetX - incrementX) && (movingCheckerX > xPos)) ||
			((movingCheckerX < storedTargetX - incrementX) && (movingCheckerX < xPos)) ||
			((movingCheckerY > storedTargetY - incrementY) && (movingCheckerY > yPos)) ||
			((movingCheckerY < storedTargetY  - incrementY) && (movingCheckerY < yPos)) ||
			((movingCheckerX == storedTargetX - incrementX) && (movingCheckerY == storedTargetY - incrementY))
			)
		{
			done = true;
		}
		//cout << "MovingCheckerX/y " << movingCheckerX << ", " << movingCheckerY << endl;
		movingCheckerX += incrementX;
		movingCheckerY += incrementY;
		//cout << "MovingCheckerX/y " << movingCheckerX << ", " << movingCheckerY << endl;
		//cout << "----------------------------------------------------------------------" << endl;
	}
	return true;
}