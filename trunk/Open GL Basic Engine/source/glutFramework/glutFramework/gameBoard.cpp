#include "gameBoard.h"
#include <vector>
#include <fstream>
#include "objectManager.h"
#include "robot.h"
#include "objectEnums.h"
#include "oglGameVars.h"
#include "Oswitch.h"
#include "Oteleport.h"
#include <cmath>
#include <ctime>

using namespace std;

gameBoard::gameBoard() : curState(GB_LOGICVIEW), processSubRecurCounter(0)
{
	std::vector<std::vector<mapTile*>> mapListTemp;

	// make it all empty
	mapList.resize(1);
	for(int x = 0; x < 1; x++)
	{
		mapList[x].resize(1);
	}
	// populate it with default objects

	mapTile* temp;	

	for(int x = 0; x < 1; x++)
	{
		vector<mapTile*>::iterator itr = mapList[x].begin();
		
		for (; itr != mapList[x].end(); itr++)
		{
			temp = new mapTile(TEmpty, true);
			(*itr) = temp;
		}
	}
	initialize();
}

gameBoard::gameBoard(int nWidth, int nHeight)
{
	Width = nWidth;
	Height = nHeight;
	// make it all empty
	mapList.resize(nWidth);
	for(int x = 0; x < nWidth; x++)
	{
		mapList[x].resize(nHeight);
	}
	// populate it with default objects

	mapTile* temp;	

	for(int x = 0; x < nWidth; x++)
	{
		vector<mapTile*>::iterator itr = mapList[x].begin();
		
		for (; itr != mapList[x].end(); itr++)
		{
			temp = new mapTile(TEmpty, true);
			(*itr) = temp;
		}
	}
	initialize();
}

gameBoard::~gameBoard()
{
	// delete the vectors
	for(int x = 0; x < Width; x++)
		for(int y = 0; y < Height; y++)
			delete mapList[x][y];
	mapList.clear();
	cleanup();
}

bool gameBoard::update()
{
	//for(int x = 0; x < Height; x++)
	//{
	//	vector<mapTile*>::iterator itr = mapList[x].begin();
	//	
	//	for (; itr != mapList[x].end(); itr++)
	//	{
	//		(*itr)->update();
	//	}
	//}

	if(curState == GB_EXECUTION)
	{
		timer = clock();
		if(timer >= startTime + processSpeed)
		{
			if(!switchInProgress)
			{
				processRobot();
			}
			else
			{
				processSwitch();	
			}
			startTime = clock();
		}
	}

	if(curState == GB_VIEWSCORE)
	{
		//this will display users score for newly completed level
		//then display animation of numbers being added to total score
		//then the total score is displayed 
		//save game progress
		
		//upon user clicking advance button, set curState to GB_FINISHED
	}
	
	if(curState == GB_FINISHED)
	{
		//load next level
		//move to next level

		//upon clicking advance button, set curState to GB_LOGICVIEW
	}

	return true;
}

bool gameBoard::draw()
{
	blackImage->drawImage();
	int imageWidth = 144;
	int imageHeight = 72;
	imageWidth *= scale;
	imageHeight *= scale;
	int hw = (imageWidth/2);	// half width
	int hh = (imageHeight/2);	// half height

	int basex = 0;
	int basey = 0;
	int drawAtX = 0;
	int drawAtY = 0;

	for(int x = 0; x < Width; x++)
	{
		basex = x*hw;
		basey = x*hh;
		vector<mapTile*>::iterator itr = mapList[x].begin();
		//std::cout << "map offsets x/y " << mapOffsetX << " " << mapOffsetY << "Current x/y" << currentX << " " << currentY << endl;
		for(int y = 0; y < Height; y++)
		{
			drawAtX = mapOffsetX + (x * hw - (y * hw) + (hw));
			drawAtY = mapOffsetY + (y * hh) + (x * hh);
			if((*itr)->getIsActive())
				drawTile((*itr)->getType(), drawAtX, drawAtY, scale, true);
			else
				drawTile((*itr)->getType(), drawAtX, drawAtY, scale, false);
			itr++;
			if((x == robotX) && (y == robotY))
			{
				// draw the robot/objects
				// todo - this will eventually iterate through all the objects in the 
				// game board and draw them in the proper spot if they should be drawn at all
				// for the time being, the only "object" that requires being drawn is the robot
				//drawAtX = mapOffsetX + (robotX * hw - (robotY * hw) + (hw));
				//drawAtY = mapOffsetY + (robotY * imageHeight - (robotY * hh) + (robotX * hh) + hh/2);
				drawAtY += hh/2;
				
				// conditional drawing based on height
				if(mapList[robotX][robotY]->getType() == TRaised1)
				{
					drawAtY -= (hh*0.45);
				}
				else if(mapList[robotX][robotY]->getType() == TRaised2)
				{
					drawAtY -= (hh*0.9);
				}
				else if(mapList[robotX][robotY]->getType() == TRaised3)
				{
					drawAtY -= (hh*1.35);
				}
				else if(mapList[robotX][robotY]->getType() == TRaised4)
				{
					drawAtY -= (hh*1.8);
				}

				drawObject(0, drawAtX, drawAtY, scale);
			}
		}
	}
	

	// temp text crap
	if(drawText)
	{
		GameVars->fontOurs.drawText(rand()%500, rand()%500, "SEEAUTS");
	}

	return true;
}


bool gameBoard::moveTile(int sX, int sY, int dX, int dY)
{
	mapList[dX][dY] = mapList[sX][sY];
	return true;
}

bool gameBoard::setTileType(int x, int y, tileTypeEnum ntileType)
{
	mapList[x][y]->setType(ntileType);
	return true;
}

tileTypeEnum gameBoard::getTileType(int x, int y)
{
	return mapList[x][y]->getType();
}

bool gameBoard::drawTile(tileTypeEnum nType, int txPos, int tyPos, double scale, bool isActive)
{
	glClearColor(0, 0, 0, 0);
	oglTexture2D* toDraw;

	toDraw = tileImages[nType];

	int tempdx = toDraw->dX;
	int tempdy = toDraw->dY;

	toDraw->dX *= scale;
	toDraw->dY *= scale;

	toDraw->mX = txPos;
	toDraw->mY = tyPos;

	if(isActive)
		toDraw->drawImageSegment(0.0, 0.0, 0.5, 0.0, 0.0, 1.0, 0.5, 1.0, 1);
	else
		toDraw->drawImageSegment(0.5, 0.0, 1.0, 0.0, 0.5, 1.0, 1.0, 1.0, 1);

	toDraw->dX = tempdx;
	toDraw->dY = tempdy;

	return true;
}

void gameBoard::initialize()
{
	oglTexture2D* tempTile;
	tempTile = new oglTexture2D(); tempTile->loadImage("tiles/TEmpty.png",		 144, 144); tileImages.push_back(tempTile);		
	tempTile = new oglTexture2D(); tempTile->loadImage("tiles/TDefault.png",	 144, 144); tileImages.push_back(tempTile);	
	tempTile = new oglTexture2D(); tempTile->loadImage("tiles/TRaised1.png",	 144, 144); tileImages.push_back(tempTile);
	tempTile = new oglTexture2D(); tempTile->loadImage("tiles/TRaised2.png",	 144, 144); tileImages.push_back(tempTile);
	tempTile = new oglTexture2D(); tempTile->loadImage("tiles/TRaised3.png",	 144, 144); tileImages.push_back(tempTile);
	tempTile = new oglTexture2D(); tempTile->loadImage("tiles/TRaised4.png",	 144, 144); tileImages.push_back(tempTile);
	tempTile = new oglTexture2D(); tempTile->loadImage("tiles/THalfTopL.png",	 144, 144); tileImages.push_back(tempTile);
	tempTile = new oglTexture2D(); tempTile->loadImage("tiles/THalfTopR.png",	 144, 144); tileImages.push_back(tempTile);
	tempTile = new oglTexture2D(); tempTile->loadImage("tiles/THalfBottomL.png", 144, 144); tileImages.push_back(tempTile);	
	tempTile = new oglTexture2D(); tempTile->loadImage("tiles/THalfBottomR.png", 144, 144); tileImages.push_back(tempTile);	
	tempTile = new oglTexture2D(); tempTile->loadImage("tiles/TGap.png",		 144, 144); tileImages.push_back(tempTile);		
	tempTile = new oglTexture2D(); tempTile->loadImage("tiles/TElectric.png",	 144, 144); tileImages.push_back(tempTile);	
	tempTile = new oglTexture2D(); tempTile->loadImage("tiles/TElectricTL.png",	 144, 144); tileImages.push_back(tempTile);	
	tempTile = new oglTexture2D(); tempTile->loadImage("tiles/TElectricTR.png",	 144, 144); tileImages.push_back(tempTile);	
	tempTile = new oglTexture2D(); tempTile->loadImage("tiles/TElectricBL.png",	 144, 144); tileImages.push_back(tempTile);	
	tempTile = new oglTexture2D(); tempTile->loadImage("tiles/TElectricBR.png",	 144, 144); tileImages.push_back(tempTile);	
	tempTile = new oglTexture2D(); tempTile->loadImage("tiles/TIce.png",		 144, 144); tileImages.push_back(tempTile);		
	tempTile = new oglTexture2D(); tempTile->loadImage("tiles/TWater.png",		 144, 144); tileImages.push_back(tempTile);		
	tempTile = new oglTexture2D(); tempTile->loadImage("tiles/TSwitchTL.png",	 144, 144); tileImages.push_back(tempTile);	
	tempTile = new oglTexture2D(); tempTile->loadImage("tiles/TSwitchTR.png",	 144, 144); tileImages.push_back(tempTile);	
	tempTile = new oglTexture2D(); tempTile->loadImage("tiles/TSwitchBL.png",	 144, 144); tileImages.push_back(tempTile);	
	tempTile = new oglTexture2D(); tempTile->loadImage("tiles/TSwitchBR.png",	 144, 144); tileImages.push_back(tempTile);	
	tempTile = new oglTexture2D(); tempTile->loadImage("tiles/TSwitch.png",		 144, 144); tileImages.push_back(tempTile);	
	tempTile = new oglTexture2D(); tempTile->loadImage("tiles/TProgramTL.png",	 144, 144); tileImages.push_back(tempTile);	
	tempTile = new oglTexture2D(); tempTile->loadImage("tiles/TProgramTR.png",	 144, 144); tileImages.push_back(tempTile);	
	tempTile = new oglTexture2D(); tempTile->loadImage("tiles/TProgramBL.png",	 144, 144); tileImages.push_back(tempTile);	
	tempTile = new oglTexture2D(); tempTile->loadImage("tiles/TProgramBR.png",	 144, 144); tileImages.push_back(tempTile);	
	tempTile = new oglTexture2D(); tempTile->loadImage("tiles/TProgram.png",	 144, 144); tileImages.push_back(tempTile);	
	tempTile = new oglTexture2D(); tempTile->loadImage("tiles/TBreakableTL.png", 144, 144); tileImages.push_back(tempTile);	
	tempTile = new oglTexture2D(); tempTile->loadImage("tiles/TBreakableTR.png", 144, 144); tileImages.push_back(tempTile);	
	tempTile = new oglTexture2D(); tempTile->loadImage("tiles/TBreakableBL.png", 144, 144); tileImages.push_back(tempTile);	
	tempTile = new oglTexture2D(); tempTile->loadImage("tiles/TBreakableBR.png", 144, 144); tileImages.push_back(tempTile);	
	tempTile = new oglTexture2D(); tempTile->loadImage("tiles/TSolid.png",		 144, 144); tileImages.push_back(tempTile);		
	tempTile = new oglTexture2D(); tempTile->loadImage("tiles/TBreakable.png",	 144, 144); tileImages.push_back(tempTile);	
	tempTile = new oglTexture2D(); tempTile->loadImage("tiles/TStart.png",		 144, 144); tileImages.push_back(tempTile);		
	tempTile = new oglTexture2D(); tempTile->loadImage("tiles/TEnd.png",		 144, 144); tileImages.push_back(tempTile);		
	tempTile = new oglTexture2D(); tempTile->loadImage("tiles/TDoorTL.png",		 144, 144); tileImages.push_back(tempTile);	
	tempTile = new oglTexture2D(); tempTile->loadImage("tiles/TDoorTR.png",		 144, 144); tileImages.push_back(tempTile);	
	tempTile = new oglTexture2D(); tempTile->loadImage("tiles/TDoorBL.png",		 144, 144); tileImages.push_back(tempTile);	
	tempTile = new oglTexture2D(); tempTile->loadImage("tiles/TDoorBR.png",		 144, 144); tileImages.push_back(tempTile);			
	tempTile = new oglTexture2D(); tempTile->loadImage("tiles/TTeleport.png",	 144, 144); tileImages.push_back(tempTile);			
	robotImage = new oglTexture2D(); robotImage->loadImage("object/robotDefault.png", 195*0.45, 110*2*0.45);

	// variables and whatnot
	mapOffsetX = 0;
	mapOffsetY = 0;
	scale = 1;

	imageBaseWidth = 144;
	imageBaseHeight = 72;

	imageWidth = imageBaseWidth *scale;
	imageHeight = imageBaseHeight *scale;

	imageWidth *= scale;
	imageHeight *= scale;

	hw = (int)(imageWidth/2);	// half width
	hh = (int)(imageHeight/2);	// half height

	screenWidth = 1024;
	screenHeight = 768;
	screenEdgeWidth = 1024;
	screenEdgeHeight = 768;

	screenEdge = 0.03;
	moveSpeed = scale * 0.1;
	maxscale = 2.0;
	minscale = 0.2;

	overallWidth = (Height + Width) * hw;
	overallHeight = (Height + Width) * hh;

	centerX = (int)((Width+1)/2);
	centerY = (int)((Height+1)/2);
	currentX = centerX;
	currentY = centerY;

	// time to setup offsets based on centers
	logicBank = GameVars->Instance()->getAllLogicBlocks();
	objectList.clear();
	robotAlive = true;
	drawText = false;

	SUB1 = new subroutine();
	SUB2 = new subroutine();
	interfaceHeight = 190;
	screenHeight -= interfaceHeight;
	screenHeight -= (int)hh;
	mapRotation = 0;

	mouseTimer = 0;
	mouseTimerStart = 0;

	switchInProgress = false;
	switchToggled = false;

	processSpeed = 500;

	reprogramHit = false;

	blackImage = new oglTexture2D;
	blackImage->loadImage("black.png", 1024, 768);
	blackImage->dX = 1024;
	blackImage->dY = 768;
	blackImage->mX = 0;
	blackImage->mY = 0;
}

