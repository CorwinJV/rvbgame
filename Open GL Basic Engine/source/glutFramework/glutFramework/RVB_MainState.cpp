#include "RVB_MainState.h"

// while standing up to walk... "Woah!" -- Corwin


//====================
// Init
//
void RVB_MainState::init()
{
	GameVars->mySide = RED;
	//currentMap = new RVB_Map(mapSize, mapSize);
	currentMap = new RVB_Map(40, 25);
	currentMap->setScale(.50);
	currentMap->setScale(.2);
	panRate = 5;
	clickedX = -1;
	clickedY = -1;
	selectedEntityList = new vector<RVB_Entity*>();


	//// new stuff for testing whee
	currentMap->addEntity(RED, 5, 3, NORTH);
	currentMap->addEntity(RED, 5, 6, NORTH);
	currentMap->addEntity(RED, 5, 9, NORTH);
	currentMap->addEntity(RED, 5, 12, NORTH);
	currentMap->addEntity(RED, 5, 15, NORTH);

	for(int x = 0; x < 25; x+=3)
	{
		//currentMap->addEntity(RED, 1, x, NORTH);
		//currentMap->addEntity(BLUE, 38, x, NORTH);

		//currentMap->addEntity(RED, 2, x, NORTH);
		//currentMap->addEntity(BLUE, 37, x, NORTH);
	}


	currentMap->addEntity(BLUE, 20, 3, NORTH);
	currentMap->addEntity(BLUE, 34, 6, NORTH);
	currentMap->addEntity(BLUE, 34, 9, NORTH);
	currentMap->addEntity(BLUE, 34, 12, NORTH);
	currentMap->addEntity(BLUE, 34, 15, NORTH);

	curMouseX = 0;
	curMouseY = 0;

	int boardWidth = currentMap->getBoardWidth();
	int boardHeight = currentMap->getBoardHeight();

	/*int yInc = 0;
	for(int y = 2; y < boardHeight; y+= 2)
	{
		int counterLimit = yInc + 2;
		yInc++;
		int counter = 0;

		for(int x = 0; x < boardWidth; x++)
		{		
			counter++;
			if(counter < counterLimit)
			{
				currentMap->toggleObstacle(x, y);
			}
			else
			{
				counter = 0;
			}
		}
	}*/
}

//====================
// Destructor
//
RVB_MainState::~RVB_MainState()
{
	delete currentMap;
	currentMap = NULL;
}

//====================
// Update
//
bool RVB_MainState::Update()
{
	/*if(panX < 1024)
	{	
		panX++;
	}
	else
	{
		panX = 0;
	}
	currentMap->setOffsetX(panX);*/

	// this will display the higher and lower states for the purposed of debugging
	for(int x = 0; x < (int)selectedEntityList->size(); x++)
	{
		cout << "Higher State = " << (*selectedEntityList)[x]->getHigherState() << " | Lower State = " << (*selectedEntityList)[x]->getLowerState() << endl;
	}

	currentMap->Update();
	return true;
}

//====================
// Draw
//
bool RVB_MainState::Draw()
{
	// update drawing data...
	tileWidth = currentMap->getTileWidth();
	scaleFactor = currentMap->getScale();
	mapOffsetX = currentMap->getOffsetX();
	mapOffsetY = currentMap->getOffsetY();


	// draw the gameboard tiles
	currentMap->Draw();
	
	// draw knowledge map for current team
	currentMap->drawKnowledgeMap(tileWidth, scaleFactor, mapOffsetX, mapOffsetY);

	// and bullets
	currentMap->drawBullets();

	// draw reticle around entities being targetted by the currently selected entities
	drawEntityTargets();

	// selection circles around the selected entities
	drawEntitySelectionCircles();

	// now draw all entities
	currentMap->drawEntities(tileWidth, scaleFactor, mapOffsetX, mapOffsetY);

	// update fog/knowledge map for red
	// update fog/knowledge map for blue
	currentMap->updateFog(RED);
	currentMap->updateFog(BLUE);

	// then some fog depending on who's turn it is
	currentMap->drawFog(tileWidth, scaleFactor, mapOffsetX, mapOffsetY);


	// if we have something selected, lets draw the move targets (if it has one)
	drawMoveTargets();


	////////////////// below here is UI stuff that gets drawn on top of the fog, anything that should be fogged up is before here..


	drawMultiSelectionBox();

	// last but not least, draw the header
	oglTexture2D* tempImg;

	switch(GameVars->mySide)
	{
	case RED:
		tempImg = GameVars->redActive;
		break;
	case BLUE:
		tempImg = GameVars->blueActive;
		break;
	case GOD:
		tempImg = GameVars->godActive;
		break;
	}

	tempImg->drawImage(1024, 64, 0, 0);

	

	return true;
}

