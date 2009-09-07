#ifndef MENUSYS_H
#define MENUSYS_H

///      Hey guys. When I moved to put a MenuSys pointer into the base GameState
/// it fucked everything up because MenuSystem.h and Button.h both had 
/// references to GameState.h which means we put the compiler in a nice little
/// infinite loop xD.
///      I moved the definition for the "State" enum into
/// oglUtility for now. I know it's not the best fix but I think that it
/// will work for everything we need it to. -Corwin

#include "oglUtility.h"
//#include "pixmap.h"
#include "Button.h"
#include "consoleDefines.h"
#include "oglTexture2D.h"
#include <cstring>
#include <vector>
#include <iostream>
#include <ctime>

using namespace std;

enum Justification{ 
				None,		// no justification - use default values
				Auto		// automatically justified
};


class MenuSys
{
protected:
    int menuHeight;
    int menuWidth;
	int menuXPos;
	int menuYPos;
	int numButtons;
	Justification buttonJust;

	bool leftButtonDown;
	bool rightButtonDown;
	int mouseXPos;
	int mouseYPos;

	bool clickAndHold;

	// each mouse-button can have it's own state,
	// so ideally we should be retaining a state
	// for each button.
	int MOUSE_LEFT_BUTTON_STATE;

	// Timer jazz for button holding, wee.
	clock_t timer;
	clock_t startTime;
	int buttonDelay;

	oglTexture2D* menuImage;
	vector<Button*> buttonList;

public:
	MenuSys(int xpos, int ypos, string imgname, Justification nbuttonJust = Auto, bool nclickAndHold = false);
	MenuSys(int xpos, int ypos, int nWidth = 500, int nHeight = 400, string imgname = "", Justification nbuttonJust = Auto, bool nclickAndHold = false);
	MenuSys();
	~MenuSys();

	//template <class T>
	inline bool addButton(string buttonImageNormal = "normal.png", string buttonImageClicked = "clicked.png", string buttonImageHover = "hover.png", int buttonXpos = 0, int buttonYpos = 0)
	{
		numButtons++;
		Button* newButton = new Button(buttonXpos, buttonYpos, buttonImageNormal, buttonImageClicked, buttonImageHover);  // this needs to be implemented for button constructor

		buttonList.push_back(newButton);
		recalcButtonPositions();
		return true;
		// if something bad happens, return false
	}

	inline bool addButton(string buttonImageNormal, string buttonImageClicked, string buttonImageHover, CFunctionPointer0R<bool> clickEventHandler, int buttonXpos = 0, int buttonYpos = 0, int buttonWidth = 50, int buttonHeight = 50)
	{
		numButtons++;
		Button* newButton = new Button(0, 0, buttonImageNormal, buttonImageClicked, buttonImageHover);  // this needs to be implemented for button constructor
		newButton->setButtonXYPos(buttonXpos, buttonYpos);
		if(buttonJust == Auto)
		{
			buttonWidth = (int)(menuWidth * 0.9);
			buttonHeight = (int)(menuHeight * 0.1);
		}
		newButton->setDimensions(buttonWidth, buttonHeight);
		
		// Click handler (callback / function pointer)
		if(clickEventHandler)
			newButton->setClickHandler(clickEventHandler);

		buttonList.push_back(newButton);
		recalcButtonPositions();
		return true;
	}

	virtual void recalcButtonPositions();
	virtual void setMenuXPos(int position);
	virtual void setMenuYPos(int position);
	virtual void setMenuWidth(int nWidth);
	virtual void setMenuHeight(int nHeight);
	virtual void setLastButtonPosition(int x, int y);
	virtual void setLastButtonDimensions(int width, int height);
	virtual void setLastButtonHoverHandler(CFunctionPointer0R<bool> clickHoverHandler);
	virtual int getXPos();
	virtual int getYPos();
	virtual int getMouseXPos();
	virtual int getMouseYPos();
	virtual void setMousePos(int x, int y);
	virtual void processMouse(int x, int y);
	virtual void processMouseClick(int button, int state, int x, int y);
	virtual void setLeftButtonStatus(bool);
	virtual void setRightButtonStatus(bool);
	virtual bool Initialize();
	virtual bool Update();
    virtual bool Draw();
	virtual bool Remove();
	virtual void keyboardInput(unsigned char c, int x, int y);

private:
	void buttonClick();
};

#endif