void gameBoard::cleanup()
{
	vector<oglTexture2D*>::iterator itr = tileImages.begin();
	for(; itr != tileImages.end(); itr++)
	{
		delete (*itr);
	}
	tileImages.clear();
	delete SUB1;
	delete SUB2;

	//while(tileImages.begin() != tileImages.end())
	//{
	//	
	//	
	//}

}

bool gameBoard::setOffsets(int x, int y)
{
	mapOffsetX = x;
	mapOffsetY = y;
	return true;
}

bool gameBoard::LoadGameMapFromFile(std::string filename)
{

	//cout << "attempting to open file: " << filename << endl;
	ifstream mapfile;
	//if(!mapfile)
	//	return false;

	mapfile.open(filename.c_str());
	//string line;
	
	int nWidth;
	int nHeight;
	int startDirection;
	int bytesAvail;
	mapfile >> nWidth;
	mapfile >> nHeight;
	mapfile >> startDirection;
	mapfile >> bytesAvail;

	bool moveForwardAvail;		//1
	mapfile >> moveForwardAvail;

	bool moveForwardUntilAvail;	//2
	mapfile >> moveForwardUntilAvail;

	bool turnLeftAvail;			//3
	mapfile >> turnLeftAvail;

	bool turnRightAvail;		//4
	mapfile >> turnRightAvail;

	bool punchAvail;			//5
	mapfile >> punchAvail;

	bool climbAvail;			//6
	mapfile >> climbAvail;

	bool crouchAvail;			//7
	mapfile >> crouchAvail;

	bool jumpAvail;				//8
	mapfile >> jumpAvail;

	bool activateAvail;			//9
	mapfile >> activateAvail;

	bool sub1Avail;				//10
	mapfile >> sub1Avail;

	bool sub2Avail;				//11
	mapfile >> sub2Avail;

	GameVars->setCurrentLogicBank(moveForwardAvail, moveForwardUntilAvail, turnLeftAvail, turnRightAvail,
								  punchAvail, climbAvail, crouchAvail, jumpAvail, activateAvail, sub1Avail, sub2Avail);

	
	GameVars->setCurrentLevelBytes(bytesAvail);

	// make it all empty
	mapList.resize(nWidth);
	for(int x = 0; x < nWidth; x++)
	{
		mapList[x].resize(nHeight);
	}

	mapTile* temp;	

	for(int x = 0; x < nWidth; x++)
	{
		vector<mapTile*>::iterator itr = mapList[x].begin();
		
		for (; itr != mapList[x].end(); itr++)
		{
			temp = new mapTile(TEmpty, true);
			(*itr) = temp;
		}
	}
	Width = 0;
	Height = 0;
	Width = nWidth;
	Height = nHeight;

	//GameVars->setLevelHeight(Height);
	//GameVars->setLevelWidth(Width);

	int temptile;

	for(int y = 0; y < Height; y++)
	{
		for(int x = 0; x < Width; x++)
		{
			mapfile >> temptile;
			if((temptile == THalfBottomL) || (temptile == THalfBottomR))
			{
				temptile = TGap;
				// do NOT comment out this cout line, it is for verification purposes when loading a map
				std::cout << filename.c_str() << "is referencing a THalfBottomL or THalfBottomR, I have converted it to a TGap" << endl;
			}
			this->setTileType(x, y, (tileTypeEnum)temptile);
			// if its a start spot, add the robot to the object list
			if(temptile == TStart)
			{
				//OM->addNewObject<robot>(x, y, 1, ORobot);
				robot* tempObj;
				tempObj = new robot(x, y, startDirection, ORobot);
				myRobot = tempObj;
				objectList.push_back(tempObj);
				robotX = x;
				robotY = y;
				robotStartX = x;
				robotStartY = y;
				currentX = robotX;
				currentY = robotY;
			}
		}
	}

	/////////////////////////////////////////////////////////////////
	// now lets see if there's any switches in the game map
	// either way lets clear the switchlist manager first
	GameVars->SM->clearSwitchList();

	if(!mapfile.eof())
	{
		int switchcount;
		int teleportcount;
		mapfile >> switchcount;
		mapfile >> teleportcount;

		int numcnt;
		int myX;
		int myY;
		int tempX;
		int tempY;
		Oswitch* tempObj;
		Oteleport* tempObjT;

		// read in the switches

		for(int x = 0; x < switchcount; x++)
		{
			
			// switch data layout
			// # of tiles controlled
			mapfile >> numcnt;

			// x y of this switch
			mapfile >> myX;
			mapfile >> myY;

			/////////////////////////////////////
			// add a switch to the switch manager
			GameVars->SM->addSwitch(myX, myY, numcnt);
			
			tempObj = new Oswitch(myX, myY, 0, OSwitch);	// old code - if SM works right, this gets removed

			for(int sx = 0; sx < numcnt; sx++)
			{
				///////////////////////////////
				// x y of first tile controlled
				mapfile >> tempX;
				mapfile >> tempY;

				///////////////////////////////////////////////////////////
				// add this target to the last switch in the switch manager
				GameVars->SM->addTargetToLastSwitch(tempX, tempY);

				tempObj->addTarget(tempX, tempY); // old code - if SM works right, this gets removed
				// ...
				// x y of nth tile controlled
			}
			switchList.push_back(tempObj);	// old code - if SM works right, this gets removed
		}

		// read in the teleporters
		for(int x = 0; x < teleportcount; x++)
		{

			mapfile >> myX;
			mapfile >> myY;
			mapfile >> tempX;
			mapfile >> tempY;
			tempObjT = new Oteleport(myX, myY, 0, OTeleport);
			tempObjT->setTarget(tempX, tempY);
			teleportList.push_back(tempObjT);
		}
	}

	mapfile.close();

	centerX = (int)((Width+1)/2);
	centerY = (int)((Height+1)/2);
	currentX = centerX;
	currentY = centerY;

	imageBaseWidth = 144;
	imageBaseHeight = 72;

	hw = (imageWidth/2);	// half width
	hh = (imageHeight/2);	// half height
	overallWidth = (Height + Width) * hw;
	overallHeight = (Height + Width) * hh;

	recalcPositions();

	currentX = robotX;
	currentY = robotY;

	return true;
}


bool gameBoard::setScale(double newScale)
{
	scale = newScale;
	return true;
}

double gameBoard::getScale()
{
	return scale;
}

void gameBoard::processMouse(int x, int y)
{
	mouseX = x;
	mouseY = y;
	mapScroll();
}

void gameBoard::processMouseClick(int button, int state, int x, int y)
{
	// Used for mouse wheel, has to be up
	if (state == GLUT_UP )
	{
		if ( button == GLUT_WHEEL_UP )
		{
			//std::cout << "Mouse up" << endl;
		}
		else if( button == GLUT_WHEEL_DOWN )
		{
			//std::cout << "Mouse down" << endl;
		}
		else if( button == GLUT_LEFT_BUTTON )
		{
			//std::cout << "Mouse LB" << endl;
		}
		else if( button == GLUT_RIGHT_BUTTON )
		{
			//std::cout << "Mouse RB" << endl;
		}
		else if( button == GLUT_MIDDLE_BUTTON )
		{
			//std::cout << "Mouse MB" << endl;
		}
	}

	//std::cout << "Button = " << button << " - State = " << state << " X,Y " << x << ", " << y <<  endl;
}

