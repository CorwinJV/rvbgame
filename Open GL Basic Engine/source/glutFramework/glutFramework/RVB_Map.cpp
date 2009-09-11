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
	cameraCenterX = mapWidth / 2;
	cameraCenterY = mapHeight / 2;
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
	recalcBoard();
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

	// now its time for fog of war
	vector<vector<double>> myFog;

	int uberFactor = 4;

	int mapWidth = mBoard.size();
	int mapHeight = mBoard[0].size();

	// first lets fog this bitch up
	myFog.resize(mapWidth * uberFactor);
	for(int x = 0; x < mapWidth * uberFactor; x++)
	{
		myFog[x].resize(mapHeight * uberFactor);
		for(int y = 0; y < mapHeight * uberFactor; y++)
		{
			if(GameVars->mySide != GOD)
			{
				myFog[x][y] = 1.0;
			}
			else
			{
				myFog[x][y] = 0.0;
			}
		}
	}
	
	// now lets go and unfog some areas

	if(GameVars->mySide != GOD)
	{
		double visionCalc = entityVisionRadius + 1;
		// iterate through entity list
		for(int entX = 0; entX < (int)objectList.size(); entX++)
		{
			entityType entityAt = objectList[entX]->getType();
			
			if(entityAt == GameVars->mySide)
			{
				int entityX = objectList[entX]->getXPos();
				int entityY = objectList[entX]->getYPos();

				for(int x = (entityX * uberFactor) - (visionCalc * uberFactor); x < (entityX * uberFactor) + ((visionCalc + 1) * uberFactor); x++)
				{ 
					for(int y = (entityY * uberFactor) - (visionCalc * uberFactor); y < (entityY * uberFactor) + ((visionCalc + 1) * uberFactor); y++)
					{
						if((x >= 0) && (x < mapWidth * uberFactor) && 
						   (y >= 0) && (y < mapHeight * uberFactor))
						{
							double distanceToTarget = GameVars->getDistanceToTarget(x, y, (entityX + 0.25) * uberFactor, (entityY + 0.25) * uberFactor);
							if(distanceToTarget <= visionCalc * uberFactor)
							{
								myFog[x][y] -= (1.0/uberFactor * (((visionCalc * uberFactor + 1) - distanceToTarget)) / visionCalc * uberFactor);
								if(myFog[x][y] < 0)
								{
									myFog[x][y] = 0;
								}
							}
						}
					}
				}
			}
		}
	}

	// finally we draws us some bullets yo!
	int tempSize = bulletList.size();

	for (int x = 0; x < tempSize; x++)
	{
		bulletList[x]->draw(scaleFactor);
	}


	// now lets draw the fog!
	// set fog dimensions

	for(int x = 0; x < mapWidth * uberFactor; x++)
	{
		for(int y = 0; y < mapHeight * uberFactor; y++)
		{

			// now draw it
			GameVars->fog->drawImageFaded(myFog[x][y],
											(tileWidth*scaleFactor / uberFactor),			 // Width
											(tileWidth*scaleFactor / uberFactor),			 // Height
											((x*tileWidth)*scaleFactor / uberFactor )+mapOffsetX,  // X
											((y*tileWidth)*scaleFactor / uberFactor )+mapOffsetY); // Y
		}
	}	


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

	return getSelectableEntityAtGridCoord(mapPosX, mapPosY);
}

RVB_Entity *RVB_Map::getSelectableEntityAtGridCoord(int gridX, int gridY)
{
	RVB_Entity* entityToReturn = NULL;

	if((gridX >= 0) && (gridX < mapWidth) &&
		(gridY >= 0) && (gridY < mapHeight))
	{
		if(objectList.size() > 0)
		{
			int size = objectList.size();
			for(int i = 0; i < size; i++)
			{
				if((objectList[i]->getXPos() == gridX) &&
					(objectList[i]->getYPos() == gridY))
				{
					entityToReturn = objectList[i];
				}
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
	double panAmount = 0.0;
	double magicNum1 = 0.4;
	double magicNum2 = 22.5;

	panAmount = (((-1/(scaleFactor*magicNum1)) + magicNum2) / 5);
	switch(direction)
	{
	case UP:
		//mapOffsetY += panAmount;
		cameraCenterY -= panAmount;
		break;
	case DOWN:
		//mapOffsetY -= panAmount;
		cameraCenterY += panAmount;
		break;
	case LEFT:
		//mapOffsetX += panAmount;
		cameraCenterX -= panAmount;
		break;
	case RIGHT:
		//mapOffsetX -= panAmount;
		cameraCenterX += panAmount;
		break;
	default:
		break;
	}

	cout << "Camera X is " << cameraCenterX << " camera Y is " << cameraCenterY << endl;

	if(cameraCenterX < 0)
	{
		cameraCenterX = 0;
	}

	if(cameraCenterX > mapWidth)
	{
		cameraCenterX = mapWidth;
	}

	if(cameraCenterY < 0)
	{
		cameraCenterY = 0;
	}

	if(cameraCenterY > mapHeight)
	{
		cameraCenterY = mapHeight;
	}
}

void RVB_Map::center()
{
}

void RVB_Map::recalcBoard()
{
	// determine the current dimensions of the tiles based of their original dimensions and its scale
	currentTileWidth = tileWidth * scaleFactor;
	currentTileHeight = tileHeight * scaleFactor;

	// determine the new offsets
	mapOffsetX = ((screenWidth / 2) - (currentTileWidth * cameraCenterX));
	mapOffsetY = ((screenHeight / 2) - (currentTileHeight * cameraCenterY));
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

bool RVB_Map::isThereAnEntityAt(int xPos, int yPos)
{
	int size = objectList.size();
	for(int i = 0; i < size; i++)
	{
		if((objectList[i]->getXPos() == xPos) &&
		   (objectList[i]->getYPos() == yPos))
		{
			return true;
		}
	}
	return false;
}

bool RVB_Map::areThereAnyEnemiesAt(int xPos, int yPos, entityType whatColorIAm)
{
	int size = objectList.size();
	for(int x = 0; x < size; x++)
	{
		if( (objectList[x]->getXPos() == xPos) &&
			(objectList[x]->getYPos() == yPos) &&
			(objectList[x]->getType() != whatColorIAm) )
		{
			return true;
		}
	}
	return false;
}
bool RVB_Map::areThereAnyFriendsAt(int xPos, int yPos, entityType whatColorIAm)
{
	int size = objectList.size();
	for(int x = 0; x < size; x++)
	{
		if( (objectList[x]->getXPos() == xPos) &&
			(objectList[x]->getYPos() == yPos) &&
			(objectList[x]->getType() == whatColorIAm) )
		{
			return true;
		}
	}
	return false;
}

void RVB_Map::updateBullets()
{
	// setup a reverse iterator
	vector<RVB_Bullet*>::reverse_iterator rItr = bulletList.rbegin();

	// iterate through the list of bullets
	for(; rItr != bulletList.rend(); rItr++)
	{
		// if any of the bullets are no longer active, 
		if((*rItr)->getActive() == false)
		{
			// remove them from the list
			delete(*rItr);
			bulletList.erase(rItr.base() - 1);
			break;
		}
	}
}

void RVB_Map::makeBullet(RVB_Bullet *newBullet)
{
	// add the new bullet that was just passed in to the vector
	bulletList.push_back(newBullet);
}