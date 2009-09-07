#include "SG400KGA1.h"
#include <stdlib.h>

bool SG400KGA1::Update()
{
	return true;
}

bool SG400KGA1::Draw()
{
	// since all fonts are now well, blackish stuff, lets draw a white background
	solidWhite->drawImage();
	// image
	if(imgSG400)
	{
		imgSG400->drawImageFaded(alpha);
	}

	// interface
	glColor3ub(255, 0, 0);
	GameVars->fontArial18.drawText(25, 700, "Enter Command:");

	glColor3ub(0, 255, 0);
	GameVars->fontArial18.drawText(220, 700, tempString);

	// history
	glColor3ub(255, 255, 255);
	GameVars->fontArial18.drawText(25, 10, "Command History");
	int offsetAmt = 0;
	int textOffset = 14;

	vector<string*>::iterator sitr;
	sitr = commandList.begin();
	
	for(;sitr < commandList.end(); sitr++)
	{
		GameVars->fontArial12.drawText(25, 29 + offsetAmt*textOffset, (*sitr)->c_str());
		offsetAmt++;
	}

	// draw instruction list on top right
	offsetAmt = 0;
	textOffset = 15;
	int xp = 575;
	int yp = 25;

	GameVars->fontArial12.drawText(xp, yp + offsetAmt * textOffset, "Available Commands");
	offsetAmt++;
	GameVars->fontArial12.drawText(xp, yp + offsetAmt * textOffset, "Values inside () & [] are");
	offsetAmt++;
	GameVars->fontArial12.drawText(xp, yp + offsetAmt * textOffset, "used without the () & []");
	offsetAmt++;
	GameVars->fontArial12.drawText(xp, yp + offsetAmt * textOffset, "() denotes values");
	offsetAmt++;
	GameVars->fontArial12.drawText(xp, yp + offsetAmt * textOffset, "[] denotes specific words");
	offsetAmt++;
	GameVars->fontArial12.drawText(xp, yp + offsetAmt * textOffset, "i.e. move up 50");
	offsetAmt++;
	GameVars->fontArial12.drawText(xp, yp + offsetAmt * textOffset, "");
	offsetAmt++;
	GameVars->fontArial12.drawText(xp, yp + offsetAmt * textOffset, "move [up/down/left/right] (pixels)");
	offsetAmt++;
	GameVars->fontArial12.drawText(xp, yp + offsetAmt * textOffset, "resize (width) (height)");
	offsetAmt++;
	GameVars->fontArial12.drawText(xp, yp + offsetAmt * textOffset, "set [alpha/width/height/x/y] (value)");
	offsetAmt++;
	GameVars->fontArial12.drawText(xp, yp + offsetAmt * textOffset, "reset");
	offsetAmt++;
	GameVars->fontArial12.drawText(xp, yp + offsetAmt * textOffset, "load (filename)");
	offsetAmt++;
	GameVars->fontArial12.drawText(xp, yp + offsetAmt * textOffset, "quit");
	offsetAmt++;
	return false;
}

void SG400KGA1::processMouse(int x, int y)
{

}

void SG400KGA1::processMouseClick(int button, int state, int x, int y)
{

}


void SG400KGA1::keyboardInput(unsigned char c, int x, int y)
{
	switch(c)
	{
	case 13: // enter key
		if(finished == 2)
		{
			tempString = "";
			finished = 0;
		}
		else
			finished = 1;
		break;
	case 8:	 // backspace
		if(tempString.length() > 0)
			tempString.erase(tempString.length() - 1, 1);
		break;
	case 27: // escape key
		tempString = "";
		/*GSM->addGameState<MainMenuState>();
		this->setStatus(DeleteMe);*/
		break;
	default:
		if(tempString.length() <= 60)
		{
			tempString += c;
			finished = 0;
		}
		break;
	}

	if(finished)
	{
		tString = new string;
		*tString = tempString;
		interpret(tString);
		commandList.push_back(tString);
		tempString = "";
	}

}

void SG400KGA1::interpret(string *iString)
{
	// parse this out into various things first
	vector<string*> parseList;

	bool done = false;
	bool foundCommand = false;
	bool foundvar1 = false;
	bool foundvar2 = false;
	string command;
	string var1 = "";
	string var2 = "";
	int targetSpot = 0;
	(*iString) += " ";

	for(int x = 0; x < (int)iString->length(); x++)
	{
		if(iString->substr(x, 1) == " ")
		{
			// we found a space, lets do something with it
			if(!foundCommand)
			{
				command = iString->substr(0, x);
				foundCommand = true;
				targetSpot = x+1;
			}
			else if(!foundvar1)
			{
				var1 = (iString->substr(targetSpot, x-targetSpot).c_str());
				foundvar1 = true;
				targetSpot = x+1;
			}
			else if(!done)
			{
				var2 = (iString->substr(targetSpot, x-targetSpot).c_str());
				done = true;
				foundvar2 = true;
			}
		}
	}

	if(foundCommand)
	{
		if(command == "quit")
		{
			exit(0);
		}
		else if(command == "move")
		{
			if(foundvar1)
			{
				if(var1 == "up")
				{
					imgSG400->mY = imgSG400->mY - atoi(var2.c_str());
				}
				else if(var1 == "down")
				{
					imgSG400->mY = imgSG400->mY + atoi(var2.c_str());
				}
				else if(var1 == "left")
				{
					imgSG400->mX = imgSG400->mX - atoi(var2.c_str());
				}
				else if(var1 == "right")
				{
					imgSG400->mX = imgSG400->mX + atoi(var2.c_str());
				}
				else
				{
					*iString += " <invalid direction>";
				}
			}
		}
		else if(command == "resize")
		{
			if(foundvar1)
			{
				imgSG400->dX = atoi(var1.c_str());
			}
			
			if(foundvar2)
			{
				imgSG400->dY = atoi(var2.c_str());
			}
		}
		else if(command == "reset")
		{
			delete imgSG400;
			imgSG400 = new oglTexture2D();
			imgSG400->loadImage("buttons\\bacardi.png", 256, 256);
			imgSG400->dX = 256;
			imgSG400->dY = 256;
			imgSG400->mX = 300;
			imgSG400->mY = 50;
			alpha = 1.0;
		}
		else if(command == "load")
		{
			if(foundvar1)
			{
				delete imgSG400;
				imgSG400 = new oglTexture2D();
				if(!imgSG400->loadImage(var1.c_str(), 256, 256))
				{
					*iString += " <invalid file>";
				}
				imgSG400->dX = 256;
				imgSG400->dY = 256;
				imgSG400->mX = 300;
				imgSG400->mY = 50;
			}
		}
		else if(command == "set" && foundvar1 && foundvar2)
		{
			if(var1 == "alpha")
			{
				alpha = atof(var2.c_str());
			}
			else if(var1 == "width")
			{
				imgSG400->dX = atoi(var2.c_str());
			}
			else if(var1 == "height")
			{
				imgSG400->dY = atoi(var2.c_str());
			}
			else if(var1 == "x")
			{
				imgSG400->mX = atoi(var2.c_str());
			}
			else if(var1 == "y")
			{
				imgSG400->mY = atoi(var2.c_str());
			}
		}
		else	// invalid command
		{
			*iString += " <Invalid Command>";
		}
	}

}