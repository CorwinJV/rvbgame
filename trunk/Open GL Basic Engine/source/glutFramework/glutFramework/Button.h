#ifndef BUTTONS_H
#define BUTTONS_H

#include "oglUtility.h"
//#include "pixmap.h"
#include "FunctionPointer.h"
#include <cstring>
#include <iostream>
#include "oglTexture2D.h"

using namespace BE;
//class Button; 

enum buttonState {
		Normal,
		Hover,
		Clicked
};

class Button 
{
protected:
   // int buttonHeight;
   // int buttonWidth;
	int		buttonXPos;
	int		buttonYPos;
	buttonState	buttonStatus;
	oglTexture2D* normal;
	oglTexture2D* clicked;
	oglTexture2D* hover;
	CFunctionPointer0R<bool>	mClickHandler;	// Matches 'bool foo()'
	CFunctionPointer0R<bool>	mHoverHandler; 

public:
	Button();
	Button(int, int, std::string, std::string, std::string);
	~Button();
	
	virtual void	setButtonStatus(buttonState buttonStatus);
	virtual int		getHeight();
	virtual int		getWidth();
	virtual buttonState	getButtonStatus();
	virtual void	setButtonXPos(int position);
	virtual void	setButtonYPos(int position);
	virtual void	setButtonXYPos(int xPos, int yPos);
	virtual void	setPosition(int x, int y);
	virtual void	setDimensions(int width, int height);
	virtual int		getXPos();
	virtual int		getYPos();
	virtual bool	Initialize();
	virtual bool	Update();
    virtual bool	Draw();
	virtual bool	Remove();
	virtual void	setClickHandler(CFunctionPointer0R<bool> clickHandler);
	virtual void	setHoverHandler(CFunctionPointer0R<bool> hoverHandler);
	virtual bool	callClickHandler();
	virtual bool	callHoverHandler();
	virtual bool	checkInBounds(int x, int y);
};

#endif