void RVB_MainState::drawMultiSelectionBox()
{
	//===============================
	// Multi-Select Selection box 
	//
	if(isLeftMouseDown)
	{
		int top = -1;
		int left = -1;
		int bottom = -1;
		int right = -1;
		if(clickedX < curMouseX)
		{
			left = clickedX;
			right = curMouseX;
		}
		else
		{
			left = curMouseX;
			right = clickedX;
		}
		if(clickedY < curMouseY)
		{
			top = clickedY;
			bottom = curMouseY;
		}
		else
		{
			top = curMouseY;
			bottom = clickedY;
		}
		int selectionBoxPixelW = 2;
		// top border 
		GameVars->rvbSelectionPix->drawImage((right - left), selectionBoxPixelW, left, top);
		// right border
		GameVars->rvbSelectionPix->drawImage(selectionBoxPixelW, (bottom - top), right, top);
		// bottom border
		GameVars->rvbSelectionPix->drawImage((right - left), selectionBoxPixelW, left, bottom);
		// left border
		GameVars->rvbSelectionPix->drawImage(selectionBoxPixelW, (bottom - top), left, top);
	}
}

void RVB_MainState::drawMoveTargets()
{
	vector<RVB_Entity*>::iterator itr = selectedEntityList->begin();
	for(;itr != selectedEntityList->end(); itr++)
	{
		if((*itr) != NULL)
		{
			double xPos = (*itr)->getXPos();
			double yPos = (*itr)->getYPos();

			if((*itr)->getTargetX() != -1)
			{
				double targetX = (*itr)->getTargetX();
				double targetY = (*itr)->getTargetY();
				GameVars->rvbEntityTarget->drawImage( (double)	(tileWidth*scaleFactor),			 // Width
													(double)(tileWidth*scaleFactor),			 // Height
													((targetX*tileWidth)*scaleFactor)+mapOffsetX,  // X
													((targetY*tileWidth)*scaleFactor)+mapOffsetY); // Y
			}
		}
	}
}

void RVB_MainState::drawEntitySelectionCircles()
{
	// if we have something selected, draw it selected
	vector<RVB_Entity*>::iterator itr = selectedEntityList->begin();
	for(;itr != selectedEntityList->end(); itr++)
	{
		if((*itr) != NULL)
		{
			double xPos = (*itr)->getXPos();
			double yPos = (*itr)->getYPos();

			GameVars->rvbEntitySelected->drawImage( (double)	(tileWidth*scaleFactor),			 // Width
													(double)(tileWidth*scaleFactor),			 // Height
													((xPos*tileWidth)*scaleFactor)+mapOffsetX,  // X
													((yPos*tileWidth)*scaleFactor)+mapOffsetY); // Y
		}
	}
}

void RVB_MainState::drawEntityTargets()
{
	vector<RVB_Entity*>::iterator itr = selectedEntityList->begin();
	for(;itr != selectedEntityList->end(); itr++)
	{
		if((*itr) != NULL)
		{
			(*itr)->drawEntityTarget(tileWidth, scaleFactor, mapOffsetX, mapOffsetY);
		}
	}
}

//======================
// Toggle the selection
// if a given entity
//
void RVB_MainState::toggleSelectedEntity(RVB_Entity* entityToToggle)
{
	bool entityFound = false;
	vector<RVB_Entity*>::reverse_iterator itr = selectedEntityList->rbegin();
	for(; itr != selectedEntityList->rend(); itr++)
	{
		if((*itr) == entityToToggle)
		{
			selectedEntityList->erase((++itr).base());
			entityFound = true;
			break;
		}
	}
	if(entityFound == false)
	{
		selectedEntityList->push_back(entityToToggle);
	}
}

void RVB_MainState::setSelectedEntitiesState(higherState newState)
{
	vector<RVB_Entity*>::iterator itr = selectedEntityList->begin();
	for(; itr != selectedEntityList->end(); itr++)
	{
		(*itr)->setState(newState);
	}
}


void RVB_MainState::setSelectedEntitiesTargets(int tX, int tY)
{
	vector<RVB_Entity*>::iterator itr = selectedEntityList->begin();
	for(; itr != selectedEntityList->end(); itr++)
	{
		(*itr)->setTarget(tX, tY);
		(*itr)->setState(HIGHERMOVING);
	}
}

