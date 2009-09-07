#ifndef INTERPRETER_H
#define INTERPRETER_H
#include <cstring>
#include "oglUtility.h"
#include "FunctionPointer.h"
#include <iostream>
#include "oglTexture2D.h"
#include <vector>

enum interDirection {
		UP,
		DOWN,
		LEFT,
		RIGHT
};

using namespace BE;

class interpObj
{
private:
	std::string Name;
	CFunctionPointer0R<bool>	targetFunc;
	int numVars;

public:
	void setName(std::string newName)	{	Name = newName;	};
	std::string getName()	{ return Name;}
	void setTargetFunc(CFunctionPointer0R<bool> nTargetFunc)	{	targetFunc = nTargetFunc;	}
	void setNumVars(int x)	{	numVars = x; };
	int getNumVars()		{ return numVars;};

	bool callTargetFunc()
	{
		if(targetFunc)
		{
			return targetFunc();
		}
		else
		{
			return false;
		}
	}

};

using namespace BE;

class interpreter
{
private:
	std::vector<interpObj*> interpList;

public:
	interpreter();	
	~interpreter();

	void addInterp(std::string name, CFunctionPointer0R<bool> nTargetFunc, int numVars);

	void interpret(std::string);
	void interpret(std::string, int var1);
	void interpret(std::string, int var1, int var2);
};

#endif