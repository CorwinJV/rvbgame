#include "RVB_entity.h" 
#include "RVB_Map.h"
#include "math.h"
//#define innovation true

//		FUN NAME  rvbWeapon(dmg, range, clip, ammo,       type,		firing rate, reload time, who fired the shot)
#define RVBPISTOL rvbWeapon(10,      6,   10,   10, WEAPON_PISTOL,	1000,		 1000,        this);
#define RVBSHOTTY rvbWeapon( 3,      3,    4,    4, WEAPON_SHOTTY,	3000,		 3000,        this);
#define RVBRIFFLE rvbWeapon(20,     12,    1,    1, WEAPON_RIFFLE,	5000,		 2000,        this);


void RVB_Entity::Update()
{
	double distanceToTarget = 0;
	RVB_Bullet* tempBullet;

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
					// we have bullets, and the firing rate has been checked
					canIShoot = true;

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

					// make sure someone from our team, or an obstacle isnt between us and the target
					if(!isThereAFriendlyEntityInTheWay(fireAtX, fireAtY) && entityCanSeeTargetAt(fireAtX, fireAtY))
					{
						// if we're not shooting the shotgun
						if(!(currentWeapon->getType() == WEAPON_SHOTTY))
						{			
							// calculate how far we are from our target
							distanceToTarget = GameVars->getDistanceToTarget(fireFromX, fireFromY, fireAtX, fireAtY);
							// SHOOT!
							tempBullet = currentWeapon->shotFired(fireFromX , fireFromY , fireAtX, fireAtY, distanceToTarget, myEntityTarget);
							board->makeBullet(tempBullet);
							// set the gun to unable to shoot
							canIShoot = false;
							// add the bullet to the gun owners vector
							shotsFired.push_back(tempBullet);
							// log the time of last shot taken
							timeOfLastFire = clock();
							// take away a bullet
							bulletsLeft -= 1;
						}
						else	//otherwise we are shooting the shotgun and have to make multiple shots
						{
							// calculate how far we are from our target
							distanceToTarget = GameVars->getDistanceToTarget(fireFromX, fireFromY, fireAtX, fireAtY);
							for(int x = 0; x < shotgunBullets; x++)
							{
								// create the bullet and fire it
								tempBullet = currentWeapon->shotFired(fireFromX , fireFromY , 
																		(rand() / 32768.0) - 0.5 + fireAtX, 
																		(rand() / 32768.0) - 0.5 + fireAtY, 
																		 distanceToTarget, myEntityTarget);
								board->makeBullet(tempBullet);
								// push the bullet onto the entity's shot fired vector
								shotsFired.push_back(tempBullet);
							}
							// take away a bullet
							bulletsLeft -= 1;
							// set the gun to unable to shoot
							canIShoot = false;
							// log the time of last shot taken
							timeOfLastFire = clock();
						}

						// after the shot(s) have been fired, set how much ammo we have left
						currentWeapon->setAmmoLeftInClip(bulletsLeft);
					}
					else	// there is a friendly entity, or obstacle in the way, so we need to move
					{
						cout << "Someone is in the way" << endl;
						// call function to create a new path here
					}
				}
				//otherwise try to reload
				else
				{
					// well we have no bullets, so no we can't shoot
					canIShoot = false;

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
			doneMoving = false;
			//doMove();
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

	if(!doneMoving)
	{
		doMove();
	}
	else
	{
		if(completionPercentage < 1.0)
		{
			completionPercentage += defaultMovementIncr;
		}
		if(completionPercentage > 1.0)
		{
			completionPercentage = 1.0;
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
				double enemyX = (*objectList)[x]->getXPos();
				double enemyY = (*objectList)[x]->getYPos();
				double distanceToTarget = GameVars->getDistanceToTarget(xPos, yPos, enemyX, enemyY);
				if(distanceToTarget < closestDistance)
				{
					// we have found the closest enemy, lets see if we can see him...
					for(int innerX = 0; innerX < numEntities; innerX++)
					{
						if((*objectList)[innerX]->getType() == type)
						{
							double friendlyX = (*objectList)[innerX]->getXPos();
							double friendlyY = (*objectList)[innerX]->getYPos();
							double distanceFriendlyToEnemy = GameVars->getDistanceToTarget(friendlyX, friendlyY, enemyX, enemyY);
							// we've found a friendly, lets see if its within vision radius of the enemy
							if(distanceFriendlyToEnemy <= entityVisionRadius)
							{
								//cout << "found an enemy that's close and visible" << endl;
								// ok now we have the closest enemy so far and its within a friendly entity's vision radius
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
	double enemyX = myEntityTarget->getXPos();
	double enemyY = myEntityTarget->getYPos();
	int numEntities = (*objectList).size();

	for(int innerX = 0; innerX < numEntities; innerX++)
	{
		if((*objectList)[innerX]->getType() == type)
		{
			double friendlyX = (*objectList)[innerX]->getXPos();
			double friendlyY = (*objectList)[innerX]->getYPos();
			double distanceFriendlyToEnemy = GameVars->getDistanceToTarget(friendlyX, friendlyY, enemyX, enemyY);
			// we've found a friendly, lets see if its within vision radius of the enemy
			if((distanceFriendlyToEnemy <= entityVisionRadius) &&
			   (entityCanSeeTargetAt(enemyX, enemyY)) &&
			   (myEntityTarget->getHealth() > 0))
			{
				//	cout << "found an enemy that's close and visible" << endl;
				// ok now we have the closest enemy so far and its within a friendly entity's vision radius
			
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
	RVB_Entity* tempEntity = NULL;
	bool imBeingWatched = false;
	bool imBeingChased  = false;
	int boardWidth = board->getBoardWidth();
	int boardHeight = board->getBoardHeight();

	// if my health is lower than 50
	if(health < 50)
	{
		// and there is someone coming at me
		//if(isAnyoneChasingMeThatICanSee())
		if(isAnyoneChasingMe())
		{
			// run like hell!
			imBeingChased = true;
			myHigherState = EVADING;
		}
	}

	// if my health is lower than 30
	if(health < 30)
	{
		// and someone is even looking at me
		//if(isAnyoneLookingAtMeThatICanSee())
		if(isAnyoneLookingAtMe())
		{
			// run like hell!
			imBeingWatched = true;
			myHigherState = EVADING;
		}
	}

	switch(myHigherState)
	{
	case CHASING:
		// is the selected entity target still at our target spot?
		//this->myEntityTarget
		if(myEntityTarget != NULL)
		{
			// if the guy we're shooting at is dead
			if(myEntityTarget->getHealth() <= 0)
			{
				// then don't shoot at the dead guy
				myHigherState = HIGHERIDLE;
				myLowerState = IDLE;
				break;
			}

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
		// if someone is chasing me
		if(imBeingChased)
		{
			// find out who it is
			tempEntity = whoIsChasingMe();

			// if someone is watching me
		}
		else if(imBeingWatched)
		{
			// find out who it is
			tempEntity = whoIsLookingAtMe();			
		}
		

		if(tempEntity != NULL)
		{
			//=-=-=-==-=-=-=-=
			// Call for help
			// Tell everyone else that someone is chasing me
			board->helpRequested(tempEntity, type);

			// move away from them
			// see if we can find a spot out of line of sight of them...
			//setMoveTargetToVisibleHidingSpotFrom(tempEntity);
			setMoveTargetToHidingSpotFrom(tempEntity);
			if(targetX != -1)
			{
				myHigherState = HIGHERMOVING;
				myLowerState = MOVING;
				break;
			}

			// if we cant, lets just run away from them...
			if((targetX = -1))
			{
				//////////////////////////////////////////////////////////////////
				/////////////////////////////////////////////////////////////////
				////////////////////////////////////////////////////////////////
				////                                                       ////
				////    to see what direction they are from us.....       ////
				////    target x - my x = which direction they are       ////
				////                                                    ////
				///////////////////////////////////////////////////////////
				//////////////////////////////////////////////////////////
				

				///////////////////////////////////////////////////////
				//////////////////////////////////////////////////////
				/////////////////////////////////////////////////////
				////////////////////////////////////////////////////
				///////////////////////////////////////////////////
				///////////
				//////////
				/////////
				////////
				///////
				//////
				/////
				////
				///
				//
				
				double directionX = xPos - tempEntity->getXPos();
				double directionY = yPos - tempEntity->getYPos();

				// now what we want to do is go exactly the other direction from this insidious bastard

				double magicNumber = max(GameVars->dAbs(directionX), GameVars->dAbs(directionY));

				if(magicNumber != 0) {	magicNumber = 1/magicNumber;	}

				// now we need to find as far as we can go on the map that's a viable movement square
				bool done = false;
				double destXt = yPos;
				double destYt = xPos;
		
				destXt += directionX;
				destYt += directionY;

				while(!done)
				{
					// don't let us move off the map, if we're at the edge of the map, set to done
					if( (destXt >= 0) && (destXt < boardWidth) &&
						(destYt >= 0) && (destYt < boardHeight) )
					{
						// if we're not off the board yet, lets kick this shit up...
						destXt += directionX;
						destYt += directionY;
					}
					else
					{
						// shit we went to far, lets back that bitch up
						destXt -= directionX;
						destYt -= directionY;

						if( (destXt == xPos) && (destYt == yPos) )
						{
							// shit we're at our own position, lets just go another way
							if(directionX > directionY)
							{
								// in this case they're more x than y, so lets go y
								// are we more toward the bottom than the top?
								if(yPos > (boardHeight/2) )
								{
									// oh we are
									destYt = 0;
								}
								else
								{
									destYt = boardHeight - 1;
								}
							}
							else
							{
								// are we more to the left or the right?
								if(xPos > (boardWidth /2))
								{
									destXt = 0;
								}
								else
								{
									destXt = boardWidth - 1;
								}
							}
						}

						// now lets set our movement to this location
						setTarget(floor(destXt), floor(destYt));
						done = true;
						myHigherState = HIGHERMOVING;
						myLowerState = MOVING;
					}
				}				
			}
		}

		
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
				doneMoving = true;
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
		myLowerState = IDLE;

		break;

	case ATTACKOPTIMAL:
		//cout << "canIShoot is set to " << canIShoot << endl;

		if(myEntityTarget != NULL)
		{
			// if the guy we're shooting at is dead
			if(myEntityTarget->getHealth() <= 0)
			{
				// then don't shoot at the dead guy
				myHigherState = HIGHERIDLE;
				myLowerState = IDLE;
				break;
			}

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
					if(canIShoot)
					{
						// if so, set state to attacking
						myLowerState = ATTACKING;
						// and break
						break;
					}
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
		
			// and if we are in optimal range
			if(distanceToTarget < (currentWeapon->getRange() / 2))
			{
				// then attack
				myLowerState = ATTACKING;
			}
		}
		break;
	case EXPLORE:
		findExplorationTarget();
		if(targetX != -1)
		{
			myHigherState = HIGHERMOVING;
			myLowerState = MOVING;
		}
		break;
	case SEEKANDDESTROY:
		findExplorationTarget();
		if(targetX != -1)
		{
			myHigherState = ATTACKMOVE;
			myLowerState = MOVING;
		}
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
				doneMoving = true;
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
	//(doneMoving == false) &&

	if( (movementSourceNodeX != -1 && movementSourceNodeY != -1) )
	{
		if(myPath != NULL)
		{
			if(myPath->getCurrentPathNode() != NULL)
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
		}
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
	defaultMovementIncr(.1), doneMoving(true)
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
	canIShoot = true;

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
		// so now we have in an increment of 1 for our larger number and some percentage of this for the other number
		magicNumber = 1/magicNumber;
	}

	incrementX *= magicNumber;
	incrementY *= magicNumber;

	bool done = false;

	//cout << "Source = " << xPos << ", " << yPos << " and target = " << targetXn << ", " << targetYn << endl;
	//cout << "increment X/y = " << incrementX << ", " << incrementY << endl;

	while(!done)
	{
		if( (movingCheckerX >= 0) && (floor(movingCheckerX) < board->getBoardWidth()) &&
			(movingCheckerY >= 0) && (floor(movingCheckerY) < board->getBoardHeight()) )
		{
			// is there an obstacle in the way
			if(board->isThereAnObstacleAt(movingCheckerX, movingCheckerY))
			{
				// yes there was, so we can't see the target
				return false;
			}
		}

		// make sure we're still on the baord, and not at the target yet
		if( ((movingCheckerX > storedTargetX - incrementX) && (movingCheckerX > xPos)) ||
			((movingCheckerX < storedTargetX - incrementX) && (movingCheckerX < xPos)) ||
			((movingCheckerY > storedTargetY - incrementY) && (movingCheckerY > yPos)) ||
			((movingCheckerY < storedTargetY  - incrementY) && (movingCheckerY < yPos)) ||
			((movingCheckerX == storedTargetX - incrementX) && (movingCheckerY == storedTargetY - incrementY))
			)
		{
			// when we're beyond the board, or have arrived at the target square
			done = true;
		}
		//cout << "MovingCheckerX/y " << movingCheckerX << ", " << movingCheckerY << endl;
		movingCheckerX += incrementX;
		movingCheckerY += incrementY;
		//cout << "MovingCheckerX/y " << movingCheckerX << ", " << movingCheckerY << endl;
		//cout << "----------------------------------------------------------------------" << endl;
	}
	// we made it out with no obstructions, we can see it
	return true;
}

bool RVB_Entity::isThereAFriendlyEntityInTheWay(double targetX_n, double targetY_n)
{
	// if they ask if you can see yourself, just say yeah we can
	if((targetX_n == xPos) && (targetY_n == yPos))
	{
		return true;
	}
	// we are going to make two assumptions
	// that our xpos is based on our top left
	// that their target is based off of it's top left
	
	double storedTargetX = targetX_n;// + 0.5;
	double storedTargetY = targetY_n;// + 0.5;

	double movingCheckerX = xPos;// + 0.5;
	double movingCheckerY = yPos;// + 0.5;

	double incrementX = targetX_n - xPos;
	double incrementY = targetY_n - yPos;

	double magicNumber = max(GameVars->dAbs(incrementX), GameVars->dAbs(incrementY));

	if(magicNumber != 0)	// damned black holes
	{
		// so now we have in an increment of 1 for our larger number and some percentage of this for the other number
		magicNumber = 1/magicNumber;
	}

	incrementX *= magicNumber;
	incrementY *= magicNumber;

	bool done = false;

	//cout << "Source = " << xPos << ", " << yPos << " and target = " << targetXn << ", " << targetYn << endl;
	//cout << "increment X/y = " << incrementX << ", " << incrementY << endl;

	movingCheckerX += incrementX;
	movingCheckerY += incrementY;

	while(!done)
	{
		if( (movingCheckerX >= 0) && (floor(movingCheckerX) < board->getBoardWidth()) &&
			(movingCheckerY >= 0) && (floor(movingCheckerY) < board->getBoardHeight()) )
		{
			// check to see if there are any friendlies along the way
			if(board->areThereAnyFriendsAt(movingCheckerX, movingCheckerY, type))
			{
				// hey look, there was
				return true;
			}
		}

		// make sure we're still on the baord, and not at the target yet
		if( ((movingCheckerX > storedTargetX - incrementX) && (movingCheckerX > xPos)) ||
			((movingCheckerX < storedTargetX - incrementX) && (movingCheckerX < xPos)) ||
			((movingCheckerY > storedTargetY - incrementY) && (movingCheckerY > yPos)) ||
			((movingCheckerY < storedTargetY  - incrementY) && (movingCheckerY < yPos)) ||
			((movingCheckerX == storedTargetX - incrementX) && (movingCheckerY == storedTargetY - incrementY))
			)
		{
			// when we're beyond the board, or have arrived at the target square
			done = true;
		}
		//cout << "MovingCheckerX/y " << movingCheckerX << ", " << movingCheckerY << endl;
		movingCheckerX += incrementX;
		movingCheckerY += incrementY;
		//cout << "MovingCheckerX/y " << movingCheckerX << ", " << movingCheckerY << endl;
		//cout << "----------------------------------------------------------------------" << endl;
	}
	// we made it out with no obstructions, so there are no friendlies in the way
	return false;
}

bool RVB_Entity::areThereAnyEntitiesInTheWay(double targetX_n, double targetY_n)
{
	// hmmm this code looks vaguely familiar...
	// if they ask if you can see yourself, just say yeah we can
	if((targetX_n == xPos) && (targetY_n == yPos))
	{
		return true;
	}
	// we are going to make two assumptions
	// that our xpos is based on our top left
	// that their target is based off of it's top left
	
	double storedTargetX = targetX_n;// + 0.5;
	double storedTargetY = targetY_n;// + 0.5;

	double movingCheckerX = xPos;// + 0.5;
	double movingCheckerY = yPos;// + 0.5;

	double incrementX = targetX_n - xPos;
	double incrementY = targetY_n - yPos;

	double magicNumber = max(GameVars->dAbs(incrementX), GameVars->dAbs(incrementY));

	if(magicNumber != 0)	// damned black holes
	{
		// so now we have in an increment of 1 for our larger number and some percentage of this for the other number
		magicNumber = 1/magicNumber;
	}

	incrementX *= magicNumber;
	incrementY *= magicNumber;

	bool done = false;

	//cout << "Source = " << xPos << ", " << yPos << " and target = " << targetXn << ", " << targetYn << endl;
	//cout << "increment X/y = " << incrementX << ", " << incrementY << endl;

	while(!done)
	{
		if( (movingCheckerX >= 0) && (floor(movingCheckerX) < board->getBoardWidth()) &&
			(movingCheckerY >= 0) && (floor(movingCheckerY) < board->getBoardHeight()) )
		{
			// check to see if there are any friendlies along the way
			if(board->areThereAnyFriendsAt(movingCheckerX, movingCheckerY, type))
			{
				// hey look, there was
				return true;
			}
			// now check to see if there is an enemy besides the one you've targeted
			if(board->areThereAnyEnemiesAt(movingCheckerX, movingCheckerY, type))
			{
				// yep we found a different enemy than who we were shooting at
				return true;
			}
		}

		// make sure we're still on the baord, and not at the target yet
		if( ((movingCheckerX > storedTargetX - incrementX) && (movingCheckerX > xPos)) ||
			((movingCheckerX < storedTargetX - incrementX) && (movingCheckerX < xPos)) ||
			((movingCheckerY > storedTargetY - incrementY) && (movingCheckerY > yPos)) ||
			((movingCheckerY < storedTargetY  - incrementY) && (movingCheckerY < yPos)) ||
			((movingCheckerX == storedTargetX - incrementX) && (movingCheckerY == storedTargetY - incrementY))
			)
		{
			// when we're beyond the board, or have arrived at the target square
			done = true;
		}
		//cout << "MovingCheckerX/y " << movingCheckerX << ", " << movingCheckerY << endl;
		movingCheckerX += incrementX;
		movingCheckerY += incrementY;
		//cout << "MovingCheckerX/y " << movingCheckerX << ", " << movingCheckerY << endl;
		//cout << "----------------------------------------------------------------------" << endl;
	}
	// we made it out with no obstructions, so there are no friendlies in the way
	return false;
}

RVB_Entity* RVB_Entity::getEntityTarget()
{
	return myEntityTarget;
}

bool RVB_Entity::isAnyoneLookingAtMe()
{
	int numEntities = (*objectList).size();
	for(int x = 0; x < numEntities; x++)
	{
		if( ((*objectList)[x]->getHealth() > 0) &&			// is it alive?
			((*objectList)[x]->getType() != type) &&		// is it an enemy
			((*objectList)[x]->getEntityTarget() == this))	// is it looking at me?
		{
			return true;
		}					
	}
	return false;
}

RVB_Entity* RVB_Entity::whoIsLookingAtMe()
{
	// determine the number of entities to check against
	int numEntities = (*objectList).size();
	// if we ever make a 10000x10000 board, then this number needs to get bigger
	double closest = 1000000.0;
	double checkClosest  = 0.0;
	RVB_Entity* thisDudeIsChasinMe = NULL;

	for(int x = 0; x < numEntities; x++)
	{
		if( ((*objectList)[x]->getHealth() > 0) &&			// is it alive?
			((*objectList)[x]->getType() != type) &&		// is it an enemy
			((*objectList)[x]->getEntityTarget() == this))	// is it looking at me?
		{
			// see how far away the guy coming at you is
			checkClosest = GameVars->getDistanceToTarget(xPos, yPos, (*objectList)[x]->getXPos(), (*objectList)[x]->getYPos());
			// see if he's the closest to you
			if(checkClosest < closest)
			{
				closest = checkClosest;
				thisDudeIsChasinMe = (*objectList)[x];
			}
		}					
	}

	// return the pointer whether someone is chasin you or not
	return thisDudeIsChasinMe;
}

RVB_Entity* RVB_Entity::whoIsChasingMe()
{
	int numEntities = (*objectList).size();
	// if we ever make a 10000x10000 board, then this number needs to get bigger
	double closest = 1000000.0;
	double checkClosest  = 0.0;
	RVB_Entity* thisDudeIsChasinMe = NULL;

	for(int x = 0; x < numEntities; x++)
	{
		if( ((*objectList)[x]->getHealth() > 0) &&				// is it alive?
			((*objectList)[x]->getType() != type) &&			// is it an enemy
			((*objectList)[x]->getEntityTarget() == this) &&	// is it looking at me?
			((*objectList)[x]->getHigherState() == CHASING))	// is it chasing me?
		{
			// see how far away the guy coming at you is
			checkClosest = GameVars->getDistanceToTarget(xPos, yPos, (*objectList)[x]->getXPos(), (*objectList)[x]->getYPos());
			// see if he's the closest to you
			if(checkClosest < closest)
			{
				closest = checkClosest;
				thisDudeIsChasinMe = (*objectList)[x];
			}
		}					
	}

	// return the pointer whether someone is chasin you or not
	return thisDudeIsChasinMe;
}

bool RVB_Entity::isThisEntityVisibleToMyTeam(RVB_Entity* someEntity)
{
	// we've found an enemy!, lets check its distance
	double enemyX = someEntity->getXPos();
	double enemyY = someEntity->getYPos();
	double distanceToTarget = GameVars->getDistanceToTarget(xPos, yPos, enemyX, enemyY);

	// can i see it?
	if (distanceToTarget < entityVisionRadius)
	{
		// do i still have los to it...
		if(entityCanSeeTargetAt(enemyX, enemyY))
		{
			return true;
		}
	}
	else
	{
		int numEntities = (*objectList).size();
		// can anyone else on my team see it?
		for(int innerX = 0; innerX < numEntities; innerX++)
		{
			if((*objectList)[innerX]->getType() == type)
			{
				double friendlyX = (*objectList)[innerX]->getXPos();
				double friendlyY = (*objectList)[innerX]->getYPos();
				double distanceFriendlyToEnemy = GameVars->getDistanceToTarget(friendlyX, friendlyY, enemyX, enemyY);
				// we've found a friendly, lets see if its within vision radius of the enemy
				if(distanceFriendlyToEnemy <= entityVisionRadius)
				{
					//cout << "found an enemy that's close and visible" << endl;
					// ok now we have the closest enemy so far and its within a friendly entity's vision radius
					// *********************************************
					// RIGHT HERE IS WHERE LINE OF SIGHT GOES, FOR NOW WE IGNORE IT
					// *********************************************
					if((*objectList)[innerX]->entityCanSeeTargetAt(enemyX, enemyY))
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool RVB_Entity::isThisSpotVisibleToMyTeam(double checkX, double checkY)
{
	// we've found an enemy!, lets check its distance
	double distanceToTarget = GameVars->getDistanceToTarget(xPos, yPos, checkX, checkY);

	// can i see it?
	if (distanceToTarget < entityVisionRadius)
	{
		// do i still have los to it...
		if(entityCanSeeTargetAt(checkX, checkY))
		{
			return true;
		}
	}
	else
	{
		int numEntities = (*objectList).size();
		// can anyone else on my team see it?
		for(int innerX = 0; innerX < numEntities; innerX++)
		{
			if((*objectList)[innerX]->getType() == type)
			{
				int friendlyX = (*objectList)[innerX]->getXPos();
				int friendlyY = (*objectList)[innerX]->getYPos();
				double distanceFriendlyToEnemy = GameVars->getDistanceToTarget(friendlyX, friendlyY, checkX, checkY);
				// we've found a friendly, lets see if its within vision radius of the enemy
				if(distanceFriendlyToEnemy <= entityVisionRadius)
				{
					//cout << "found an enemy that's close and visible" << endl;
					// ok now we have the closest enemy so far and its within a friendly entity's vision radius
					// *********************************************
					// RIGHT HERE IS WHERE LINE OF SIGHT GOES, FOR NOW WE IGNORE IT
					// *********************************************
					if((*objectList)[innerX]->entityCanSeeTargetAt(checkX, checkY))
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}


bool RVB_Entity::isAnyoneLookingAtMeThatICanSee()
{
	int numEntities = (*objectList).size();

	for(int x = 0; x < numEntities; x++)
	{
		if( ((*objectList)[x]->getHealth() > 0) &&				// is it alive?
			((*objectList)[x]->getType() != type) &&			// is it an enemy
			((*objectList)[x]->getEntityTarget() == this) &&	// is it looking at me?
			(isThisEntityVisibleToMyTeam((*objectList)[x])) )	// is it visible to my team?
		{
			return true;
		}					
	}
	return false;
}

bool RVB_Entity::isAnyoneChasingMe()
{
	int numEntities = (*objectList).size();
	for(int x = 0; x < numEntities; x++)
	{
		if( ((*objectList)[x]->getHealth() > 0) &&				// is it alive?
			((*objectList)[x]->getType() != type) &&			// is it an enemy
			((*objectList)[x]->getEntityTarget() == this) &&	// is it looking at me?
			((*objectList)[x]->getHigherState() == CHASING))	// is it chasing me?
		{
			return true;
		}					
	}
	return false;
}

higherState RVB_Entity::getHigherState()
{
	return myHigherState;
}
entityState	RVB_Entity::getLowerState()
{
	return myLowerState;
}

bool RVB_Entity::isAnyoneChasingMeThatICanSee()
{
	int numEntities = (*objectList).size();
	for(int x = 0; x < numEntities; x++)
	{
		if( ((*objectList)[x]->getHealth() > 0) &&				// is it alive?
			((*objectList)[x]->getType() != type) &&			// is it an enemy
			((*objectList)[x]->getEntityTarget() == this) &&	// is it looking at me?
			((*objectList)[x]->getHigherState() == CHASING)	&&	// is it chasing me?
			(isThisEntityVisibleToMyTeam((*objectList)[x])) )	// is it visible to my team?
		{
			return true;
		}					
	}
	return false;
}

bool RVB_Entity::isMyTargetChasingMe()
{
	if( (myEntityTarget->getHealth() > 0) &&					// is it alive?
		(myEntityTarget->getType() != type) &&					// is it an enemy
		(myEntityTarget->getEntityTarget() == this) &&			// is it looking at me?
		(myEntityTarget->getHigherState() == CHASING))			// is it chasing me?
	{
		return true;
	}			
	return false;
}

void RVB_Entity::setMoveTargetToHidingSpotFrom(RVB_Entity* someEntity)
{
	// what we're going to do is iterate through the entire board and 
	// see if there's any spots that are out of line of sight from the enemy, if so...
	// we're going to see if its the closest spot, we're doing stupid ai so this is lazy searching

	targetX = -1;
	targetY = -1;

	double boardWidth = board->getBoardWidth();
	double boardHeight = board->getBoardHeight();

	double bestDistanceSoFar = boardWidth * boardHeight; // something stupidly high to start with
	double distanceToTarget = 0;

	for(int x = 0; x < boardWidth; x++)
	{
		for(int y = 0; y < boardHeight; y++)
		{
			if( (!someEntity->entityCanSeeTargetAt(x, y)) &&	// if someEntity cannot see target at x y we have a potential winner
				(!board->isThereAnEntityAt(x, y)) && // and there's nobody standing there
				(!board->isThereAnObstacleAt(x, y)) ) // and there's no obstacle there
			{
				distanceToTarget = GameVars->getDistanceToTarget(xPos, yPos, x, y);
				if(distanceToTarget < bestDistanceSoFar)
				{
					bestDistanceSoFar = distanceToTarget;
					targetX = x;
					targetY = y;
				}
			}
		}
	}
	if(targetX != -1)
	{
		setTarget(targetX, targetY);
	}
}
void RVB_Entity::setMoveTargetToVisibleHidingSpotFrom(RVB_Entity* someEntity)
{
	// this will almost be a copy/paste of above only now it will include the extra check to see if we can see this spot... 
	targetX = -1;
	targetY = -1;

	double boardWidth = board->getBoardWidth();
	double boardHeight = board->getBoardHeight();

	double bestDistanceSoFar = boardWidth * boardHeight; // something stupidly high to start with
	double distanceToTarget = 0;

	for(int x = 0; x < boardWidth; x++)
	{
		for(int y = 0; y < boardHeight; y++)
		{
			if(!someEntity->entityCanSeeTargetAt(x, y))	// if someEntity cannot see target at x y we have a potential winner
			{
				distanceToTarget = GameVars->getDistanceToTarget(xPos, yPos, x, y);
				if((distanceToTarget < bestDistanceSoFar) &&
				   (isThisSpotVisibleToMyTeam(x, y)) &&	
					(!board->isThereAnEntityAt(x, y)) && // and there's nobody standing there
					(!board->isThereAnObstacleAt(x, y)) ) // and there's no obstacle there
				{
					bestDistanceSoFar = distanceToTarget;
					targetX = x;
					targetY = y;
				}
			}
		}
	}

	if(targetX != -1)
	{
		setTarget(targetX, targetY);
	}
}

// find exploration target
void RVB_Entity::findExplorationTarget()
{
	vector<vector<RVB_MapTile*>> *myKnowledgeMap = NULL;

	vector<int> viableX;
	vector<int> viableY;
	// first build a list of tile's that border unexplored tiles
	switch(type)
	{
	case RED:
		myKnowledgeMap = board->getKnowledgeMap(RED);
		break;
	case BLUE:
		myKnowledgeMap = board->getKnowledgeMap(BLUE);
		break;
	default:
		break;
	}

	int boardWidth = board->getBoardWidth();
	int boardHeight = board->getBoardHeight();
	int xToCheck = 0;
	int yToCheck = 0;
	bool viableSpot = false;

	for(int x = 0; x < boardWidth; x++)
	{
		for(int y = 0; y < boardHeight; y++)
		{
			viableSpot = false;
			if( (*myKnowledgeMap)[x][y]->getTileType() != TT_NULL && (*myKnowledgeMap)[x][y]->getTileType() != TT_OBSTACLE)
			{
				// filthy dirty code duplication begins...

				// up
				xToCheck = 0;	yToCheck = -1;
				if( (((x + xToCheck) >= 0) && ((x + xToCheck < boardWidth)) && 	 ((y + yToCheck) >= 0) && ((y + yToCheck < boardHeight))))	{
					if((*myKnowledgeMap)[x + xToCheck][y + yToCheck]->getTileType() == TT_NULL)		{
						viableSpot = true;	}	}

				// down
				xToCheck = 0;	yToCheck = 1;
				if( (((x + xToCheck) >= 0) && ((x + xToCheck < boardWidth)) && 	 ((y + yToCheck) >= 0) && ((y + yToCheck < boardHeight))))	{
					if((*myKnowledgeMap)[x + xToCheck][y + yToCheck]->getTileType() == TT_NULL)		{
						viableSpot = true;	}	}

				// left
				xToCheck = -1;	yToCheck = 0;
				if( (((x + xToCheck) >= 0) && ((x + xToCheck < boardWidth)) && 	 ((y + yToCheck) >= 0) && ((y + yToCheck < boardHeight))))	{
					if((*myKnowledgeMap)[x + xToCheck][y + yToCheck]->getTileType() == TT_NULL)		{
						viableSpot = true;	}	}

				// right
				xToCheck = 1;	yToCheck = 0;
				if( (((x + xToCheck) >= 0) && ((x + xToCheck < boardWidth)) && 	 ((y + yToCheck) >= 0) && ((y + yToCheck < boardHeight))))	{
					if((*myKnowledgeMap)[x + xToCheck][y + yToCheck]->getTileType() == TT_NULL)		{
						viableSpot = true;	}	}

				// upleft
				xToCheck = -1;	yToCheck = -1;
				if( (((x + xToCheck) >= 0) && ((x + xToCheck < boardWidth)) && 	 ((y + yToCheck) >= 0) && ((y + yToCheck < boardHeight))))	{
					if((*myKnowledgeMap)[x + xToCheck][y + yToCheck]->getTileType() == TT_NULL)		{
						viableSpot = true;	}	}

				// upright
				xToCheck = 1;	yToCheck = -1;
				if( (((x + xToCheck) >= 0) && ((x + xToCheck < boardWidth)) && 	 ((y + yToCheck) >= 0) && ((y + yToCheck < boardHeight))))	{
					if((*myKnowledgeMap)[x + xToCheck][y + yToCheck]->getTileType() == TT_NULL)		{
						viableSpot = true;	}	}
				
				// downleft
				xToCheck = -1;	yToCheck = 1;
				if( (((x + xToCheck) >= 0) && ((x + xToCheck < boardWidth)) && 	 ((y + yToCheck) >= 0) && ((y + yToCheck < boardHeight))))	{
					if((*myKnowledgeMap)[x + xToCheck][y + yToCheck]->getTileType() == TT_NULL)		{
						viableSpot = true;	}	}

				// downright
				xToCheck = 1;	yToCheck = 1;
				if( (((x + xToCheck) >= 0) && ((x + xToCheck < boardWidth)) && 	 ((y + yToCheck) >= 0) && ((y + yToCheck < boardHeight))))	{
					if((*myKnowledgeMap)[x + xToCheck][y + yToCheck]->getTileType() == TT_NULL)		{
						viableSpot = true;	}	}
			}
			if(viableSpot)
			{
				viableX.push_back(x);
				viableY.push_back(y);
			}
		}
	}
	// at this point we have our list of viables... lets pick a random one and GTFO
	if(viableX.size() > 0)
	{
		int randMax = viableX.size();
		int randNum = rand() % randMax;
		
		setTarget(viableX[randNum], viableY[randNum]);
	}
}

#ifndef innovation
#error You forgot to define innovation.
#endif

void RVB_Entity::runToNearestFriendlyUnit()
{
	RVB_Entity* nearestFriendly = NULL;
	RVB_Entity* tempEntity = NULL;

	int boardWidth = board->getBoardWidth();
	int boardHeight = board->getBoardHeight();

	double closest = boardWidth * boardHeight;

	for(int x = 0; x < boardWidth; x++)
	{
		for(int y = 0; y < boardWidth; y++)
		{
			if(board->isThereAnEntityAt(x,y))
			{
				tempEntity = board->getSelectableEntityAtGridCoord(x, y);
				if(tempEntity->getType() == type)
				{
					double toCheck = GameVars->getDistanceToTarget(xPos, yPos, tempEntity->getXPos(), tempEntity->getYPos());
					if(toCheck < closest)
					{
						nearestFriendly = tempEntity;
					}
				}
			}
		}
	}

	if(nearestFriendly != NULL)
	{
		setTarget(nearestFriendly->getXPos(), nearestFriendly->getYPos());
		//// don't know if we want this here or not
		myHigherState = CHASING;
	}
}

void RVB_Entity::receivedRequestForHelp(RVB_Entity* entityChasing)
{
	setEnemyTarget(entityChasing);
	setState(CHASING);
}