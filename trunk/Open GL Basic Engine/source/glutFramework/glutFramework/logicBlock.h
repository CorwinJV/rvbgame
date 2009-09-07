#ifndef LOGICBLOCK_H
#define LOGICBLOCK_H

#include "aiInstructionEnums.h"
#include "oglTexture2D.h"
#include <vector>
#include <string>
#include "aiInstructionEnums.h"

enum logicBlockState
{
	BS_INACTIVE,
	BS_ACTIVE,
	BS_HIGHLIGHTED
};

	//===============================================
	// logicBlock
	// This struct defines a container for a texture
	// (icon), a description (like a tool tip),
	// a point value, and a vector of AiInstructions
	// enum values.
	//
	// These blocks are the building blocks of 
	// execution.

struct logicBlock
{
	logicBlock()
	{
		blockTexture = NULL;
	}
	logicBlock(std::string textureName, int width, int height, std::string description, int byteCost, AiInstructions enumInstruction)
	{
		blockTexture = new oglTexture2D();
		blockTexture->loadImage(textureName, width, height);
		blockDescription = description;
		logicBlock::byteCost = byteCost;
		logicBlock::enumInstruction = enumInstruction;
		curButtonState = BS_INACTIVE;
		isUsable = true;
		isCurrentlyUsable = true;
	}
	~logicBlock()
	{
		delete blockTexture;
		blockTexture = NULL;
	}

	// Copy Constructor:
	logicBlock(const logicBlock& p) 
	{
		blockDescription = p.blockDescription;
		blockTexture = new oglTexture2D(*(p.blockTexture));
		instructionList = p.instructionList;
		byteCost = p.byteCost;
		enumInstruction = p.enumInstruction;
		curButtonState = p.curButtonState;
		isUsable = p.isUsable;
		isCurrentlyUsable = p.isCurrentlyUsable;
	}

	void addInstruction(AiInstructions instr)
	{
		instructionList.push_back(instr);
	}

	bool checkInBounds(int x, int y, int widthBound, int heightBound)
	{
		// dX and dY should be width and height
		if (((x > blockTexture->mX) && (x < (blockTexture->mX + widthBound))) &&
			(y > blockTexture->mY) && (y < (blockTexture->mY + heightBound)))
			return true;
		else 
			return false;
	}

	std::string							blockDescription;
	oglTexture2D*						blockTexture; 
	std::vector<AiInstructions>			instructionList;
	int									byteCost;
	AiInstructions						enumInstruction;
	logicBlockState						curButtonState;
	bool								isUsable;
	bool								isCurrentlyUsable;
};

#endif