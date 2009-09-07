#ifndef OGLUTILITY_H
#define OGLUTILITY_H

#include <string>
#include <GL/glut.h>

enum State { 
				Active,		// updating	and drawing
				Passive,	// drawing and not updating
				Hidden,		// not updating and not drawing
				DeleteMe,	// flagged for removal
};

struct screenBox
{
	int x;
	int y;
	int width;
	int height;

	screenBox()
		: x(0), y(0), 
		width(0), height(0)
	{
	}
};

//======================
// Instruction List Tabs
enum instructionTab
{
	TAB_MAIN,
	TAB_SUB1,
	TAB_SUB2
};


#endif