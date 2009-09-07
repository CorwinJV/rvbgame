#include "RVB_Map.h"
#include "oglGameVars.h"

//====================
// Constructors
//
RVB_Map::RVB_Map()
{
	RVB_Map(20, 20);
	cout << "You shouldn't be using this " << endl;
}

RVB_Map::RVB_Map(int mapSizeX, int mapSizeY)
:mRotation(0.0)
{
	mapWidth  = mapSizeX;
	mapHeight = mapSizeY;
	// Make a map of the provided dimensions
	
	// Loop from 0 to x
		// Loop from 0 to y
			// Build the Y (column)
		// Push to initial vector

	//=================================
	// Set up our 2 dim vector with 
	// null pointers 
	mBoard.resize(mapSizeX);
	for(int x = 0; x < mapSizeX; x++)
	{
		mBoard[x].resize(mapSizeY);
	}

	//=================================
	// Allocate memory for null pointers 
	// & initialize, it's just good practice.
	// Ya fags.
	for(int i = 0; i < mapSizeX; i++)
	{
		for(int j = 0; j < mapSizeY; j++)
		{
			mBoard[i][j] = new RVB_MapTile(TT_DEFAULT);
		}
	}

	//=================================
	// Reg. Data Member Initialization
	scaleFactor = 1.0;
	panRate = 10;
	mapOffsetX = 0;
	mapOffsetY = 0;
}

//====================
// Destructor
//
RVB_Map::~RVB_Map()
{

}

//====================
// Update
//
void RVB_Map::Update()
{
	// Update all of the entities on our board
	int tempLimit = objectList.size();
	for(int xi = 0; xi < tempLimit; xi++)
	{
		objectList[xi]->Update();
	}	
}

//====================
// Draw
//
void RVB_Map::Draw()
{
	// Iterate through map tiles
		// Switch to determine which picture to draw (enum)
		// draw at position offset by height / width
	
	for(int x = 0; x < (int)mBoard.size(); x++)
	{
		for(int y = 0; y < (int)mBoard[x].size(); y++)
		{
			oglTexture2D* imgToDraw = NULL;
			switch(mBoard[x][y]->getTileType())
			{

			case TT_DEFAULT:
				{
					imgToDraw = GameVars->rvbTile;
					break;
				}
			case TT_OBSTACLE:
				{
					imgToDraw = GameVars->rvbObstacle;
					break;
				}
			case TT_NULL:
				{
					imgToDraw = GameVars->rvbNullTile;
					break;
				}
			default:
				{
					break;
				}
			}

			if(imgToDraw != NULL)
			{
				// stupid corwin
				/*if((x == 1) && (y == 1))
				{
					mRotation++;
					imgToDraw->rotationInDeg = mRotation;
				}*/
				imgToDraw->drawImage((int)(tileWidth*scaleFactor),			 // Width
									(int)(tileWidth*scaleFactor),			 // Height
									((x*tileWidth)*scaleFactor)+mapOffsetX,  // X
									((y*tileWidth)*scaleFactor)+mapOffsetY); // Y
				
				imgToDraw->rotationInDeg = 0.0;
			}
		}
	}

	// now lets tell all objects in the board to draw themselves!
	int tempLimit = objectList.size();
	for(int xi = 0; xi < tempLimit; xi++)
	{
		objectList[xi]->Draw(tileWidth, scaleFactor, mapOffsetX, mapOffsetY);
	}	

	drawText();
	
}

double RVB_Map::getScale()
{
	return scaleFactor;
}

int RVB_Map::getOffsetX()
{
	return mapOffsetX;
}

int RVB_Map::getOffsetY()
{
	return mapOffsetY;
}

void RVB_Map::setScale(double newS)
{
	scaleFactor = newS;
}

void RVB_Map::setOffsetX(int newX)
{
	mapOffsetX = newX;
}

void RVB_Map::setOffsetY(int newY)
{
	mapOffsetY = newY;
}

void RVB_Map::setOffset(int newX, int newY)
{
	mapOffsetX = newX;
	mapOffsetY = newY;
}

int RVB_Map::getBoardHeight()
{
	return mapHeight;
}

int RVB_Map::getBoardWidth()
{
	return mapWidth;
}

int RVB_Map::getTileWidth()
{
	return tileWidth;
}

void RVB_Map::getGridCoord(int x, int y, int *returnGridX, int *returnGridY)
{
	(*returnGridX) = (int)(((x - mapOffsetX)/scaleFactor) / tileWidth);
	(*returnGridY) = (int)(((y - mapOffsetY)/scaleFactor) / tileWidth);
}

void RVB_Map::drawText()
{
	
}