void gameBoard::mapScroll()
{
	double mMoveSpeed = moveSpeed/2;
	// mouse stuff
	recalcPositions();

	mouseTimer = clock();
	int mouseDelay = 50;

	// see if mouse is at top of screen
	if((mouseY >= 0) && (mouseY < screenEdgeHeight*screenEdge))
	{
		if(mouseTimer > mouseTimerStart + mouseDelay)
		{
			mouseTimerStart = clock();
			//mapOffsetY+= moveSpeed;
			panup();
		}
	}

	// see if mouse is at bottom of screen
	if((mouseY < screenEdgeHeight) && (mouseY > (screenEdgeHeight - (screenEdgeHeight*screenEdge))))
	{
		if(mouseTimer > mouseTimerStart + mouseDelay)
		{
			mouseTimerStart = clock();
			pandown();
		}
	}
	// see if mouse is at left side of screen
	if((mouseX >= 0) && (mouseX < screenEdgeWidth * screenEdge))
	{
		if(mouseTimer > mouseTimerStart + mouseDelay)
		{
			mouseTimerStart = clock();
			panleft();
		}
	}
	// see if mouse is at right side of screen
	if((mouseX < screenEdgeWidth) && (mouseX > (screenEdgeWidth - (screenEdgeWidth * screenEdge))))
	{
		if(mouseTimer > mouseTimerStart + mouseDelay)
		{
			mouseTimerStart = clock();
			panright();
		}
	}
	verifyCameraCenter();
}

void gameBoard::keyboardInput(unsigned char c, int x, int y)
{
	vector<object*>::iterator oitr;
	vector<logicBlock*>::iterator lbitr;

	recalcPositions();

	double kbmovespeed = moveSpeed;
	switch(c)
	{
	case 'q': //up-left
	case '7':
		panupleft();
		break;
	case 'e': // up-right
	case '9':
		panupright();
		break;
	case 'z': // down-left
		pandownleft();
		break;
	case '1':
		pandownleft();
		//find the robot
		oitr = objectList.begin();
		for(;oitr != objectList.end(); oitr++)
		{
			if((*oitr)->getType() == ORobot)
			{
			// find ACTIVATE in allLogicBlocks
				(*oitr)->rotate(-1);
			}
		}	
		break;
	case 'c': // down-right
	case '3':
		pandownright();
		break;
	case 'w': // up
	case '8':
		panup();
		break;
	case 'a':  // left
	case '4':
		panleft();
		break;
	case 'd':  // right
	case '6':
		panright();
		break;
	case 's': // down
		pandown();
		break;
	case '2':
		pandown();
		break;
	case '-':	// zoom out (decrease scale)
	case '_':
		zoomout();
		break;
	case '=':	// zoom in (increase scale)
	case '+':
		zoomin();
		break;
	case '.':	// reset scale
		scale = 1;
		break;
	case '[':	// center on robot
	case '5':
		center();
		break;
	case 't': // 
	case 'T':
		break; // disabled
		// process the full switch list
		/*GameVars->SM->startProcessing(robotX, robotY);
		for(int x = 0; x < GameVars->SM->getNumTargets(robotX, robotY); x++)
		{
			mapList[GameVars->SM->getCurrentTargetX(robotX, robotY)][GameVars->SM->getCurrentTargetY(robotX, robotY)]->toggleActive();
			GameVars->SM->advanceTarget(robotX, robotY);
		}*/
		break;
	case 'X':
		break; // disabled
		/*robotX--;
		currentX = robotX;
		std::cout << "robot x/y = " << robotX << ", " << robotY << endl;
		break;*/
	case 'x':
		break; // disabled
		/*robotX++;
		currentX = robotX;
		std::cout << "robot x/y = " << robotX << ", " << robotY << endl;
		break;*/
	case 'Y':
		break; // disabled
		/*robotY--;
		currentY = robotY;
		std::cout << "robot x/y = " << robotX << ", " << robotY << endl;
		break;*/
	case 'y':
		break; // disabled
		/*robotY++;
		currentY = robotY;
		std::cout << "robot x/y = " << robotX << ", " << robotY << endl;
		break;*/
	case '/':
		rotateMapLeft();
		break; 
	case '*':
		rotateMapRight();
		break; 
	case '`':
		// toggle drawtext
		if(drawText == false)
			drawText = true;
		else
			drawText = false;
		break;
	case '~':
		// level skip
		curState = GB_FINISHED;
		break;
	default:
		break;
	}

	//std::cout << "RobotX,y = " << robotX << ", " << robotY << std::endl;

	if (scale > maxscale)	scale = maxscale;
	if (scale < minscale)	scale = minscale;

	keepRobotOnTheBoard();
	verifyCameraCenter();
	teleporterCheck();
}

bool gameBoard::resetMap()
{
	//OM->startOver();

	std::vector<object*>::iterator oitr = objectList.begin();
	if(mapRotation > 0)
		while(mapRotation > 0)
			rotateMapLeft();

	if(mapRotation < 0)
		while(mapRotation < 0)
			rotateMapRight();

	GameVars->SM->resetAllSwitches();
	switchInProgress = false;
	switchToggled = false;
	scale = 1.0;
	

	for(;oitr != objectList.end(); oitr++)
	{
		if((*oitr)->getType() == ORobot)
		{
			robotX = robotStartX;
			robotY = robotStartY;
			(*oitr)->setXPos(robotX);
			(*oitr)->setYPos(robotY);
			(*oitr)->reset();
		}
	}

	for(int x = 0; x < Width; x++)
	{
		for(int y = 0; y < Height; y++)
		{
			mapList[x][y]->resetActive();
		}
	}
	return true;
}


bool gameBoard::drawObject(int objectType, int txPos, int tyPos, double scale)
{
	// for now, lets just draw the robot, additional code to come
	txPos += (hh*0.7)+5*scale;
	tyPos += (hh*0.2)+5*scale;
	glClearColor(0, 0, 0, 0);
	oglTexture2D* toDraw;

	toDraw = robotImage;

	int tempdx = toDraw->dX;
	int tempdy = toDraw->dY;

	toDraw->dX *= scale;
	toDraw->dY *= scale;

	toDraw->mX = txPos;
	toDraw->mY = tyPos;

	// find the robot
	vector<object*>::iterator oitr = objectList.begin();
	for(;oitr != objectList.end(); oitr++)
	{
		if((*oitr)->getType() == ORobot)
		{
			// now lets see which direction its facing
			switch((*oitr)->getDirection())
			{
			case 0:
				toDraw->drawImageSegment(0.0, 0.0,	0.25, 0.0,
										 0.0, 1.0,  0.25, 1.0, 1);
				break;
			case 1:
				toDraw->drawImageSegment(0.25, 0.0,	0.5, 0.0,
										 0.25, 1.0, 0.5, 1.0, 1);
				break;
			case 2:
				toDraw->drawImageSegment(0.5, 0.0,	0.75, 0.0,
										 0.5, 1.0,  0.75, 1.0, 1);
				break;
			case 3:
				toDraw->drawImageSegment(0.75, 0.0,	1.0, 0.0,
										 0.75, 1.0, 1.0, 1.0, 1);
				break;
			}
		}
	}
	toDraw->dX = tempdx;
	toDraw->dY = tempdy;

	return true;
}

void gameBoard::processRobot()
{
	// find the robot
	vector<object*>::iterator oitr = objectList.begin();
	vector<Oswitch*>::iterator sitr = switchList.begin();
	//Oswitch* tempSwitch;
	//tileTypeEnum robotSquare;
	//int			 robotDirection;
	//tileTypeEnum destType;
	//bool		 destActive;
	delayAdvance = false;

	//===================================
	//  Command Highlighting
	oitr = objectList.begin();
	for(;oitr != objectList.end(); oitr++)
	{
		if((*oitr)->getType() == ORobot)
		{
			switch((*oitr)->getNextCommand())
			{
			case SUBR1:
				if(mInterfaceAdvanceHandler)
				{
					mInterfaceAdvanceHandler(TAB_SUB1, SUB1->getNextCommandBlock());
				}
				break;
			case SUBR2:
				if(mInterfaceAdvanceHandler)
				{
					mInterfaceAdvanceHandler(TAB_SUB2, SUB2->getNextCommandBlock());
				}
				break;
			default:
				if(mInterfaceAdvanceHandler)
				{
					mInterfaceAdvanceHandler(TAB_MAIN, (*oitr)->getNextCommandBlock());
				}
				break;
			}
		}
	}

	oitr = objectList.begin();
	for(;oitr != objectList.end(); oitr++)
	{
		if((*oitr)->getType() == ORobot)
		{
			if((*oitr)->getAlive() == false)
			{
				// tempabc
				curState = GB_ROBOTDIED;
				return;
			}
			
			switch((*oitr)->getNextCommand())
			{
			case SUBR1:
				GameVars->commandsProcessed++;
				delayAdvance = processSub(1);
				break;
			case SUBR2:
				GameVars->commandsProcessed++;
				delayAdvance = processSub(2);
				break;
			case MOVE_FORWARD1:
				GameVars->commandsProcessed++;
				this->RCmoveRobotForward();
				// special case for ice squares
				if(mapList[robotX][robotY]->getType() == TIce)
				{
					if(this->RCcanRobotMoveForward((*oitr)->getDirection(), 1))
					{
						delayAdvance = true;
					}
				}
				break;
			case TURN_LEFT1:
				(*oitr)->rotate(-1);
				GameVars->commandsProcessed++;
				break;
			case TURN_RIGHT1:
				(*oitr)->rotate(1);
				GameVars->commandsProcessed++;
				break;
			case CROUCH:  // just like move forward above, only far less squares that can be moved into
				this->RCcrouch();
				GameVars->commandsProcessed++;
				break;
			case CLIMB:	  // just like move forward above, only far less squares that can be moved into
				this->RCclimb();
				GameVars->commandsProcessed++;
				break;
			case JUMP:	  // just like move forward above, only far less squares that can be moved into
				this->RCjumpRobotForward();
				GameVars->commandsProcessed++;
				break;
			case PUNCH:	  // just like move forward above, only far less squares that can be moved into
				this->RCpunch();
				GameVars->commandsProcessed++;
				break;
			case MOVE_FORWARD_UNTIL_UNABLE: // just like move forward above, only no advancement of command until destination square is invalid
				delayAdvance = this->RCmoveRobotForward();
				GameVars->commandsProcessed++;
				break;
			case SUB:	// special case
				break;
			case LOOP:	// since this should always be displayed as the last command automatically, this isn't really needed
						// the object's ai list automagically loops when it gets to its end
				break;
			case ACTIVATE:	// for now lets just check for a door so we can see it working in testmap1
				this->RCactivate();
				GameVars->commandsProcessed++;
				break;
			case DO_NOT_PROCESS:
				break;
			case STOP:
				GameVars->commandsProcessed++;
				delayAdvance = true;
				break;
			default:
				break;
			}
		}

		if(delayAdvance)
		{
			GameVars->commandsProcessed--;
		}

		if(!delayAdvance)
		{
			(*oitr)->advanceCommand();
		}
		
		currentX = robotX;
		currentY = robotY;
		verifyCameraCenter();
		teleporterCheck();
	}
}

