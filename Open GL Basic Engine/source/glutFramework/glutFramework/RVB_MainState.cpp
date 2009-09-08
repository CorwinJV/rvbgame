#include "RVB_MainState.h"

// while standing up to walk... "Woah!" -- Corwin


//====================
// Init
//
void RVB_MainState::init()
{
	//currentMap = new RVB_Map(mapSize, mapSize);
	currentMap = new RVB_Map(40, 25);
	currentMap->setScale(.50);
	currentMap->setScale(.2);
	panRate = 5;
	clickedX = -1;
	clickedY = -1;

	//// new stuff for testing whee
	currentMap->addEntity(RED, 5, 3, NORTH);
	currentMap->addEntity(RED, 5, 6, NORTH);
	currentMap->addEntity(RED, 5, 9, NORTH);
	currentMap->addEntity(RED, 5, 12, NORTH);
	currentMap->addEntity(RED, 5, 15, NORTH);

	//for(int x = 0; x < 25; x+=3)
	//{
	//	currentMap->addEntity(RED, 1, x, NORTH);
	//	currentMap->addEntity(RED, 2, x, NORTH);
	//	currentMap->addEntity(BLUE, 38, x, NORTH);
	//	currentMap->addEntity(BLUE, 37, x, NORTH);
	//}


	currentMap->addEntity(BLUE, 35, 3, NORTH);
	currentMap->addEntity(BLUE, 35, 6, NORTH);
	currentMap->addEntity(BLUE, 35, 9, NORTH);
	currentMap->addEntity(BLUE, 35, 12, NORTH);
	currentMap->addEntity(BLUE, 35, 15, NORTH);

	selectedEntity = NULL;

	curMouseX = 0;
	curMouseY = 0;

	int boardWidth = currentMap->getBoardWidth();
	int boardHeight = currentMap->getBoardHeight();

	int yInc = 0;
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

	}
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
	currentMap->Update();
	return true;
}

//====================
// Draw
//
bool RVB_MainState::Draw()
{
	currentMap->Draw();

	int tileWidth = currentMap->getTileWidth();
	double scaleFactor = currentMap->getScale();
	int mapOffsetX = currentMap->getOffsetX();
	int mapOffsetY = currentMap->getOffsetY();

	// if we have something selected, draw it selected
	if(selectedEntity != NULL)
	{
		int xPos = selectedEntity->getXPos();
		int yPos = selectedEntity->getYPos();

		GameVars->rvbEntitySelected->drawImage( (int)	(tileWidth*scaleFactor),			 // Width
												(int)(tileWidth*scaleFactor),			 // Height
												((xPos*tileWidth)*scaleFactor)+mapOffsetX,  // X
												((yPos*tileWidth)*scaleFactor)+mapOffsetY); // Y

		// if the entity has a target, lets draw that too
		if(selectedEntity->getTargetX() != -1)
		{
			int targetX = selectedEntity->getTargetX();
			int targetY = selectedEntity->getTargetY();
			GameVars->rvbEntityTarget->drawImage( (int)	(tileWidth*scaleFactor),			 // Width
												(int)(tileWidth*scaleFactor),			 // Height
												((targetX*tileWidth)*scaleFactor)+mapOffsetX,  // X
												((targetY*tileWidth)*scaleFactor)+mapOffsetY); // Y
		}
	}
	return true;
}


//====================
// Mouse & Keyboard 
//
void RVB_MainState::processMouse(int x, int y)
{
	curMouseX = x;
	curMouseY = y;
}

void RVB_MainState::processMouseClick(int button, int state, int x, int y)
{
	if((button == GLUT_RIGHT_BUTTON) && (state == GLUT_DOWN))
	{
		int pie = 10;
	}
	if((button == GLUT_LEFT) && (state == GLUT_DOWN))
	{
		currentMap->getGridCoord(x, y, &clickedX, &clickedY);
		if( (clickedX >= 0) && 
			(clickedX < currentMap->getBoardWidth()) &&
			(clickedY >= 0) &&
			(clickedY < currentMap->getBoardHeight()))
			{
			RVB_Entity* tempEntity = currentMap->getSelectableEntity(x, y);
			
			// if we have nothing selected and we clicked on an entity, lets selected it
			if((selectedEntity == NULL) && (tempEntity != NULL))
			{
				selectedEntity = tempEntity;
			}
			// if we have soemthing selected and we click on a new entity, select the new entity
			else if((selectedEntity != NULL) && (selectedEntity != tempEntity) && (tempEntity != NULL))
			{
				selectedEntity = tempEntity;
			}
			// if we have something selected and we click on it again, deselect the entity
			else if((selectedEntity != NULL) && (selectedEntity == tempEntity))
			{
				selectedEntity = NULL;
			}
			// if we have something selected and we click nowhere, set the target for that piece to the newly clicked tile
			else if((selectedEntity != NULL) && (selectedEntity != tempEntity))
			{
				if(currentMap->getTileTypeAt(clickedX, clickedY) != TT_OBSTACLE)
				{
					selectedEntity->setTarget(clickedX, clickedY);
				}
			}
			// if we have nothing selected and we click on nothingness, lets toggle an obstacle
			else if((selectedEntity == NULL) && (tempEntity == NULL))
			{
				currentMap->toggleObstacle(clickedX, clickedY);
			}
		}
	}
}

void RVB_MainState::keyboardInput(unsigned char c, int x, int y)
{
	curMouseX = x;
	curMouseY = y;
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
		// cycle selected entity
	case '[':
		selectedEntity = currentMap->cycleEntityDown(selectedEntity);
		break;
	case ']':
		selectedEntity = currentMap->cycleEntityUp(selectedEntity);
		break;
		// deselect entity
	case '\\':
		selectedEntity = NULL;
		break;
		// clear all entity targets 
	case '`':
		currentMap->clearEntityTargets();
		break;
		// all selected targets move to mouse position
	case ' ':
		currentMap->setAllEntityTargets(curMouseX, curMouseY);
		break;
	default:
		break;
	}
}