RVB_Entity *RVB_Map::getSelectableEntity(int mouseX, int mouseY)
{
	int curX = mouseX;
	int curY = mouseY;
	int mapPosX = -1;
	int mapPosY = -1;
	getGridCoord(curX, curY, &mapPosX, &mapPosY);

	RVB_Entity* entityToReturn = NULL;
	if((mapPosX != -1) && 
	   (mapPosY != -1))
	{
		int size = objectList.size();
		for(int i = 0; i < size; i++)
		{
			if((objectList[i]->getXPos() == mapPosX) &&
			   (objectList[i]->getYPos() == mapPosY))
			{
				entityToReturn = objectList[i];
			}
		}
	}

	return entityToReturn;
}

void RVB_Map::clearEntityTargets()
{
	int size = objectList.size();
	for(int i = 0; i < size; i++)
	{
		objectList[i]->clearTarget();
	}
}

void RVB_Map::setAllEntityTargets(int mouseX, int mouseY)
{
	// Get the grid coord of the mouse coords.
	int rX, rY;
	getGridCoord(mouseX, mouseY, &rX, &rY); 

	// Iterate and set target
	int size = objectList.size();
	for(int i = 0; i < size; i++)
	{
		objectList[i]->setTarget(rX, rY);
	}
}

void RVB_Map::addEntity(entityType newType, int xPos, int yPos, entityDirection newDirection)
{
	RVB_Entity* tempEntity = new RVB_Entity(newType, xPos, yPos, newDirection, this, &objectList);
	objectList.push_back(tempEntity);
}

void RVB_Map::toggleObstacle(int xPos, int yPos)
{
	if(mBoard[xPos][yPos]->getTileType() == TT_DEFAULT)
	{
		mBoard[xPos][yPos]->setTileType(TT_OBSTACLE);
	}
	else
	{
		mBoard[xPos][yPos]->setTileType(TT_DEFAULT);
	}
}

TileType RVB_Map::getTileTypeAt(int xPos, int yPos)
{
	return mBoard[xPos][yPos]->getTileType();
}

bool RVB_Map::isTileValidMove(int xPos, int yPos)
{
	switch(getTileTypeAt(xPos, yPos))
	{
		// For any other fail conditions
		// fall through to the obstacle case.
	case TT_OBSTACLE:
		{
			return false;
		}
	}

	for(unsigned int i = 0; i < objectList.size(); i++)
	{
		if((objectList[i]->getXPos() == xPos) &&
			(objectList[i]->getYPos() == yPos))
		{
			return false;
		}
	}

	// If we haven't hit any fail conditions...
		// Then we can move here
	return true;
}

void RVB_Map::pan(mapDirection direction)
{
	double panAmmount = 0.0;
	double magicNum1 = 0.4;
	double magicNum2 = 22.5;

	panAmmount = (-1/(scaleFactor*magicNum1)) + magicNum2;
	switch(direction)
	{
	case UP:
		setOffsetY(getOffsetY() + panAmmount);
		break;
	case DOWN:
		setOffsetY(getOffsetY() - panAmmount);
		break;
	case LEFT:
		setOffsetX(getOffsetX() + panAmmount);
		break;
	case RIGHT:
		setOffsetX(getOffsetX() - panAmmount);
		break;
	default:
		break;
	}
}

void RVB_Map::zoomIn()
{
	// zoom in 5%
	scaleFactor *= 1.05;
	// make sure we aren't zoomed in too far
	if(scaleFactor > 1)
	{
		scaleFactor = 1;	
	}
	cout << scaleFactor << endl;
}

void RVB_Map::zoomOut()
{
	// zoom out 5%
	scaleFactor /= 1.05;
	// make sure we aren't zoomed out too far
	if(scaleFactor < .2)
	{
		scaleFactor = 0.2;
	}
	cout << scaleFactor << endl;
}

RVB_Entity* RVB_Map::cycleEntityDown(RVB_Entity* selectedEntity)
{
	// Get the index of the selected entity
	int selectedEntitysIndex = -1;
	for(unsigned int i = 0; i < objectList.size(); i++)
	{
		if(objectList[i] == selectedEntity)
		{
			selectedEntitysIndex = i;
		}
	}
	if(selectedEntitysIndex == -1)
	{
		return NULL;
	}

	// If the selectedEntity is the front of our vector
	if(selectedEntitysIndex == 0)
	{
		// return the back of our vector
		return objectList[objectList.size() - 1];
	}
	else
	{
		// return the entity at selectedEntityIndex-1
		return objectList[selectedEntitysIndex - 1];
	}
	return NULL;
}

RVB_Entity* RVB_Map::cycleEntityUp(RVB_Entity* selectedEntity)
{
	// Get the index of the selected entity
	int selectedEntitysIndex = -1;
	for(unsigned int i = 0; i < objectList.size(); i++)
	{
		if(objectList[i] == selectedEntity)
		{
			selectedEntitysIndex = i;
		}
	}
	if(selectedEntitysIndex == -1)
	{
		return NULL;
	}

	// If the selectedEntity is the back of our vector
	if(selectedEntitysIndex == (objectList.size() - 1))
	{
		// return the front of the vector
		return objectList[0];
	}
	else
	{
		// return the entity at selectedEntityIndex+1
		return objectList[selectedEntitysIndex+1];
	}
	return NULL;
}

