#include "MenuSystem.h"
#include <iostream>
#include "GameStateManager.h"
#include <cstring>

//constructor
MenuSys::MenuSys(int xpos, int ypos, string imgname, Justification nbuttonJust, bool nclickAndHold)
: menuImage(NULL), MOUSE_LEFT_BUTTON_STATE(GLUT_UP), timer(0), startTime(0), 
	buttonDelay(500)
{
	menuXPos = xpos;
	menuYPos = ypos;
	menuImage = new oglTexture2D();
	if(menuImage != NULL)
		menuImage->loadImage(imgname.c_str(), 600, 475);
	menuWidth = 600;
	menuHeight = 475;
	menuImage->mX = xpos;
	menuImage->mY = ypos;
	numButtons = 0;
	buttonJust = nbuttonJust;
	clickAndHold = nclickAndHold;
}

MenuSys::MenuSys(int xpos, int ypos, int nWidth, int nHeight, string imgname, Justification nbuttonJust, bool nclickAndHold)
: menuImage(NULL), MOUSE_LEFT_BUTTON_STATE(GLUT_UP), timer(0), startTime(0), buttonDelay(500)
{
	menuXPos = xpos;
	menuYPos = ypos;
	menuImage = new oglTexture2D();
	if(menuImage != NULL)
		menuImage->loadImage(imgname.c_str(), nWidth, nHeight);
	menuImage->mX = xpos;
	menuImage->mY = ypos;
	menuWidth = nWidth;
	menuHeight = nHeight;
	numButtons = 0;
	buttonJust = nbuttonJust;
	clickAndHold = nclickAndHold;
}

MenuSys::MenuSys()
{
	menuXPos = 0;
	menuYPos = 0;
	menuImage = NULL;
	numButtons = 0;
	buttonJust = None;
}

//destructor
MenuSys::~MenuSys()
{
	vector<Button*>::iterator itr = buttonList.begin();

	// delete buttonlist contents
	for (; itr != buttonList.end(); itr++)
    {
		(*itr)->~Button();
    }
	buttonList.clear();
	delete menuImage;
}

//functions that dont do anything, yey!
bool MenuSys::Initialize()
{
	return true;
}

bool MenuSys::Draw()
{
	// draw myself first, then draw the buttons
	if(menuImage != NULL)
		menuImage->drawImage();

	vector<Button*>::iterator itr = buttonList.begin();

	// draw buttonlist
	for (; itr != buttonList.end(); itr++)
    {
		(*itr)->Draw();
    }
	return true;
}

int MenuSys::getXPos()
{
	return menuXPos;
}

int MenuSys::getYPos()
{
	return menuYPos;
}

int MenuSys::getMouseXPos()
{
	return mouseXPos;
}

int MenuSys::getMouseYPos()
{
	return mouseYPos;
}

bool MenuSys::Remove()
{
	return true;
}

bool MenuSys::Update()
{
	// any menu maitenance goes here
	vector<Button*>::iterator itr = buttonList.begin();

	// update buttonlist
	for (; itr != buttonList.end(); itr++)
    {
		(*itr)->Update();
    }

	if(clickAndHold)
	{
		// For clicking and holding a button, we're gonna
		// want to fire the buttons click event over and 
		// over
		if (MOUSE_LEFT_BUTTON_STATE == GLUT_DOWN)
		{
			if(startTime != 0)
			{
				timer += clock() - startTime;
				if(timer >= buttonDelay)
				{
					buttonClick();
					timer = 0;
					startTime = clock();
				}
			}
		}
	}
	return true;
}

void MenuSys::setMenuXPos(int position)
{
	menuXPos = position;
}

void MenuSys::setMenuYPos(int position)
{
	menuYPos = position;
}
void MenuSys::setMenuWidth(int nWidth)
{
	menuWidth = nWidth;
}
void MenuSys::setMenuHeight(int nHeight)
{
	menuHeight = nHeight;
}

void MenuSys::setLastButtonPosition(int x, int y)
{
	vector<Button*>::iterator itr = buttonList.end();
	itr--;

	if((*itr) != NULL)
	{
		//(*itr)->setPosition(x, y);
		(*itr)->setButtonXYPos(x, y);
	}
}

void MenuSys::setLastButtonDimensions(int width, int height)
{
	vector<Button*>::iterator itr = buttonList.end();
	itr--;

	if((*itr) != NULL)
	{
		(*itr)->setDimensions(width, height);
	}
}