bool gameBoard::robotAtEndSquare()
{
	// find the end square
	if ((mapList[robotX][robotY]->getType() == TEnd) && (mapList[robotX][robotY]->getIsActive()))
		return true;
	else
		return false;
}

GameBoardState gameBoard::getCurState()
{
	return curState;
}

void gameBoard::teleporterCheck()
{
	vector<Oteleport*>::iterator titr = teleportList.begin();
	// see if we're standing in a teleporter square
	if(mapList[robotX][robotY]->getType() == TTeleport)
	{
	//	std::cout << "robot has found a teleporter at " << robotX << ", " << robotY << std::endl;
		// if we are, lets find the teleporter in the list
		for(;titr < teleportList.end(); titr++)
		{
			if((((*titr)->getXPos()) == robotX) && (((*titr)->getYPos()) == robotY))
			{
					
				// draw before
				currentX = (*titr)->getTargetX();
				currentY = (*titr)->getTargetY();
				this->recalcPositions();
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				this->draw();
				glutSwapBuffers();

				timer = clock();
				startTime = clock();
				while(timer < startTime + processSpeed)
				{
					timer = clock();
				}
				

				// now lets teleport!
				robotX = (*titr)->getTargetX();
				robotY = (*titr)->getTargetY();
				keepRobotOnTheBoard();
				this->recalcPositions();
				
				
				// draw after
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				this->draw();
				glutSwapBuffers();

				timer = clock();
				startTime = clock();
				while(timer < startTime + 500)
				{
					timer = clock();
				}
			}
		}
	}
}

void gameBoard::keepRobotOnTheBoard()
{
	if(robotX < 0)			robotX = 0;
	if(robotX >= Width)		robotX = Width-1;
	if(robotY < 0)			robotY = 0;
	if(robotY >= Height)	robotY = Height-1;
}

bool gameBoard::interfaceHasFiredExecuteOrder(std::vector<logicBlock*> executionList, std::vector<logicBlock*> executionListSub1, std::vector<logicBlock*> executionListSub2)
{
	// reset the map
	/*for(int x = 0; x < Width; x++)
	{
		for(int y = 0; y < Height; y++)
		{
			mapList[x][y]->setActive(true);
		}
	}*/
	resetMap();
	GameVars->commandsProcessed = 0;

	SUB1->clearInstructions();
	std::vector<logicBlock*>::iterator sItr = executionListSub1.begin();
	for(; sItr != executionListSub1.end(); sItr++)
	{
		SUB1->addCommand(*sItr);
	}

	SUB2->clearInstructions();
	sItr = executionListSub2.begin();
	for(; sItr != executionListSub2.end(); sItr++)
	{
		SUB2->addCommand(*sItr);
	}

	// Find the robot
	std::vector<object*>::iterator oitr = objectList.begin();

	for(;oitr != objectList.end(); oitr++)
	{
		if((*oitr)->getType() == ORobot)
		{
			(*oitr)->reset();
			(*oitr)->setAlive(true);
			robotAlive = true;
			robotX = robotStartX;
			robotY = robotStartY;
			currentX = robotX;
			currentY = robotY;
			
			// Add every element of executionList from the interface to
			// the robot's instruction list.
			std::vector<logicBlock*>::iterator exeItr = executionList.begin();
			(*oitr)->clearInstructions();
			for(; exeItr < executionList.end()-1; exeItr++)
			{
				(*oitr)->addCommand(*exeItr);
				(*oitr)->coreDump();
				SUB1->coreDump();
				SUB2->coreDump();
			}
		}
	}
	
	curState = GB_EXECUTION;
	startTime = clock();
	return true;
}

bool gameBoard::interfaceHasFiredAbortOrder()
{
	if((curState == GB_EXECUTION) && (!robotAtEndSquare()))
	{
		curState = GB_LOGICVIEW;
		GameVars->SM->resetAllSwitches();
		switchInProgress = false;
		switchToggled = false;
	}
	return false;
}

bool gameBoard::interfaceHasFiredResetOrder()
{
	curState = GB_LOGICVIEW;
	
	resetMap();

	SUB1->clearInstructions();
	SUB2->clearInstructions();

	GameVars->commandsProcessed = 0;

	// Find the robot
	std::vector<object*>::iterator oitr = objectList.begin();

	for(;oitr != objectList.end(); oitr++)
	{
		if((*oitr)->getType() == ORobot)
		{
			(*oitr)->reset();
			(*oitr)->setAlive(true);
			robotAlive = true;
			robotX = robotStartX;
			robotY = robotStartY;
			currentX = robotX;
			currentY = robotY;
			
			(*oitr)->clearInstructions();			
		}
	}
	return false;
}


