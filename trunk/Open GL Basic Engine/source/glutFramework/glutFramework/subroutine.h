#ifndef SUBROUTINE_H
#define SUBROUTINE_H

#include "aiInstructionEnums.h"
#include <vector>
#include "objectEnums.h"
#include "logicBlock.h"

using namespace std;

class subroutine
{
protected:
	objectEnum		objectType;
	vector<logicBlock*>	instructionList;
	vector<logicBlock*>::iterator currentInstruction;


public:
	//default constructors, destructors
	subroutine();
	~subroutine();
	virtual void	update();
	virtual bool	advanceCommand();
	virtual AiInstructions	getNextCommand();
	virtual logicBlock*		getNextCommandBlock();
	virtual objectEnum		getType();
	virtual void	addCommand(logicBlock *newCommand);
	virtual void	removeLastCommand();
	virtual void	clearInstructions();
	virtual void	coreDump();
	virtual void	draw();
	virtual bool	isEmpty();
};

#endif