void RVB_MainState::setSelectedEntitiesEnemyTargets(int tX, int tY)
{
	vector<RVB_Entity*>::iterator itr = selectedEntityList->begin();
	for(; itr != selectedEntityList->end(); itr++)
	{
		(*itr)->setEnemyTarget(currentMap->getSelectableEntityAtGridCoord(tX, tY));
	}
}

void RVB_MainState::hurtSelectedEntities()
{
	vector<RVB_Entity*>::iterator itr = selectedEntityList->begin();
	for(; itr != selectedEntityList->end(); itr++)
	{
		(*itr)->applyDamage(5);
	}
}

void RVB_MainState::healSelectedEntities()
{
	vector<RVB_Entity*>::iterator itr = selectedEntityList->begin();
	for(; itr != selectedEntityList->end(); itr++)
	{
		(*itr)->setHealth(5);
	}
}

//======================
// Mouse & Keyboard 
//
void RVB_MainState::processMouse(int x, int y)
{
	curMouseX = x;
	curMouseY = y;
}

void RVB_MainState::processMouseClick(int button, int state, int x, int y)
{
	int modifiers = glutGetModifiers();

	if((button == GLUT_RIGHT_BUTTON) && (state == GLUT_DOWN))
	{
		int gridPosX = -1;
		int gridPosY = -1;
		currentMap->getGridCoord(x, y, &gridPosX, &gridPosY);

		if( (gridPosX >=0) && (gridPosX < currentMap->getBoardWidth()) &&
			(gridPosY >=0) && (gridPosY < currentMap->getBoardHeight()) )
		{
			if(modifiers & GLUT_ACTIVE_ALT)
			{
				// DEBUG
				// Toggle obstacles on that tile
				if((gridPosX != -1) && (gridPosY != -1))
				{
					currentMap->toggleObstacle(gridPosX, gridPosY);
				}
			}
			else if(modifiers & GLUT_ACTIVE_CTRL)
			{
				// Fire on selected target
				// attack move for now...
				setSelectedEntitiesTargets(gridPosX, gridPosY);
				setSelectedEntitiesState(ATTACKMOVE);			
			}
			else // No modifiers
			{
				// If on empty tile || If on obstacle
					// Pathfind to empty tile ||  Pathfinds to closest non-obstacle
				if((gridPosX != -1) && (gridPosY != -1))
				{
					// if there are no entities or obstacles... then go ahead and set targets
					if(currentMap->isTileValidMove(gridPosX, gridPosY))
					{
						setSelectedEntitiesTargets(gridPosX, gridPosY);
						// and set them to moving
						setSelectedEntitiesState(HIGHERMOVING);
					}
					// now check to see if there's any enemies at the location
					if(selectedEntityList->size() > 0)
					{
						if( (currentMap->areThereAnyEnemiesAt(gridPosX, gridPosY, (*selectedEntityList)[0]->getType())) ||
							(currentMap->areThereAnyFriendsAt(gridPosX, gridPosY, (*selectedEntityList)[0]->getType())) )
						{
							if((currentMap->getSelectableEntityAtGridCoord(gridPosX, gridPosY)->getHealth() > 0))
							{
								setSelectedEntitiesEnemyTargets(gridPosX, gridPosY);
								setSelectedEntitiesState(CHASING);
							}
						}
					}
				}
			}
			//=================
			// TODO!!!
			// If on other color entity
				// Set selected entity to attack other color entity

			// On same color entity 
				// Follow / guard target
		}			
	}
	if((button == GLUT_LEFT) && (state == GLUT_DOWN))
	{
		isLeftMouseDown = true;
		clickedX = x;
		clickedY = y;
		curMouseX = x;
		curMouseY = y;
	}
	if((button == GLUT_LEFT) && (state == GLUT_UP))
	{
		if(isLeftMouseDown)
		{
			// If you click on a non-entity
				// Clear selected entities
			if((modifiers & GLUT_ACTIVE_SHIFT) ||
				(modifiers & GLUT_ACTIVE_CTRL))
			{
				// If shift is held down
					// we're appending to the selectedEntityList
			}
			else
			{
				selectedEntityList->clear();
			}

			//==============================
			// Get the information on which 
			// tiles were selected
				// Get the grid coords for the clicked positions
			int gridCoordSet1X = -1;
			int gridCoordSet1Y = -1;
			int gridCoordSet2X = -1;
			int gridCoordSet2Y = -1;
			currentMap->getGridCoord(clickedX, clickedY, &gridCoordSet1X, &gridCoordSet1Y);
			currentMap->getGridCoord(x, y, &gridCoordSet2X, &gridCoordSet2Y);
				
				// Make a proper rectangle with a top-left value & a bottom-right value
			int left = 0;
			int right = 0;
			int top = 0;
			int bottom = 0;
				// The lowest X of our 2 XY coords becomes "left"
				// the highest X of our 2 XY coords becomes "right"

			// minor tweak yay ******
			left = min(gridCoordSet1X, gridCoordSet2X);
			right = max(gridCoordSet1X, gridCoordSet2X);

				// The lowest Y of our 2 XY coords becomes "top"
				// the highest Y of our 2 XY coords becomes "bottom"
			top = min(gridCoordSet1Y, gridCoordSet2Y);
			bottom = max(gridCoordSet1Y, gridCoordSet2Y);


			//==============================
			// Get the information on which 
			// tiles were selected
				// If you click on an entity
			for(int i = left; i <= right; i++)
			{
				for(int j = top; j <= bottom; j++)
				{
					RVB_Entity* tempEntity = NULL;
					tempEntity = currentMap->getSelectableEntityAtGridCoord(i, j);
					
					if(tempEntity != NULL)
					{
						if((tempEntity->getType() == GameVars->mySide) || GameVars->mySide == GOD)
						{
							// If you click on an entity
								// And control is held down
							if(modifiers & GLUT_ACTIVE_CTRL)
							{
								// Toggle selection of the entity you clicked on
								if(tempEntity != NULL)
								{
									toggleSelectedEntity(tempEntity);
								}
							}
							else
							{
								// select it
								if(tempEntity != NULL)
								{
									//If the entity is not already selected
									bool entityIsSelectedAlready = false;
									vector<RVB_Entity*>::iterator itr = selectedEntityList->begin();
									for(; itr != selectedEntityList->end(); itr++)
									{
										if((*itr) == tempEntity)
										{
											entityIsSelectedAlready = true;
										}
									}
									if(entityIsSelectedAlready == false)
									{
										selectedEntityList->push_back(tempEntity);
									}
								}
							}
						}
					}
				}
			}
		}
		isLeftMouseDown = false;
	}
}