bool gameBoard::RCcanRobotLeaveSquare(int direction)
{
	// we just need to know information about our own square
	tileTypeEnum robotSquare = mapList[robotX][robotY]->getType();
	bool robotSquareActive = mapList[robotX][robotY]->getIsActive();

	// squares that can be left
	if( (robotSquare == TDefault) ||
		(robotSquare == TRaised1) ||
		(robotSquare == TRaised2) ||
		(robotSquare == TRaised3) ||
		(robotSquare == TRaised4) ||
		(robotSquare == TElectric) ||
		((robotSquare == TElectricTL) && (direction != 3)) ||
		((robotSquare == TElectricTR) && (direction != 0)) || 
		((robotSquare == TElectricBL) && (direction != 2)) ||
		((robotSquare == TElectricBR) && (direction != 1)) ||
		(robotSquare == TIce) ||
		(robotSquare == TWater) ||
		(robotSquare == TSwitchTL) ||
		(robotSquare == TSwitchTR) ||
		(robotSquare == TSwitchBL) ||
		(robotSquare == TSwitchBR) ||
		(robotSquare == TSwitch) ||
		(robotSquare == TProgramTL) ||
		(robotSquare == TProgramTR) ||
		(robotSquare == TProgramBL) ||
		(robotSquare == TProgramBR) ||
		(robotSquare == TProgram) ||
		((robotSquare == TBreakableTL) && (((!robotSquareActive) && (direction == 3)) || (direction != 3))) ||
		((robotSquare == TBreakableTR) && (((!robotSquareActive) && (direction == 0)) || (direction != 0))) ||
		((robotSquare == TBreakableBL) && (((!robotSquareActive) && (direction == 2)) || (direction != 2))) ||
		((robotSquare == TBreakableBR) && (((!robotSquareActive) && (direction == 1)) || (direction != 1))) ||
		(robotSquare == TSolid) ||
		(robotSquare == TBreakable) ||
		(robotSquare == TStart) ||
		(robotSquare == TEnd) ||
		( (robotSquare == TDoorTL) && (((direction == 3) && (!robotSquareActive)) || (direction != 3))) ||
		( (robotSquare == TDoorTR) && (((direction == 0) && (!robotSquareActive)) || (direction != 0))) ||
		( (robotSquare == TDoorBL) && (((direction == 2) && (!robotSquareActive)) || (direction != 2))) ||
		( (robotSquare == TDoorBR) && (((direction == 1) && (!robotSquareActive)) || (direction != 1))))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool gameBoard::RCwillRobotDieTryingToLeaveSquare(int direction)
{
	tileTypeEnum robotSquare = mapList[robotX][robotY]->getType();
	bool robotSquareActive = mapList[robotX][robotY]->getIsActive();
	
	if ((((robotSquare == TElectricTL) && (direction == 3)) || 
		 ((robotSquare == TElectricTR) && (direction == 0)) ||
		 ((robotSquare == TElectricBL) && (direction == 2)) ||
		 ((robotSquare == TElectricBR) && (direction == 1))) && robotSquareActive)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool gameBoard::RCcanRobotMoveForward(int direction, int destNum)
{
	int destX;
	int destY;
	tileTypeEnum robotSquare;
	tileTypeEnum destType;
	bool destActive;

	destX = robotX;
	destY = robotY;

	switch(direction)
	{
	case 0:// facing up/right (up on map)
		destY = robotY -destNum;					
		break;
	case 1:// facing down/right (right on map)
		destX = robotX +destNum;
		break;
	case 2:// facing down/left (down on map)
		destY = robotY + destNum;
		break;
	case 3:// facing up/left (left on map)
		destX = robotX -destNum;
		break;
	}
				
	if((destX >= 0) && (destX < Width) && (destY >= 0) && (destY < Height))
	{
		destType = mapList[destX][destY]->getType();
		destActive = mapList[destX][destY]->getIsActive();

		robotSquare = mapList[robotX][robotY]->getType();

		if ( (destType == TDefault) || 
			 ((destType == TRaised1) && ((robotSquare == TRaised1) || (robotSquare == TRaised2) || (robotSquare == TRaised3) || (robotSquare == TRaised4))) ||
			 ((destType == TRaised2) && ((robotSquare == TRaised2) || (robotSquare == TRaised3) || (robotSquare == TRaised4))) ||
			 ((destType == TRaised3) && ((robotSquare == TRaised3) || (robotSquare == TRaised4)))||
			 ((destType == TRaised4) && ((robotSquare == TRaised4))) ||
			 (destType == TGap) || 
			 (destType == TElectric) ||
			 ((destType == TElectricTL) && (direction != 1)) || 
			 ((destType == TElectricTR) && (direction != 2)) ||
			 ((destType == TElectricBL) && (direction != 0)) ||
			 ((destType == TElectricBR) && (direction != 3)) ||
			 (destType == TIce) ||
			 (destType == TWater) ||
			 (destType == TSwitchTL) ||
			 (destType == TSwitchTR) ||
			 (destType == TSwitchBL) ||
			 (destType == TSwitchBR) ||
			 (destType == TSwitch) ||
			 (destType == TProgramTL) ||
			 (destType == TProgramTR) ||
			 (destType == TProgramBL) ||
			 (destType == TProgramBR) ||
			 (destType == TProgram) ||
			 ((destType == TBreakableTL) && (((direction == 0) || (direction == 2) || (direction == 3)) || !destActive)) ||
			 ((destType == TBreakableTR) && (((direction == 0) || (direction == 1) || (direction == 3)) || !destActive)) ||
			 ((destType == TBreakableBL) && (((direction == 1) || (direction == 2) || (direction == 3)) || !destActive)) ||
			 ((destType == TBreakableBR) && (((direction == 0) || (direction == 1) || (direction == 2)) || !destActive)) ||
			 ((destType == TBreakable) && (!destActive)) ||
			 ((destType == TSolid) && (!destActive)) ||
			 (destType == TStart) ||
			 (destType == TEnd) ||
			 ( (destType == TDoorTL) && (((direction == 0) || (direction == 2) || (direction == 3)) || !destActive)) ||
			 ( (destType == TDoorTR) && (((direction == 0) || (direction == 1) || (direction == 3)) || !destActive)) ||
			 ( (destType == TDoorBL) && (((direction == 1) || (direction == 2) || (direction == 3)) || !destActive)) ||
			 ( (destType == TDoorBR) && (((direction == 0) || (direction == 1) || (direction == 2)) || !destActive)) ||
			 (destType == TTeleport))
		{
			return true; 
		}
	}
	return false;
}
bool gameBoard::RCwillRobotDieStayingHere()
{
	// robot vars
	tileTypeEnum robotSquare;

	robotSquare = mapList[robotX][robotY]->getType();
	bool robotSquareActive = mapList[robotX][robotY]->getIsActive();

	// if robot square = a bad square, return true, otherwise return false
	if(	(robotSquare == TGap) ||
		(robotSquare == TWater) ||
		((robotSquare == TElectric) && (robotSquareActive == true))
		)
	{
		return true;
	}
	else
	{
		return false;
	}
	
}



void gameBoard::RCjumpRobotForward()
{
	// first thing's first, lets find the robot
	std::vector<object*>::iterator oitr = objectList.begin();

	int distanceToMove = 2;

	for(;oitr != objectList.end(); oitr++)
	{
		if((*oitr)->getType() == ORobot)
		{
			int robotDirection = (*oitr)->getDirection();

			// lets see if we are going to die attempting to leave this squiare
			if(this->RCwillRobotDieTryingToLeaveSquare(robotDirection))
			{
				// yes we died
				(*oitr)->setAlive(false);
				robotAlive = false;
				return;
			}

			// at this point we're still alive, now lets see if we can leave this square
			if(!this->RCcanRobotLeaveSquare(robotDirection))
			{
				// no we can't leave
				return;
			}

			// now lets see if our jump can allow us to move forward from our square to the destination square
			if(!this->RCcanRobotMoveForward(robotDirection, 2))
			{
				// no we can't actually land at the destination square
				// lets check the one square infront of us and see if that can be landed on instead
				if(!this->RCcanRobotMoveForward(robotDirection, 1))
				{
					// no we can't even move forward 1 square, you're screwed, stay put
					return;
				}
				else
				{
					// at least we can move forward 1 square, lets set our distance to 1
					distanceToMove = 1;
				}
			}
			else
			{
				// wow we can jump two squares forward! amazing!
				// lets see if we can also clear the square infront of us
				if(!this->RCcanRobotMoveForward(robotDirection, 1))
				{
					// nope we cannot even move out of this square, we'll just have to stay put
					return;
				}
			}

			// ok at this point we're not dead, we can leave our square, and we can either jump 2 squares clearing the middle
			// square, or can jump forward 1 square, at least we're not dead.. yet
			// lets do the jump and see what happens!

			int destX = robotX;
			int destY = robotY;
			switch(robotDirection)
			{
			case 0:// facing up/right (up on map)
				destY = robotY - distanceToMove;					
				break;
			case 1:// facing down/right (right on map)
				destX = robotX + distanceToMove;
				break;
			case 2:// facing down/left (down on map)
				destY = robotY + distanceToMove;
				break;
			case 3:// facing up/left (left on map)
				destX = robotX - distanceToMove;
				break;
			}
			if( (destX >=0) && (destX < Width) )
				robotX = destX;
			if( (destY >=0) && (destY < Height) )
				robotY = destY;

			(*oitr)->setXPos(robotX);
			(*oitr)->setYPos(robotY);
			
			// now that we've moved forward
			// lets see if we're about to die
			if(this->RCwillRobotDieStayingHere())
			{
				// yes we died
				(*oitr)->setAlive(false);
				robotAlive = false;
			}
		}				
	}	
};

void gameBoard::RCcrouch()
{
	// first thing's first, lets find the robot
	std::vector<object*>::iterator oitr = objectList.begin();

	for(;oitr != objectList.end(); oitr++)
	{
		if((*oitr)->getType() == ORobot)
		{
			int robotDirection = (*oitr)->getDirection();
			tileTypeEnum robotSquare = mapList[robotX][robotY]->getType();
			tileTypeEnum destSquare;

			int destX = robotX;
			int destY = robotY;
			int landingX = robotX;
			int landingY = robotY;
			switch(robotDirection)
			{
			case 0:// facing up/right (up on map)
				destY = robotY -1;
				landingY = robotY - 2;
				break;
			case 1:// facing down/right (right on map)
				destX = robotX +1;
				landingX = robotX +2;
				break;
			case 2:// facing down/left (down on map)
				destY = robotY + 1;
				landingY = robotY +2;
				break;
			case 3:// facing up/left (left on map)
				destX = robotX -1;
				landingX = robotX -2;
				break;
			}
			
			if (((destX >=0) && (destX < Width)) && ((landingX >=0) && (landingX < Width)))
			{}
			else
				return;
			if (((destY >=0) && (destY < Height)) && ((landingY >=0) && (landingY < Height)))
			{}
			else
				return;

			destSquare = mapList[destX][destY]->getType();

			if( ((destSquare == THalfTopR) && ((robotDirection == 0) || (robotDirection == 2))) ||
				((destSquare == THalfTopL) && ((robotDirection == 1) || (robotDirection == 3))) )				
			{
				robotX = landingX;
				robotY = landingY;
				(*oitr)->setXPos(robotX);
				(*oitr)->setYPos(robotY);
				this->RCwillRobotDieStayingHere();
			}
		}				
	}
}

void gameBoard::RCclimb()
{
	// first thing's first, lets find the robot
	std::vector<object*>::iterator oitr = objectList.begin();

	for(;oitr != objectList.end(); oitr++)
	{
		if((*oitr)->getType() == ORobot)
		{
			int robotDirection = (*oitr)->getDirection();
			tileTypeEnum robotSquare = mapList[robotX][robotY]->getType();
			tileTypeEnum destSquare;

			int destX = robotX;
			int destY = robotY;
			int landingX = robotX;
			int landingY = robotY;
			switch(robotDirection)
			{
			case 0:// facing up/right (up on map)
				destY = robotY -1;
				landingY = robotY - 2;
				break;
			case 1:// facing down/right (right on map)
				destX = robotX +1;
				landingX = robotX +2;
				break;
			case 2:// facing down/left (down on map)
				destY = robotY + 1;
				landingY = robotY +2;
				break;
			case 3:// facing up/left (left on map)
				destX = robotX -1;
				landingX = robotX -2;
				break;
			}
			if((destX >= 0) && (destX < Width) && (destY >= 0) && (destY < Height))
			{
				destSquare = mapList[destX][destY]->getType();
			}
			else
				return;

			// first lets see if we're climbing up to a new level
			if(((destSquare == TRaised4) && (robotSquare == TRaised3)) ||
			   ((destSquare == TRaised3) && (robotSquare == TRaised2)) ||
			   ((destSquare == TRaised2) && (robotSquare == TRaised1)) ||
			   ((destSquare == TRaised1) && ( (robotSquare != TRaised1) && (robotSquare != TRaised2) && 
			                                  (robotSquare != TRaised3) && (robotSquare != TRaised4))))
			{
				robotX = destX;
				robotY = destY;
				(*oitr)->setXPos(robotX);
				(*oitr)->setYPos(robotY);
				this->RCwillRobotDieStayingHere();
				return;
			}

			// if we're not climbing up to a new level, lets see if its even possible to climb over the square into a new square
			if (((destX >=0) && (destX < Width)) && ((landingX >=0) && (landingX < Width)))
			{}
			else
				return;
			if (((destY >=0) && (destY < Height)) && ((landingY >=0) && (landingY < Height)))
			{}
			else
				return;

			// now that we can climb over two squares, lets see if what's infront of us can be climbed over

			// halfwalls
			if( ((destSquare == THalfBottomL) && ((robotDirection == 0) || (robotDirection == 2))) ||
				((destSquare == THalfBottomR) && ((robotDirection == 1) || (robotDirection == 3))) )				
			{
				robotX = landingX;
				robotY = landingY;
				(*oitr)->setXPos(robotX);
				(*oitr)->setYPos(robotY);
				this->RCwillRobotDieStayingHere();
			}
		}				
	}
}

void gameBoard::RCpunch()
{
	// first thing's first, lets find the robot
	std::vector<object*>::iterator oitr = objectList.begin();

	for(;oitr != objectList.end(); oitr++)
	{
		if((*oitr)->getType() == ORobot)
		{
			int robotDirection = (*oitr)->getDirection();
			tileTypeEnum robotSquare = mapList[robotX][robotY]->getType();
			tileTypeEnum destSquare;

			int destX = robotX;
			int destY = robotY;
			switch(robotDirection)
			{
			case 0:// facing up/right (up on map)
				destY = robotY -1;					
				break;
			case 1:// facing down/right (right on map)
				destX = robotX +1;
				break;
			case 2:// facing down/left (down on map)
				destY = robotY + 1;
				break;
			case 3:// facing up/left (left on map)
				destX = robotX -1;
				break;
			}

			if( (destX >=0) && (destX < Width))	{}
			else
				return;
			if((destY >=0) && (destY < Height))	{}
			else
				return;

			// at this point the dest square is in the map so we can just go ahead and work with stuff

			destSquare = mapList[destX][destY]->getType();
			
			// first lets check for the standard breakable square 1 square away from us...
			if(destSquare == TBreakable)
			{
				// its breakable!
				// lets break it
				mapList[destX][destY]->setActive(false);

				// by concensus this has been well, disabled
				// and move the robot to this new square
				//robotX = destX;
				//robotY = destY;
				//(*oitr)->setXPos(robotX);
				//(*oitr)->setYPos(robotY);
			}
			// now lets check if its a breakable wall that we're facing the back of
			else if( ((destSquare == TBreakableBL) && (robotDirection == 0)) ||
				     ((destSquare == TBreakableTL) && (robotDirection == 1)) ||
					 ((destSquare == TBreakableTR) && (robotDirection == 2)) ||
					 ((destSquare == TBreakableBR) && (robotDirection == 3)))
			{
				mapList[destX][destY]->setActive(false);
			}
			// or lets also check to see if the square we're standing in is a breakable directional square
			// since our dest's are set, we don't need to set anything additional
			else if (((robotSquare == TBreakableTR) && (robotDirection == 0)) ||
					 ((robotSquare == TBreakableBR) && (robotDirection == 1)) ||
					 ((robotSquare == TBreakableBL) && (robotDirection == 2)) ||
				     ((robotSquare == TBreakableTL) && (robotDirection == 3)))
			{

				mapList[destX][destY]->setActive(false);
				mapList[robotX][robotY]->setActive(false);
			}
		}				
	}
}


void gameBoard::RCactivate()
{
	// find the robot
	vector<object*>::iterator oitr = objectList.begin();
	vector<Oswitch*>::iterator sitr = switchList.begin();
	//Oswitch* tempSwitch;
	tileTypeEnum robotSquare;
	int			 robotDirection;
	tileTypeEnum destType;
	bool		 destActive;

	int destX = 0;
	int destY = 0;
	robotSquare = mapList[robotX][robotY]->getType();
	robotDirection = (*oitr)->getDirection();

	// first lets see if anything happens based upon the square we're standing in....

	// are we standing in a door square facing the door in our own square?
	// if so, lets toggle it
	switch(robotDirection)
	{
	case 0:// facing up/right (up on map)					
		if(robotSquare == TDoorTR)
			mapList[robotX][robotY]->toggleActive();
		break;
	case 1:// facing down/right (right on map)
		if(robotSquare == TDoorBR)
			mapList[robotX][robotY]->toggleActive();
		break;
	case 2:// facing down/left (down on map)
		if(robotSquare == TDoorBL)
			mapList[robotX][robotY]->toggleActive();
		break;
	case 3:// facing up/left (left on map)
		if(robotSquare == TDoorTL)
			mapList[robotX][robotY]->toggleActive();
		break;
	}

	destX = robotX;
	destY = robotY;
	// now lets see if the robot is facing a door				
	switch(robotDirection)
	{
	case 0:// facing up/right (up on map)					

		destY = robotY -1;					
		break;
	case 1:// facing down/right (right on map)
		destX = robotX +1;
		break;
	case 2:// facing down/left (down on map)
		destY = robotY + 1;
		break;
	case 3:// facing up/left (left on map)
		destX = robotX -1;
		break;
	}

	// now lets check if the door that is facing the robot
	if((destX >= 0) && (destX < Width) && (destY >= 0) && (destY < Height))
	{
		destType = (mapList[destX][destY]->getType()) ;
		destActive = (mapList[destX][destY]->getIsActive());

		switch(robotDirection)
		{
		case 0:// facing up/right (up on map)					
			if(destType == TDoorBL)
				mapList[destX][destY]->toggleActive();
			break;
		case 1:// facing down/right (right on map)
			if(destType == TDoorTL)
				mapList[destX][destY]->toggleActive();
			break;
		case 2:// facing down/left (down on map)
			if(destType == TDoorTR)
				mapList[destX][destY]->toggleActive();
			break;
		case 3:// facing up/left (left on map)
			if(destType == TDoorBR)
				mapList[destX][destY]->toggleActive();
			break;
		}
	}

	// are we standing on a switch square?
	sitr = switchList.begin();

	//int sx;
	//int sy;

	if((robotSquare == TSwitch) ||
	   ((robotSquare == TSwitchTR) && (robotDirection == 0)) ||
	   ((robotSquare == TSwitchBR) && (robotDirection == 1)) ||
	   ((robotSquare == TSwitchBL) && (robotDirection == 2)) ||
	   ((robotSquare == TSwitchTL) && (robotDirection == 3)))

	{
		// if we are in fact standing on a switch... lets see if one exists in the switch list
		if((GameVars->SM->isThereASwitchAt(robotX, robotY)) && (!switchInProgress))
		{
			// if there is, lets set the happy go lucky bool in gameboard to true
			switchInProgress = true;
			mapList[robotX][robotY]->toggleActive();
			GameVars->SM->startProcessing(robotX, robotY);
		}

		//////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////
		// current switch system seems to be working, incase any weird bugs crop up
		// i'm leaving this old code here, if no problems are found then all this old 
		// shit needs to be nuked

		//////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////

		//for(;sitr != switchList.end(); sitr++)
		//{
		//	//  find the switch in the object list
		//	if(((*sitr)->getType() == OSwitch) && ((*sitr)->getXPos() == robotX) && ((*sitr)->getYPos() == robotY))
		//	{
		//		// process through its stuff
		//		//tempSwitch = (*sitr);
		//		for(int xyx = 0; xyx < (*sitr)->getNumTargets(); xyx++)
		//		{	
		//			sx = (*sitr)->getNextX();
		//			sy = (*sitr)->getNextY();
		//			currentX = sx;
		//			currentY = sy;

		//			this->recalcPositions();
		//			
		//			// draw before
		//			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//			this->draw();
		//			glutSwapBuffers();

		//			timer = clock();
		//			startTime = clock();
		//			while(timer < startTime + 500)
		//			{
		//				timer = clock();
		//			}
		//			

		//			mapList[sx][sy]->toggleActive();
		//			
		//			
		//			// draw after
		//			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//			this->draw();
		//			glutSwapBuffers();

		//			timer = clock();
		//			startTime = clock();
		//			while(timer < startTime + 500)
		//			{
		//				timer = clock();
		//			}
		//		}
		//	}
		//}
		//////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////
	}

	// now lets see if we're on a reprogrammable square or facing a reprogram wall
	if( (robotSquare == TProgram) ||
		((robotSquare == TProgramTR) && (robotDirection == 0)) ||
		((robotSquare == TProgramBR) && (robotDirection == 1)) ||
		((robotSquare == TProgramBL) && (robotDirection == 2)) ||
		((robotSquare == TProgramTL) && (robotDirection == 3)) )				
	{
		(*oitr)->setDefaults(robotDirection, robotX, robotY);

		// store the map tiles actives
		for(int x = 0; x < Width; x++)
		{
			for(int y = 0; y < Height; y++)
			{
				mapList[x][y]->setResetActive(mapList[x][y]->getIsActive());
			}
		}
		GameVars->totalCommandsProcessed += GameVars->commandsProcessed;
		GameVars->commandsProcessed = 0;
		
		robotStartX = robotX;
		robotStartY = robotY;
		curState = GB_LOGICVIEW;

		// Go ahead and tell the interface it can stop processing...
		mInterfaceReprogramHandler();

		// and clear the instruction list
		mInterfaceClearExecutionList();
		GameVars->setPMStatus(5);
		reprogramHit = true;
	}
}



bool gameBoard::RCmoveRobotForward()	// returns false when its over, true if its still activating
{
	// first thing's first, lets find the robot
	std::vector<object*>::iterator oitr = objectList.begin();

	for(;oitr != objectList.end(); oitr++)
	{
		if((*oitr)->getType() == ORobot)
		{
			int robotDirection = (*oitr)->getDirection();

			// lets see if we are going to die attempting to leave this squiare
			if(this->RCwillRobotDieTryingToLeaveSquare(robotDirection))
			{
				// yes we died
				(*oitr)->setAlive(false);
				robotAlive = false;
				return false;
			}

			// at this point we're still alive, now lets see if we can leave this square
			if(!this->RCcanRobotLeaveSquare(robotDirection))
			{
				// no we can't leave
				return false;
			}

			// now lets see if we can move forward a square
			if(!this->RCcanRobotMoveForward(robotDirection, 1))
			{
				// no we can't move forward
				return false;
			}

			// ok at this point we're not dead, we can leave our square, and we can move forward into the next square
			// lets move forward and see what happens!

			int destX = robotX;
			int destY = robotY;
			switch(robotDirection)
			{
			case 0:// facing up/right (up on map)
				destY = robotY -1;					
				break;
			case 1:// facing down/right (right on map)
				destX = robotX +1;
				break;
			case 2:// facing down/left (down on map)
				destY = robotY + 1;
				break;
			case 3:// facing up/left (left on map)
				destX = robotX -1;
				break;
			}
			if( (destX >=0) && (destX < Width) )
				robotX = destX;
			if( (destY >=0) && (destY < Height) )
				robotY = destY;

			(*oitr)->setXPos(robotX);
			(*oitr)->setYPos(robotY);
			
			// now that we've moved forward
			// lets see if we're about to die
			if(this->RCwillRobotDieStayingHere())
			{
				// yes we died
				(*oitr)->setAlive(false);
				robotAlive = false;
				 return false;
			}
		}	
	}
	return true;
}

void gameBoard::setState(GameBoardState state)
{
	curState = state;
}

bool gameBoard::processSub(int whichSub)
{
	//=======================
	// Subroutines processing
	// other subs does not work...
	// Right now we're designing the system
	// so that it won't allow these kinds of
	// algorithms but this will have to be fixed
	// in the future.
	processSubRecurCounter++;
	//if(processSubRecurCounter > 32)
	//{
	//	processSubRecurCounter--;
	//	return false;
	//}
	//std::cout << "Processing..." << endl;
	AiInstructions nextCommand;
	bool parentDelay = false;
	bool moveForwardFiring = false;
	

	if(whichSub == 1)
		if(SUB1->isEmpty())
		{
			/*processSubRecurCounter--;*/
			return false;
		}
		else
		{
			nextCommand = SUB1->getNextCommand();
		}

	if(whichSub == 2)
		if(SUB2->isEmpty())
		{
			/*processSubRecurCounter--;*/
			return false;
		}
		else
		{
			nextCommand = SUB2->getNextCommand();
		}

	std::vector<object*>::iterator oitr = objectList.begin();

	for(;oitr != objectList.end(); oitr++)	// finding the robot
	{
		if((*oitr)->getType() == ORobot)
		{
			switch(nextCommand)
			{
			case MOVE_FORWARD1:
				GameVars->commandsProcessed++;
				this->RCmoveRobotForward();
				// special case for ice squares
				if(mapList[robotX][robotY]->getType() == TIce)
				{
					if(this->RCcanRobotMoveForward((*oitr)->getDirection(), 1))
					{
						GameVars->commandsProcessed--;
						parentDelay = true;
						/*processSubRecurCounter--;*/
						return true;
					}
				}
				break;
			case TURN_LEFT1:
				(*oitr)->rotate(-1);
				GameVars->commandsProcessed++;
				break;
			case TURN_RIGHT1:
				(*oitr)->rotate(1);
				GameVars->commandsProcessed++;
				break;
			case CROUCH:  // just like move forward above, only far less squares that can be moved into
				this->RCcrouch();
				GameVars->commandsProcessed++;
				break;
			case CLIMB:	  // just like move forward above, only far less squares that can be moved into
				this->RCclimb();
				GameVars->commandsProcessed++;
				break;
			case JUMP:	  // just like move forward above, only far less squares that can be moved into
				this->RCjumpRobotForward();
				GameVars->commandsProcessed++;
				break;
			case PUNCH:	  // just like move forward above, only far less squares that can be moved into
				this->RCpunch();
				GameVars->commandsProcessed++;
				break;
			case SUB:	// special case
				break;
			case LOOP:	// since this should always be displayed as the last command automatically, this isn't really needed
						// the object's ai list automagically loops when it gets to its end
				break;
			case ACTIVATE:	// for now lets just check for a door so we can see it working in testmap1
				this->RCactivate();
				GameVars->commandsProcessed++;
				break;

			case MOVE_FORWARD_UNTIL_UNABLE: // just like move forward above, only no advancement of command until destination square is invalid
				moveForwardFiring = this->RCmoveRobotForward();	// false if over, true if activating
				GameVars->commandsProcessed++;
				if(moveForwardFiring)
				{
					GameVars->commandsProcessed--;
					/*processSubRecurCounter--;*/
					return true;
				}				
				break;

			case SUBR1:
				parentDelay = processSub(1);
				return parentDelay;
				break;

			case SUBR2:
				parentDelay = processSub(2);
				return parentDelay;
				break;

			case DO_NOT_PROCESS:
				break;
			case STOP:
				GameVars->commandsProcessed--;
				return true;
			default:
				break;
			}
		}
	}

	if(whichSub == 1)
		parentDelay = SUB1->advanceCommand();
		
	if(whichSub == 2)
		parentDelay = SUB2->advanceCommand();

	if(this->robotAtEndSquare())
	{
		// stupid parent is going to subtract one because we're in a subroutine, that's no good
		GameVars->commandsProcessed++;
	}

	/*processSubRecurCounter--;*/
	return parentDelay;
}

void gameBoard::SetInterfaceAdvanceHandler(CFunctionPointer2R<bool, instructionTab, logicBlock*> interfaceAdvanceHandler)
{
	mInterfaceAdvanceHandler = interfaceAdvanceHandler;
}

void gameBoard::SetInterfaceReprogramHandler(CFunctionPointer0R<bool> interfaceReprogramHandler)
{
	mInterfaceReprogramHandler = interfaceReprogramHandler;
}

void gameBoard::SetInterfaceClearExecutionListHandler(CFunctionPointer0R<bool> InterfaceClearExecutionList)
{
	mInterfaceClearExecutionList = InterfaceClearExecutionList;
}


void gameBoard::panleft()
{	
	recalcPositions();
	currentX -= moveSpeed*2;
	currentY += moveSpeed*2;
	verifyCameraPositionX();
	verifyCameraPositionY();
	verifyCameraCenter();
}

void gameBoard::panright()
{
	recalcPositions();
	currentX += moveSpeed*2;
	currentY -= moveSpeed*2;
	verifyCameraPositionX();
	verifyCameraPositionY();
	verifyCameraCenter();
}

void gameBoard::panup()
{
	recalcPositions();
	currentX -= moveSpeed*2;
	currentY -= moveSpeed*2;
	verifyCameraPositionX();
	verifyCameraPositionY();
	verifyCameraCenter();
}

void gameBoard::pandown()
{
	recalcPositions();
	currentX += moveSpeed*2;
	currentY += moveSpeed*2;
	verifyCameraPositionX();
	verifyCameraPositionY();
	verifyCameraCenter();
}
void gameBoard::panupleft()
{
	recalcPositions();
	currentX -= moveSpeed*2;
	verifyCameraPositionX();
	verifyCameraCenter();
}
void gameBoard::panupright()
{
	recalcPositions();
	currentY -= moveSpeed*2;
	verifyCameraPositionY();
	verifyCameraCenter();
}
void gameBoard::pandownleft()
{
	recalcPositions();
	currentY += moveSpeed*2;
	verifyCameraPositionY();
	verifyCameraCenter();
}
void gameBoard::pandownright()
{
	recalcPositions();
	currentX += moveSpeed*2;
	verifyCameraPositionX();
	verifyCameraCenter();
}
void gameBoard::zoomout()
{
	scale -= 0.05;
	if (scale < minscale)	scale = minscale;
}
void gameBoard::zoomin()
{
	scale += 0.05;
	if (scale > maxscale)	scale = maxscale;
}
void gameBoard::center()
{
	currentX = robotX;
	currentY = robotY;
}
bool gameBoard::getRobotAlive()
{
	return robotAlive;
}

void gameBoard::spinRobot()
{
	vector<object*>::iterator oitr;
	oitr = objectList.begin();
	for(;oitr != objectList.end(); oitr++)
	{
		if((*oitr)->getType() == ORobot)
		{
			(*oitr)->rotate(1);				
		}
	}	
}

void gameBoard::rotateMapRight()
{
	mapRotation++;
	std::vector<std::vector<mapTile*>> mapListTemp;

	int tempWidth = Height;
	int tempHeight = Width;

	// make it all empty
	mapListTemp.resize(tempWidth);
	for(int x = 0; x < tempWidth; x++)
	{
		mapListTemp[x].resize(tempHeight);
	}
	bool foundRobot = false;

	// now that we have an empty vector
	for(int x = 0; x < tempWidth; x++)
	{
		for(int y = 0; y < tempHeight; y++)
		{
			int newY = Height-x-1;
			mapListTemp[x][y] = mapList[y][newY];
			// tile comparisons go here
			// i.e.
			// half top
			if		(mapListTemp[x][y]->getType() == THalfTopL)		{	mapListTemp[x][y]->setType(THalfTopR);		}
			else if	(mapListTemp[x][y]->getType() == THalfTopR)		{	mapListTemp[x][y]->setType(THalfTopL);		}
			// half bottom
			else if (mapListTemp[x][y]->getType() == THalfBottomR)	{	mapListTemp[x][y]->setType(THalfBottomL);	}
			else if (mapListTemp[x][y]->getType() == THalfBottomL)	{	mapListTemp[x][y]->setType(THalfBottomR);	}
			// electric
			else if	(mapListTemp[x][y]->getType() == TElectricTL)	{	mapListTemp[x][y]->setType(TElectricTR);	}
			else if (mapListTemp[x][y]->getType() == TElectricTR)	{	mapListTemp[x][y]->setType(TElectricBR);	}
			else if (mapListTemp[x][y]->getType() == TElectricBR)	{	mapListTemp[x][y]->setType(TElectricBL);	}
			else if (mapListTemp[x][y]->getType() == TElectricBL)	{	mapListTemp[x][y]->setType(TElectricTL);	}
			// switches
			else if	(mapListTemp[x][y]->getType() == TSwitchTL)		{	mapListTemp[x][y]->setType(TSwitchTR);		}
			else if	(mapListTemp[x][y]->getType() == TSwitchTR)		{	mapListTemp[x][y]->setType(TSwitchBR);		}
			else if	(mapListTemp[x][y]->getType() == TSwitchBR)		{	mapListTemp[x][y]->setType(TSwitchBL);		}
			else if	(mapListTemp[x][y]->getType() == TSwitchBL)		{	mapListTemp[x][y]->setType(TSwitchTL);		}
			// reprogram squares
			else if	(mapListTemp[x][y]->getType() == TProgramTL)	{	mapListTemp[x][y]->setType(TProgramTR);		}
			else if	(mapListTemp[x][y]->getType() == TProgramTR)	{	mapListTemp[x][y]->setType(TProgramBR);		}
			else if	(mapListTemp[x][y]->getType() == TProgramBR)	{	mapListTemp[x][y]->setType(TProgramBL);		}
			else if	(mapListTemp[x][y]->getType() == TProgramBL)	{	mapListTemp[x][y]->setType(TProgramTL);		}
			// breakables
			else if	(mapListTemp[x][y]->getType() == TBreakableTL)	{	mapListTemp[x][y]->setType(TBreakableTR);	}
			else if	(mapListTemp[x][y]->getType() == TBreakableTR)	{	mapListTemp[x][y]->setType(TBreakableBR);	}
			else if	(mapListTemp[x][y]->getType() == TBreakableBR)	{	mapListTemp[x][y]->setType(TBreakableBL);	}
			else if	(mapListTemp[x][y]->getType() == TBreakableBL)	{	mapListTemp[x][y]->setType(TBreakableTL);	}
			// doors
			else if	(mapListTemp[x][y]->getType() == TDoorTL)		{	mapListTemp[x][y]->setType(TDoorTR);		}
			else if	(mapListTemp[x][y]->getType() == TDoorTR)		{	mapListTemp[x][y]->setType(TDoorBR);		}
			else if	(mapListTemp[x][y]->getType() == TDoorBR)		{	mapListTemp[x][y]->setType(TDoorBL);		}
			else if	(mapListTemp[x][y]->getType() == TDoorBL)		{	mapListTemp[x][y]->setType(TDoorTL);		}

			if((robotX == y) && (robotY == (newY)) && (!foundRobot))
			{
				foundRobot = true;
				robotX = x;
				robotY = y;
				vector<object*>::iterator oitr;
				oitr = objectList.begin();
				for(;oitr != objectList.end(); oitr++)
				{
					if((*oitr)->getType() == ORobot)
					{
						(*oitr)->rotate(1);	
					}
				}
			}
		}
	}

	double tempX, tempY;

	tempX = Height - currentY;
	tempY = currentX;

	currentX = tempX;
	currentY = tempY;


	// now for switches
	GameVars->SM->rotateRight(Width, Height);
	//vector<Oswitch*>::iterator sitr = switchList.begin();

	int tempTargetX;
	int tempTargetY;
	//int numControlled;
	//for(;sitr < switchList.end(); sitr++)
	//{
	//	// rotate the switch
	//	tempX = (*sitr)->getXPos();
	//	tempY = (*sitr)->getYPos();
	//	(*sitr)->setXPos(Height - tempY-1);
	//	(*sitr)->setYPos(tempX);

	//	numControlled = (*sitr)->getNumTargets();
	//	// now rotate the targets
	//	for(int x = 0; x < numControlled; x++)
	//	{
	//		tempTargetX = (*sitr)->getTargetX();
	//		tempTargetY = (*sitr)->getTargetY();
	//		(*sitr)->setTargetX(Height - tempTargetY - 1);
	//		(*sitr)->setTargetY(tempTargetX);
	//		(*sitr)->cycleTargets();
	//	}
	//}

	// now for teleporters
	std::vector<Oteleport*>::iterator titr = teleportList.begin();

	for(; titr < teleportList.end(); titr++)
	{
		tempX = (*titr)->getXPos();
		tempY = (*titr)->getYPos();
		tempTargetX = (*titr)->getTargetX();
		tempTargetY = (*titr)->getTargetY();
		(*titr)->setXPos(Height - tempY - 1);
		(*titr)->setYPos(tempX);
		(*titr)->setTarget(Height - tempTargetY - 1, tempTargetX);
	}

	recalcPositions();

	Width = tempWidth;
	Height = tempHeight;
	mapList = mapListTemp;
}

void gameBoard::rotateMapLeft()
{
	mapRotation--;
	std::vector<std::vector<mapTile*>> mapListTemp;

	int tempWidth = Height;
	int tempHeight = Width;

	// make it all empty
	mapListTemp.resize(tempWidth);
	for(int x = 0; x < tempWidth; x++)
	{
		mapListTemp[x].resize(tempHeight);
	}
	bool foundRobot = false;

	// now that we have an empty vector
	for(int x = 0; x < tempWidth; x++)
	{
		for(int y = 0; y < tempHeight; y++)
		{
			mapListTemp[x][y] = mapList[Width-y-1][x];

			// tile comparisons go here
			// i.e.
			// half top
			if		(mapListTemp[x][y]->getType() == THalfTopL)		{	mapListTemp[x][y]->setType(THalfTopR);		}
			else if	(mapListTemp[x][y]->getType() == THalfTopR)		{	mapListTemp[x][y]->setType(THalfTopL);		}
			// half bottom
			else if (mapListTemp[x][y]->getType() == THalfBottomR)	{	mapListTemp[x][y]->setType(THalfBottomL);	}
			else if (mapListTemp[x][y]->getType() == THalfBottomL)	{	mapListTemp[x][y]->setType(THalfBottomR);	}
			// electric
			else if	(mapListTemp[x][y]->getType() == TElectricTL)	{	mapListTemp[x][y]->setType(TElectricBL);	}
			else if (mapListTemp[x][y]->getType() == TElectricBL)	{	mapListTemp[x][y]->setType(TElectricBR);	}
			else if (mapListTemp[x][y]->getType() == TElectricBR)	{	mapListTemp[x][y]->setType(TElectricTR);	}
			else if (mapListTemp[x][y]->getType() == TElectricTR)	{	mapListTemp[x][y]->setType(TElectricTL);	}
			// switches
			else if	(mapListTemp[x][y]->getType() == TSwitchTL)		{	mapListTemp[x][y]->setType(TSwitchBL);		}
			else if	(mapListTemp[x][y]->getType() == TSwitchBL)		{	mapListTemp[x][y]->setType(TSwitchBR);		}
			else if	(mapListTemp[x][y]->getType() == TSwitchBR)		{	mapListTemp[x][y]->setType(TSwitchTR);		}
			else if	(mapListTemp[x][y]->getType() == TSwitchTR)		{	mapListTemp[x][y]->setType(TSwitchTL);		}
			// reprogram squares
			else if	(mapListTemp[x][y]->getType() == TProgramTL)	{	mapListTemp[x][y]->setType(TProgramBL);		}
			else if	(mapListTemp[x][y]->getType() == TProgramBL)	{	mapListTemp[x][y]->setType(TProgramBR);		}
			else if	(mapListTemp[x][y]->getType() == TProgramBR)	{	mapListTemp[x][y]->setType(TProgramTR);		}
			else if	(mapListTemp[x][y]->getType() == TProgramTR)	{	mapListTemp[x][y]->setType(TProgramTL);		}
			// breakables
			else if	(mapListTemp[x][y]->getType() == TBreakableTL)	{	mapListTemp[x][y]->setType(TBreakableBL);	}
			else if	(mapListTemp[x][y]->getType() == TBreakableBL)	{	mapListTemp[x][y]->setType(TBreakableBR);	}
			else if	(mapListTemp[x][y]->getType() == TBreakableBR)	{	mapListTemp[x][y]->setType(TBreakableTR);	}
			else if	(mapListTemp[x][y]->getType() == TBreakableTR)	{	mapListTemp[x][y]->setType(TBreakableTL);	}
			// doors
			else if	(mapListTemp[x][y]->getType() == TDoorTL)		{	mapListTemp[x][y]->setType(TDoorBL);		}
			else if	(mapListTemp[x][y]->getType() == TDoorBL)		{	mapListTemp[x][y]->setType(TDoorBR);		}
			else if	(mapListTemp[x][y]->getType() == TDoorBR)		{	mapListTemp[x][y]->setType(TDoorTR);		}
			else if	(mapListTemp[x][y]->getType() == TDoorTR)		{	mapListTemp[x][y]->setType(TDoorTL);		}

			if((robotX == (Width-y-1)) && (robotY == x) && (!foundRobot))
			{
				foundRobot = true;
				robotX = x;
				robotY = y;
				vector<object*>::iterator oitr;
				oitr = objectList.begin();
				for(;oitr != objectList.end(); oitr++)
				{
					if((*oitr)->getType() == ORobot)
					{
						(*oitr)->rotate(-1);				
					}
				}
			}
		}
	}


	
	double tempX, tempY;

	tempX = currentY;
	tempY = Width - currentX;

	currentX = tempX;
	currentY = tempY;

	int tempTargetX;
	int tempTargetY;

	// now for switches
	GameVars->SM->rotateLeft(Width, Height);
	//vector<Oswitch*>::iterator sitr = switchList.begin();

	//int numControlled;
	//for(;sitr < switchList.end(); sitr++)
	//{
	//	// rotate the switch
	//	tempX = (*sitr)->getXPos();
	//	tempY = (*sitr)->getYPos();
	//	(*sitr)->setXPos(tempY);
	//	(*sitr)->setYPos(Width-tempX-1);

	//	numControlled = (*sitr)->getNumTargets();
	//	// now rotate the targets
	//	for(int x = 0; x < numControlled; x++)
	//	{
	//		tempTargetX = (*sitr)->getTargetX();
	//		tempTargetY = (*sitr)->getTargetY();
	//		(*sitr)->setTargetX(tempTargetY);
	//		(*sitr)->setTargetY(Width-tempTargetX-1);
	//		(*sitr)->cycleTargets();
	//	}
	//}

	// now for teleporters
	std::vector<Oteleport*>::iterator titr = teleportList.begin();

	for(; titr < teleportList.end(); titr++)
	{
		tempX = (*titr)->getXPos();
		tempY = (*titr)->getYPos();
		tempTargetX = (*titr)->getTargetX();
		tempTargetY = (*titr)->getTargetY();
		(*titr)->setXPos(tempY);
		(*titr)->setYPos(Width-tempX-1);
		(*titr)->setTarget(tempTargetY, Width-tempTargetX -1);
	}

	recalcPositions();

	Width = tempWidth;
	Height = tempHeight;
	mapList = mapListTemp;
}


bool gameBoard::verifyCameraCenter()
{
	// this function should be run after other functions, it effectively checks
	// to see if the board should be centered in any direction

	// with the new camera centering, i'm not sure if this is really even needed
	// it doesn't seem to be breaking anything soo... i'm not going to touch it

	//recalcPositions();
	//int centerpointX = (int)screenWidth/2;
	//int centerpointY = (int)(screenHeight - interfaceHeight)/2;

	//// now lets see if this board should be centered or not
	//if(overallWidth < screenWidth)
	//{
	//	// center horizontally
	//	mapOffsetX = centerpointX - (int)overallWidth/2;
	//}
	//
	//if(overallHeight < screenHeight)
	//{
	//	// center vertically
	//	mapOffsetY = centerpointY - (int)overallHeight/2;
	//}
	//recalcPositions();
	return true;
}

void gameBoard::recalcPositions()
{	
	// recalculate image width/height with current scale
	imageWidth = imageBaseWidth * scale;
	imageHeight = imageBaseHeight * scale;

	// recalc half width/height
	hw = imageWidth/2;
	hh = imageHeight/2;

	// recalc overall width and height
	overallWidth = (((Height + Width) * hw) + hw);
	overallHeight = (((Height + Width) * hh) + hh);

	//moveSpeed = scale * 0.1;	

	// setting offsetx based on camera center spot
	// start at the center of the screen
	// if the screen is not centering properly, look here
	mapOffsetX = (screenWidth/2) + (hw*currentY) - (hw*currentX) - hw*2;
	mapOffsetY = (screenHeight/2)- (hh*currentY) - (hh*currentX) - hh;
}

bool gameBoard::verifyCameraPositionX()
{
	// if they have moved the camera beyond the gameboard
	// return a failure
	//std::cout << "Camera X = " << currentX << endl;
	if(currentX > Width)	
	{
		currentX = Width;
		return false;
	}
	else if(currentX < 0)
	{
		currentX = 0;
		return false;
	}
	return true;
}
bool gameBoard::verifyCameraPositionY()
{
	// if they have moved the camera beyond the gameboard
	// return a failure
	//std::cout << "Camera Y = " << currentY << endl;
	if(currentY > Height)	
	{
		currentY = Height;
		return false;
	}
	else if(currentY < 0)
	{
		currentY = 0;	
		return false;
	}
	return true;
}

void gameBoard::outputSwitchInfo()
{
	vector<Oswitch*>::iterator sitr = switchList.begin();
	// process through all switches

	int counter = 0;
	for(;sitr < switchList.end(); sitr++)
	{
		counter++;
		std::cout << "Switch #" << counter << " is at " << (*sitr)->getXPos() << ", " << (*sitr)->getYPos() << " and controls " << (*sitr)->getNumTargets() << " tiles." << endl;
		// for the switch, process through what's controlled by the switches
		for(int x = 0; x < (*sitr)->getNumTargets(); x++)
		{
			std::cout << "Target #" << x << " is at " << (*sitr)->getNextX() << ", " << (*sitr)->getNextY() << endl;
		}
	}

}

void gameBoard::processSwitch()
{
	// this function should NEVER be called other than by the update code
	// switches adjust at the same rate that the robot moves
	// all this code needs to do is change the currentx/y to match a switch
	// then toggle the switch
	// two cycles per switch, one pre, one post
	
	currentX = GameVars->SM->getCurrentTargetX(robotX, robotY);
	currentY = GameVars->SM->getCurrentTargetY(robotX, robotY);
	
	if(!switchToggled)
	{
		// since this is the pre state, we just need to find out where this switch's next target is and set currentx/y to it
		
		switchToggled = true;
	}
	else
	{
		mapList[GameVars->SM->getCurrentTargetX(robotX, robotY)][GameVars->SM->getCurrentTargetY(robotX, robotY)]->toggleActive();
		GameVars->SM->advanceTarget(robotX, robotY);
		switchToggled = false;
		// now in this state, we need to toggle it one single time and set the currentx/y to our position

	}
	if(GameVars->SM->doneProcessing())
	{
		switchInProgress = false;
	}
}

bool gameBoard::getReprogramHit()
{
	return reprogramHit;
}

void gameBoard::setReprogramHit(bool newStatus)
{
	reprogramHit = newStatus;
}

void gameBoard::resetZoom()
{
	scale = 1.0;
}