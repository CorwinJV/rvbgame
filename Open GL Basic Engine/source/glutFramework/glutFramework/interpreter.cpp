#include "interpreter.h"

void interpreter::addInterp(std::string name, CFunctionPointer0R<bool> nTargetFunc, int numVars = 0)
{
	interpObj* tempInterpObj;
	tempInterpObj = new interpObj();
	tempInterpObj->setTargetFunc(nTargetFunc);
	tempInterpObj->setNumVars(numVars);
	tempInterpObj->setName(name);
	interpList.push_back(tempInterpObj);
}

void interpreter::interpret(std::string interpMe)
{
	std::vector<interpObj*>::iterator iitr = interpList.begin();

	// iterate through the vector looking for a pattern match
	for(;iitr < interpList.end(); iitr++)
	{
		// found the match
		if(interpMe == (*iitr)->getName())
		{
			// do whatever should be done			
		}
	}
}