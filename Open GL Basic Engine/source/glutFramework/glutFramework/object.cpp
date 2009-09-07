#include "object.h"

object::object()
{
	xPos = 0;
	yPos = 0;
	direction = 0;
	active = true;
	startXPos = 0;
	startYPos = 0;
	startDirection = 0;
	startActive = true;
	currentInstruction = instructionList.begin();
	alive = true;
}

object::object(int x, int y, int newDirection, objectEnum newType)
{
	setXPos(x);
	setYPos(y);
	direction = newDirection;
	active = true;
	objectType = newType;
	startXPos = x;
	startYPos = y;
	startDirection = newDirection;
	startActive = true;
	currentInstruction = instructionList.begin();
	alive = true;
}

object::~object()
{
	delete image;
	//std::cout << "tick tick tick boom" << endl;
}

bool object::getActive()
{
	return active;
}

int object::getDirection()
{
	return direction;
}

int object::getXPos()
{
	return xPos;
}

int object::getYPos()
{
	return yPos;
}

void object::setActive(bool activate)
{
	active = activate;
}

void object::toggleActive()
{
	if(active == true)
		active = false;
	else
		active = true;
}

void object::rotate(int rotation)
{
	//send in a +1 for right, -1 for left
	direction += rotation;

	if(direction < 0)	direction = 3;
	if(direction > 3)	direction = 0;
}

void object::setXPos(int x)
{
	xPos = x;
}

void object::setYPos(int y)
{
	yPos = y;
}

void object::advanceCommand()
{
	currentInstruction++;
	if(currentInstruction == instructionList.end())
		currentInstruction = instructionList.begin();
}

AiInstructions object::getNextCommand()
{
	return (*currentInstruction)->enumInstruction;
}

logicBlock* object::getNextCommandBlock()
{
	return (*currentInstruction);
}

void object::draw()
{
	
}

void object::update()
{
	//advance animation code here
}

void object::startOver()
{	
	xPos = startXPos;
	yPos = startYPos;
	direction = startDirection;
	active = startActive;
}

objectEnum object::getType()
{
	return objectType;
}

void object::addCommand(logicBlock* newCommand)
{
	instructionList.push_back(newCommand);
	currentInstruction = instructionList.begin();
}

void object::removeLastCommand()
{
	instructionList.pop_back();
}

void object::clearInstructions()
{
	instructionList.clear();
}

void object::coreDump()
{
	vector <logicBlock*>::iterator itr = instructionList.begin();

//	std::cout << "Core Dump: " << endl;
	for(;itr < instructionList.end(); itr++)
	{
		//std::cout << (*itr)->enumInstruction << " ";
	}
	//std::cout << endl;
}

void object::setAlive(bool status)
{
	alive = status;
}
bool object::getAlive()
{
	return alive;
}

void object::reset()
{
	xPos = startXPos;
	yPos = startYPos;
	direction = startDirection;
}

void object::setDefaults(int direction, int x, int y)
{
	startXPos = x;
	startYPos = y;
	startDirection = direction;
}