void RVB_MainState::keyboardInput(unsigned char c, int x, int y)
{
	curMouseX = x;
	curMouseY = y;

	int size = 0;

	RVB_Entity* tempEntity;

	// manual shooting variables
	double targetX = 0;
	double targetY = 0;
	// create the temp bullet
	//RVB_Bullet* tempBullet;
	
	//================================
	// Regular Keys
	//
	switch(c)
	{
		//panning keys
	case 'w':
		// TODO, write pan function
		// implement functionality of the following line
		currentMap->pan(UP);
		//currentMap->setOffsetY(currentMap->getOffsetY() + (panRate / currentMap->getScale()));
		break;
	case 'a':
		currentMap->pan(LEFT);
		break;
	case 's':
		currentMap->pan(DOWN);
		break;
	case 'd':
		currentMap->pan(RIGHT);
		break;
		// zoom
	case '+':
		currentMap->zoomIn();
		break;
	case '-':
		currentMap->zoomOut();
		break;
	//	// cycle selected entity
	//case '[':
	//	selectedEntity = currentMap->cycleEntityDown(selectedEntity);
	//	break;
	//case ']':
	//	selectedEntity = currentMap->cycleEntityUp(selectedEntity);
	//	break;
		// deselect entity
	case '\\':
		selectedEntityList->clear();
		break;
		// clear all entity targets 
	case '`':
		currentMap->clearEntityTargets();
		break;
		// all selected targets move to mouse position
	case ' ':
		currentMap->setAllEntityTargets(curMouseX, curMouseY);
		break;
	case 'o':
		setSelectedEntitiesState(EXPLORE);
		break;
	case 'p':
		setSelectedEntitiesState(SEEKANDDESTROY);
		break;
	case 'i':
		setSelectedEntitiesState(EVADING);
		break;
	case 'r':
	case 'R':
		selectedEntityList->clear();
		if(GameVars->mySide == RED)
		{
			GameVars->mySide = BLUE;
		}
		else if (GameVars->mySide == BLUE)
		{
			GameVars->mySide = GOD;
		}
		else if (GameVars->mySide == GOD)
		{
			GameVars->mySide = RED;
		}
	case 'g':
		hurtSelectedEntities();
		break;
	case 'h':
		healSelectedEntities();
		break;
	case '1':
		// set to the size of the selected Entity list
		size = selectedEntityList->size();
		for(int x = 0; x < size; x++)
		{
			// set any entity that is selected to attack optimal state
			currentMap->setToAttackOptimal((*selectedEntityList)[x]);
		}
		break;
	case '2':
		// attack this guy
		size = selectedEntityList->size();
		tempEntity = NULL;
		tempEntity = currentMap->getSelectableEntity(curMouseX, curMouseY);

		if(tempEntity != NULL)
		{
			for(int x = 0; x < size; x++)
			{
				// set any entity that is selected to attack optimal state
				(*selectedEntityList)[x]->setEnemyTarget(tempEntity);
			}
		}
		break;
	case '3':
		size = selectedEntityList->size();
		for(int x = 0; x < size; x++)
		{
			// set any entity that is selected to attack optimal state
			(*selectedEntityList)[x]->setState(HIGHERATTACKING);
		}
		break;

	case 'u':
		// manual shoot up

		// set to the size of the selected Entity list
		//size = selectedEntityList->size();
		//for(int x = 0; x < size; x++)
		//{
		//	// set target's position to where you are
		//	targetX = (*selectedEntityList)[x]->getXPos();
		//	targetY = (*selectedEntityList)[x]->getYPos();
		//	// since we're shooting upward, subtract one from the Y coordinate
		//	targetY -= 1;

		//	// have any selected entity shoot upwards
		//	tempBullet = (*selectedEntityList)[x]->currentWeapon->shotFired((*selectedEntityList)[x]->getXPos(), 
		//													   (*selectedEntityList)[x]->getYPos(), 
		//													    targetX, targetY, 1, NULL); 
		//	// add the bullet to the map
		//	currentMap->makeBullet(tempBullet);
		//}
		break;
	case 'm':
		// manual shoot down
		
		// set to the size of the selected Entity list
		//size = selectedEntityList->size();
		//for(int x = 0; x < size; x++)
		//{
		//	// set target's position to where you are
		//	targetX = (*selectedEntityList)[x]->getXPos();
		//	targetY = (*selectedEntityList)[x]->getYPos();
		//	// since we're shooting down, add one from the Y coordinate
		//	targetY += 2;

		//	// have any selected entity shoot upwards
		//	tempBullet = (*selectedEntityList)[x]->currentWeapon->shotFired((*selectedEntityList)[x]->getXPos(), 
		//													   (*selectedEntityList)[x]->getYPos() + 1.5, 
		//													    targetX, targetY, 1, NULL); 
		//	// add the bullet to the map
		//	currentMap->makeBullet(tempBullet);
		//}
		
		break;
	case 'j':
		// manual shoot left
		
		//// set to the size of the selected Entity list
		//size = selectedEntityList->size();
		//for(int x = 0; x < size; x++)
		//{
		//	// set target's position to where you are
		//	targetX = (*selectedEntityList)[x]->getXPos();
		//	targetY = (*selectedEntityList)[x]->getYPos();
		//	// since we're shooting left, subtract one from the X coordinate
		//	targetX -= 1;

		//	// have any selected entity shoot upwards
		//	tempBullet = (*selectedEntityList)[x]->currentWeapon->shotFired((*selectedEntityList)[x]->getXPos(), 
		//													   (*selectedEntityList)[x]->getYPos(), 
		//													    targetX, targetY, 1, NULL); 
		//	// add the bullet to the map
		//	currentMap->makeBullet(tempBullet);
		//}
		
		break;
	case 'k':
		// manual shoot right
		
		// set to the size of the selected Entity list
		//size = selectedEntityList->size();
		//for(int x = 0; x < size; x++)
		//{
		//	// set target's position to where you are
		//	targetX = (*selectedEntityList)[x]->getXPos();
		//	targetY = (*selectedEntityList)[x]->getYPos();
		//	// since we're shooting right, add one from the X coordinate
		//	targetX += 2;

		//	// have any selected entity shoot upwards
		//	tempBullet = (*selectedEntityList)[x]->currentWeapon->shotFired((*selectedEntityList)[x]->getXPos() + 1.5, 
		//													   (*selectedEntityList)[x]->getYPos(), 
		//													    targetX, targetY, 1, NULL); 
		//	// add the bullet to the map
		//	currentMap->makeBullet(tempBullet);
		//}
		
		break;
	default:
		break;
	}
}