void MenuSys::setLastButtonHoverHandler(CFunctionPointer0R<bool> clickHoverHandler)
{
	if(clickHoverHandler)
	{
		vector<Button*>::iterator itr = buttonList.end();
		itr--;

		if((*itr) != NULL)
		{
			(*itr)->setHoverHandler(clickHoverHandler);
		}
	}
}

void MenuSys::keyboardInput(unsigned char c, int x, int y)
{
	if (c == 27) 
		exit(0);
}

void MenuSys::processMouse(int x, int y)
{
	setMousePos(x, y);
	
	// The mouse has moved, so we don't want to
	// save any state information regarding the
	// button being pressed (we don't want the
	// user to be able to move their mouse around
	// and activate multiple buttons)
	MOUSE_LEFT_BUTTON_STATE = GLUT_UP;
	startTime = 0;
	timer = 0;


#ifdef mouseWork
	std::cout << "MOUSE POSITION:: " << x << ", " << y << endl;
#endif

	// process buttons with mouse coords
	vector<Button*>::iterator itr = buttonList.begin();

	// update buttonlist
	for (; itr != buttonList.end(); itr++)
    {
		if( (*itr)->checkInBounds(x, y) )
		{
			(*itr)->setButtonStatus(Hover);
		}
		else
		{
			(*itr)->setButtonStatus(Normal);
		}
    }


	/*if (button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
	{
		setLeftButtonStatus(true);
		#ifdef mouseWork
		std::cout<<"Left button Pressed" << endl;
		#endif
	}
	if (button==GLUT_LEFT_BUTTON && state==GLUT_UP)
	{
		setLeftButtonStatus(false);
		#ifdef mouseWork
		std::cout<<"Left button not Pressed" << endl;
		#endif
	}

	if (button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
	{
		setRightButtonStatus(true);
		#ifdef mouseWork
		std::cout<<"right button Pressed" << endl;
		#endif
	}
	if (button==GLUT_LEFT_BUTTON && state==GLUT_UP)
	{
		setRightButtonStatus(false);
		#ifdef mouseWork
		std::cout<<"right button not Pressed" << endl;
		#endif
	}*/
}

void MenuSys::processMouseClick(int button, int state, int x, int y)
{
	// Save the current state of the button...
	if (button == GLUT_LEFT_BUTTON)
	{
		MOUSE_LEFT_BUTTON_STATE = state;
		startTime = clock();
	}
	else
	{
		startTime = 0;
		timer = 0;
	}

	if (button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
	{
		buttonClick();
	}
}

///===========================
// Functionalized this block,
// moving it from processMouseClick
// so that it can be called from multiple places easier.
void MenuSys::buttonClick()
{
	vector<Button*>::iterator itr = buttonList.begin();
	for(; itr != buttonList.end(); itr++)
	{
		int mX = (*itr)->getXPos();
		int mY = (*itr)->getYPos();
		if((*itr)->checkInBounds(mouseXPos, mouseYPos))
		{
			(*itr)->callClickHandler();
		}
	}
}

void MenuSys::setMousePos(int x, int y)
{
	mouseXPos = x;
	mouseYPos = y;
}

void MenuSys::setLeftButtonStatus(bool left)
{
	leftButtonDown = left;
}

void MenuSys::setRightButtonStatus(bool right)
{
	rightButtonDown = right;
}

void MenuSys::recalcButtonPositions()
{
	if((buttonJust == Auto) && (menuImage != NULL))
	{
		if(numButtons > 0)
		{
			double tempheight = 0;
			double offsety = 0;
			double totalheight = 0;
			double currentspot = 0;

			for(int x = 0; x < numButtons; x++)
			{
				totalheight += buttonList[x]->getHeight();			
			}
			//tempheight = menuImage->height - totalheight;
			//*** need to double check on if its dy, should be height
			tempheight = menuImage->dY - totalheight;
			
			offsety = (tempheight / numButtons);
			currentspot = offsety/2;

			currentspot += menuImage->mY;
			for(int x = 0; x < numButtons; x++)
			{
				buttonList[x]->setButtonYPos(currentspot);
				//buttonList[x]->setButtonXPos((((menuImage->mX + menuImage->width + menuImage->mX) / 2) - (buttonList[x]->getWidth()/2)));
				//*** need to double check on if its dx, should be width
				buttonList[x]->setButtonXPos((((menuImage->mX + menuImage->dX + menuImage->mX) / 2) - (buttonList[x]->getWidth()/2)));
				currentspot += buttonList[x]->getHeight();
				currentspot += offsety;
			}
		}
	}
}