#include "subroutine.h"

subroutine::subroutine()
{
	currentInstruction = instructionList.begin();
	objectType = OSubroutine;
}

subroutine::~subroutine()
{
}

bool subroutine::advanceCommand()
{
	currentInstruction++;
	if(currentInstruction == instructionList.end())
	{
		currentInstruction = instructionList.begin();
		return false;
	}
	return true;
}

AiInstructions subroutine::getNextCommand()
{
	return (*currentInstruction)->enumInstruction;
}

logicBlock* subroutine::getNextCommandBlock()
{
	return (*currentInstruction);
}

void subroutine::draw()
{
}

void subroutine::update()
{
}

objectEnum subroutine::getType()
{
	return objectType;
}

void subroutine::addCommand(logicBlock* newCommand)
{
	instructionList.push_back(newCommand);
	currentInstruction = instructionList.begin();
}

void subroutine::removeLastCommand()
{
	instructionList.pop_back();
}

void subroutine::clearInstructions()
{
	instructionList.clear();
}

void subroutine::coreDump()
{
	vector <logicBlock*>::iterator itr = instructionList.begin();

	//std::cout << "Subroutine Core Dump: " << endl;
	for(;itr < instructionList.end(); itr++)
	{
		//std::cout << (*itr)->enumInstruction << " ";
	}
	//std::cout << endl;
}

bool subroutine::isEmpty()
{
	return instructionList.empty();
}