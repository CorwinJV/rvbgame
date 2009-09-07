#ifndef LOGICINTERFACE_H
#define LOGICINTERFACE_H

#include "oglUtility.h"
#include "oglTexture2D.h"
#include "oglGameVars.h"
#include "aiInstructionEnums.h"
#include "logicBlock.h"
#include "oglGameVars.h"
#include "MenuSystem.h"
#include "FunctionPointer.h"

#include <vector>
#include <sstream>
#include <string>

using namespace BE;

class LogicInterface
{

public:
	LogicInterface();
	~LogicInterface();
	void Update();
	void Draw();
	void processMouse(int x, int y);
	void processMouseClick(int button, int state, int x, int y);

	//=====================
	// Button Callbacks
	bool LogicBankUpArrowButtonClick();
	bool LogicBankDownArrowButtonClick();
	bool ExecutionListUpArrowButtonClick();
	bool ExecutionListDownArrowButtonClick();
	bool MainTabButtonClick();
	bool Sub1TabButtonClick();
	bool Sub2TabButtonClick();

	bool ExecuteButtonClick();
	bool AbortButtonClick();
	bool ResetButtonClick();
	bool ClearButtonClick();

	bool ClearExecutionList();
	void ResetExecutionMode();
	bool CommandAdvanced(instructionTab instrTab, logicBlock* curBlock);
	bool ReprogramReached();
	bool launchHelpState();
	
	bool speedUpButtonClick();
	bool slowDownButtonClick();

	void SetExecuteHandler(CFunctionPointer3R<bool, std::vector<logicBlock*>, std::vector<logicBlock*>, std::vector<logicBlock*>> clickHandler);
	void SetAbortHandler(CFunctionPointer0R<bool> clickHandler);
	void SetResetHandler(CFunctionPointer0R<bool> resetHandler);
	void SetHelpHandler(CFunctionPointer0R<bool> helpHandler);
	void SetSpeedUpHandler(CFunctionPointer0R<bool> speedUpHandler);
	void SetSlowDownHandler(CFunctionPointer0R<bool> slowDownHandler);
	void GetCurrentMapLogicBank();
	void GetCurrentLevelBytes();

private:
	MenuSys* alwaysActiveMenu;
	MenuSys* myMenu;
	MenuSys* executingMenu;
	MenuSys* resetMenu;
	bool isButtonBeingClicked;
	bool isExecuting;

	oglTexture2D* menuBar;
	oglTexture2D* panelArt;
	oglTexture2D* commandBackdrop;
	oglTexture2D* commandBackdrop2;
	oglTexture2D* scrollBar;
	oglTexture2D* insertionLine;
	oglTexture2D* bytesLeftBackground;

	int mouseX;
	int mouseY;
	int draggedBlockMouseX;
	int draggedBlockMouseY;
	bool isMouseDragging;
	logicBlock* draggedBlock;

	int currentHoverBlockIndex;

	//=====================
	// Button Callbacks
	//CFunctionPointer1R<bool, std::vector<logicBlock*>> mExecuteHandler;
	CFunctionPointer3R<bool, std::vector<logicBlock*>, std::vector<logicBlock*>, std::vector<logicBlock*>> mExecuteHandler;
	CFunctionPointer0R<bool> mAbortHandler;
	CFunctionPointer0R<bool> mResetHandler;
	CFunctionPointer0R<bool> mHelpHandler;
	CFunctionPointer0R<bool> mSpeedUpHandler;
	CFunctionPointer0R<bool> mSlowDownHandler;

	//======================
	// Score Tracking
	int mapByteLimit;
	int usedBytes;

	//======================
	// Size and Positional Variables
	screenBox sideBarBox;
	int* curExecutionListYOffset;
	int executionListYOffset;
	int executionListSub1YOffset;
	int executionListSub2YOffset;

	// hacks for missing features
	int executionListScrolled;
	int executionListSub1Scrolled;
	int executionListSub2Scrolled;
	int* curExecutionListScrolled;

	int sideBarColumnCount;
	
	screenBox bottomBarBox;
	int logicBankYOffset;
	int logicBankScrollbarXOffset;

	int instructionBlockW;
	int instructionBlockH;
	int instructionSpacing;

	int logicBankNumColumns;
	int logicBankNumRowsOnScreen;

	int instructionListNumColumns;
	int instructionListNumRowsOnScreen;

	bool drawInsertionLine;
	int insertionLineColumn;
	int insertionLineRow;

	screenBox logicBankBox;
	screenBox instructionListBox;

	// Highlighted Command Tracking
	bool isProcessingSub;

	std::vector<logicBlock*>* logicBank;		// The usable logic blocks
	std::vector<logicBlock*> executionList;	// The command to give the robot
	std::vector<logicBlock*> executionListSub1;
	std::vector<logicBlock*> executionListSub2;

	instructionTab curInstrTab;
};

#endif // LOGICINTERFACE_H