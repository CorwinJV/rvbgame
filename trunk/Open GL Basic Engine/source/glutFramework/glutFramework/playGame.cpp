#include "playGame.h"
#include "GameStateManager.h"
#include <sstream>
#include <string>


bool playGame::Update()
{
	// get the game state
	curState = gamePlay->getCurState();

	// level variables
	int levelCounter;
	string tempString;
	int tempInt;
	bool inGameStatus;
	
	// see if the robot is at the end square
	if((gamePlay->robotAtEndSquare())&& (curState == GB_EXECUTION))
	{
		curState = GB_VICTORYDANCE;
	}

	// Update mInterface all the time
	mInterface.Update();
	compass->Update();

	if(GameVars->getCurrentLevel() <= GameVars->numTutorialLevels)
	{
		skipMenu->Update();
	}

	int maxLevel;
	switch(curState)
	{
	case GB_LOGICVIEW:
		inGameStatus = true;
		GameVars->setInGame(inGameStatus);
		gameSaved = false;
		pregameRunning = false;
		gamePlay->update();
		gamePlay->mapScroll();
		break;

	case GB_EXECUTION:
		inGameStatus = true;
		GameVars->setInGame(inGameStatus);
		if(!finishNow && !finishing)
		{
			gameSaved = false;
			gamePlay->update();
			gamePlay->mapScroll();
		}
		break;

	case GB_PREGAME:
		inGameStatus = true;
		GameVars->setInGame(inGameStatus);
		gameSaved = false;
		if(pregameRunning)
		{
			timer = clock();
			if(timer > (startTime + 5000))
			{
				pregameRunning = false;
				GameVars->commandsProcessed = 0;
				GameVars->totalCommandsProcessed = 0;
				gamePlay->setState(GB_LOGICVIEW);
			}
		}
		else
		{
			startTime = clock();
			timer = clock();
			pregameRunning = true;
		}
		break;

	case GB_ROBOTDIED:
		inGameStatus = true;
		GameVars->setInGame(inGameStatus);
		gameSaved = false;
		doneDead = false;
		if(pregameRunning)
		{
			timer = clock();
			if(timer > (startTime + 5000))
			{
				pregameRunning = false;
				doneDead = true;
			}
		}
		else
		{
			startTime = clock();
			timer = clock();
			pregameRunning = true;
		}
		break;

	case GB_VIEWSCORE:
		inGameStatus = false;
		GameVars->setInGame(inGameStatus);
		//save the game for the player, if it hasn't been saved yet
		finishNow = false;
		if(!gameSaved)
		{
			GameVars->updatePlayerFile();
		//	GameVars->PM->saveProfile();
			gameSaved = true;
		}

		//Get the number of bytes user started the level off with and subtract number remaining
		playerScore = GameVars->getTotalScore();

		// eventually we will implement an equation here to calculate the score based off percentage 
		// of points used for the level score
		
		myMenu->Update();
		break;

	case GB_FINISHED:
		inGameStatus = false;
		GameVars->setInGame(inGameStatus);
		//gamePlay->~gameBoard();

		mInterface.ClearExecutionList();
		mInterface.ResetExecutionMode();
		levelCounter = GameVars->getCurrentLevel();
		levelCounter++;
		maxLevel = GameVars->getMaxLevel();

		if(levelCounter < (GameVars->getMaxLevel()))
		{
			delete gamePlay;			
			// in with the new
			gamePlay = new gameBoard();			
			tempString = GameVars->getFilename(levelCounter);
			//tempInt = GameVars->getCurrentLevel();
			gamePlay->LoadGameMapFromFile(tempString);
			mInterface.GetCurrentMapLogicBank();
			mInterface.GetCurrentLevelBytes();
			GameVars->commandsProcessed = 0;
			GameVars->totalCommandsProcessed = 0;
			curState = GB_PREGAME;
			GameVars->didYouKnowI++;
			if(GameVars->didYouKnowI == GameVars->didYouKnow.end())
			{
				GameVars->didYouKnowI = GameVars->didYouKnow.begin();
			}
		}
		else
		{
			curState = GB_YOUWIN;
			levelCounter;			
		}

		GameVars->setLevel(levelCounter);
		if(levelCounter > GameVars->getPlayerMaxLevel())
		{
			GameVars->setPlayerMaxLevel(levelCounter);
		}
	
		//=====================================================
		// Register the gameBoard callback with the interface!
		mInterface.SetExecuteHandler(BE::CreateFunctionPointer3R(gamePlay, &gameBoard::interfaceHasFiredExecuteOrder));
		mInterface.SetAbortHandler(BE::CreateFunctionPointer0R(gamePlay, &gameBoard::interfaceHasFiredAbortOrder));
		mInterface.SetResetHandler(BE::CreateFunctionPointer0R(gamePlay, &gameBoard::interfaceHasFiredResetOrder));
		mInterface.SetHelpHandler(BE::CreateFunctionPointer0R(this, &playGame::launchHelpState));
		mInterface.SetSpeedUpHandler(BE::CreateFunctionPointer0R(this, &playGame::speedUp));
		mInterface.SetSlowDownHandler(BE::CreateFunctionPointer0R(this, &playGame::slowDown));
		gamePlay->SetInterfaceAdvanceHandler(BE::CreateFunctionPointer2R(&mInterface, &LogicInterface::CommandAdvanced));
		gamePlay->SetInterfaceReprogramHandler(BE::CreateFunctionPointer0R(&mInterface, &LogicInterface::ReprogramReached));
		gamePlay->SetInterfaceClearExecutionListHandler(BE::CreateFunctionPointer0R(&mInterface, &LogicInterface::ClearExecutionList));

		gamePlay->setState(curState);
		pregameRunning = false;
		break;
	case GB_YOUWIN:
		// upload score onto server (when it gets implemented)

		break;
	case GB_VICTORYDANCE:
		if(!finishing)
		{
			startTime = clock();
			spintimer = clock();
			spintimerStart = clock();
			finishing = true;
		}
		timer = clock();
		if(timer > startTime + 2000)
		{
			finishing = false;
			finishNow = true;
			curState = GB_VIEWSCORE;
			gamePlay->setState(curState);
			GameVars->totalCommandsProcessed += GameVars->commandsProcessed;
			double scoreToAdd = 0;
			int bytesUsed = GameVars->getBytesUsed();
			int bytesAvail = GameVars->getCurrentLevelBytes();
			// happy magical scoring stuff yay

			// for the time being, we're going to add a 10% bonus per level
			// such that at level 1 you get 110% score, level 2 you get  120%
			// and so on and so on capping out at roughly 250% at 15 levels
			// so bust out those subroutines on the higher levels for maximum scorage yay!
			// score bonus is only applied to the memory used portion of the scoring
			// function, the default of 200 base is always default of 200 base so yeah there
			// for shitz n gigglez i'm having it give only 25% bonus on tutorial levels
			// you won't get a whole lot of points on the tutorials
			double levelmultiplier = 0.0;
			if(GameVars->getCurrentLevel() > GameVars->numTutorialLevels)
			{	// if we're in a normal level, do this
				levelmultiplier = GameVars->getCurrentLevel() - GameVars->numTutorialLevels;
				levelmultiplier *= 0.1;  // here's our 10% 
				levelmultiplier += 1;
			}
			else
			{
				levelmultiplier = 0.25;	// 25% for tutorial levels, suck it tutorials
			}

			scoreToAdd = ((100 - (((double)bytesUsed/(double)bytesAvail)*100)) * 10)*levelmultiplier + 200;
			//scoreToAdd *= (double)GameVars->getCurrentLevel() * 0.1;

			// sends in your score for the level just completed to be compared against previous attempts
			GameVars->setLevelScore(scoreToAdd);
			// Get the level score from the level just completed, and add it up with all previous levels completed
			GameVars->setTotalScore(GameVars->getLevelScore() + GameVars->getTotalScore());
		}
		spintimer = clock();
		if(spintimer > spintimerStart + 200)
		{
			gamePlay->spinRobot();
			spintimerStart = clock();
		}
		break;
	default:
		break;
	}

	if((!gamePlay->getRobotAlive()) && doneDead)
	{
		mInterface.AbortButtonClick();
		doneDead = false;
		gamePlay->setState(GB_LOGICVIEW);
	}

	// ask the gameboard is a reprogrammable square was hit, if so we need to make a popup screen!
	if(gamePlay->getReprogramHit())
	{
		gamePlay->setReprogramHit(false);
		GSM->addGameState<clickOKState>();
	}

	return true;
}

bool playGame::Draw()
{
	GameBoardState curState;
	curState = gamePlay->getCurState();
	string tempString;
	int offsetAmt = 0;
	std::stringstream painInTheAss;
	//clock_t startTime;
	int tempInt;
	oglTexture2D fadeToBlack;
	int textspacing = 30;
	int speed = gamePlay->getProcessSpeed();
	int viewscoretext = backgroundImage->mY+50;
	
	// did you know stuffs
	int tY = 600;
	int tYspace = 20;
	int tAmt = 0;
	vector<string*> didYouKnowParsed;
	vector<string*>::iterator dykItr;

	switch(curState)
	{
	case GB_LOGICVIEW:
		glClearColor(0, 0, 0, 0);
		gamePlay->draw();
		if(GameVars->getCurrentLevel() <= GameVars->numTutorialLevels)
		{
			skipMenu->Draw();
		}
		mInterface.Draw();
		drawLevelInfo();
		compass->Draw();
		
		// display starting speed
		glColor3ub(0, 0, 0);
		GameVars->fontArial12.drawText(258, 650, "Speed: ");
		painInTheAss.str("");
		painInTheAss << (1100 - speed) << " MHZ";
		tempString += painInTheAss.str();
		GameVars->fontDigital12.drawText(258, 670, tempString);
		break;

	case GB_EXECUTION:
		glClearColor(0, 0, 0, 0);
		gamePlay->draw();
		mInterface.Draw();
		drawLevelInfo();
		compass->Draw();

		// display current speed
		glColor3ub(0, 0, 0);
		GameVars->fontArial12.drawText(258, 650, "Speed: ");
		painInTheAss.str("");
		painInTheAss << (1100 - speed) << " MHZ";
		tempString += painInTheAss.str();
		GameVars->fontDigital12.drawText(258, 670, tempString);
		break;

		break;

	case GB_PREGAME:

		if(currentStatus == Passive)	// waiting for click ok to continue to finish...
		{
			startTime = clock();
			timer = clock();
			pregameRunning = true;
			gamePlay->setState(GB_PREGAME);
		}

		clearBackground();

		logoImage->drawImage();
		backgroundImage->drawImage();

		glColor3ub(0, 0, 0);

		// player name
		tempString = "Player Name: ";
		tempString += GameVars->PM->getPlayerName();
		GameVars->fontArial24.drawText(preGameTextOffsetX, preGameTextOffsetY + offsetAmt*preGameTextSpacing, tempString);
		offsetAmt++;

		// level name
		tempInt = GameVars->getCurrentLevel();
		tempString = GameVars->getLevelName(tempInt);

		offsetAmt += GameVars->fontArial24.drawText(preGameTextOffsetX, preGameTextOffsetY + offsetAmt*preGameTextSpacing, tempString, 48);

		// level description
		offsetAmt += GameVars->fontArial18.drawText(preGameTextOffsetX, preGameTextOffsetY + offsetAmt*preGameTextSpacing, GameVars->getDesc(tempInt), 48);

				// bytes available
		tempString = "Bytes Available: ";
		painInTheAss.str("");
		tempInt = GameVars->getCurrentLevelBytes();
		painInTheAss << tempInt;
		tempString += painInTheAss.str();
		GameVars->fontArial18.drawText(preGameTextOffsetX, preGameTextOffsetY + offsetAmt*preGameTextSpacing, tempString);
		offsetAmt++;
		
		// did you know
		GameVars->fontArial16.drawText(preGameTextOffsetX, tY+ (tAmt*tYspace), "Did You Know:");
		tAmt++;
	
		GameVars->parseMeIntoRows(&didYouKnowParsed, (*GameVars->didYouKnowI), 56, true);
		dykItr = didYouKnowParsed.begin();

		for(;dykItr < didYouKnowParsed.end(); dykItr++)
		{
			GameVars->fontArial16.drawText(preGameTextOffsetX, tY+ (tAmt*tYspace), (*dykItr)->c_str());
			tAmt++;
		}
		break;

	case GB_ROBOTDIED:
		// gl shit that may or may not be needed for font stuff, we shall find out shortly
		clearBackground();
		//glutSwapBuffers();
		//glEnable(GL_TEXTURE_2D);
		//glEnable(GL_BLEND);

		/*glColor3ub(255, 0, 0);

		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);*/

		// player name
		//GameVars->fontDigital200.drawText(150 + rand()%15, 250 + rand()%15, "YOU DIED");
		youDiedImage->mX = rand()%15;
		youDiedImage->mY = rand()%15;
		youDiedImage->drawImage();
		break;

	case GB_VIEWSCORE:
		clearBackground();
		logoImage->drawImage();
		backgroundImage->drawImage();
		
		if(myMenu != NULL)
			myMenu->Draw();

		glColor3ub(0, 0, 0);
		//
		// level name
		tempString = "Congratulations! Level Complete!";
		GameVars->fontArial32.drawText(150, viewscoretext+ offsetAmt*textspacing, tempString);
		offsetAmt++;
		offsetAmt++;
		offsetAmt++;

		// level name
		tempInt = GameVars->getCurrentLevel();
		tempString = GameVars->getLevelName(tempInt);

		offsetAmt += GameVars->fontArial18.drawText(200, viewscoretext+ offsetAmt*textspacing, tempString, 45);

		// level description
		tempString = GameVars->getDesc(tempInt);
		// description
		offsetAmt += GameVars->fontArial18.drawText(200, viewscoretext+ offsetAmt*textspacing, tempString, 45);
		offsetAmt++;

		painInTheAss.str("");

		// bytes used
		tempString = "";
		tempString = "BYTES USED: ";
		tempInt = GameVars->getBytesUsed(); // this should get the bytes used value
		painInTheAss.str("");
		painInTheAss << tempInt;
		tempString += painInTheAss.str();
		GameVars->fontArial24.drawText(200, viewscoretext+ offsetAmt*textspacing, tempString);
		offsetAmt++;

		// commands used
		tempString = "";
		tempString = "COMMANDS PROCESSED: ";
		tempInt = GameVars->totalCommandsProcessed;
		painInTheAss.str("");
		painInTheAss << tempInt;
		tempString += painInTheAss.str();
		GameVars->fontArial24.drawText(200, viewscoretext+ offsetAmt*textspacing, tempString);
		offsetAmt++;
		
		// level score
		tempString = "";
		tempString = "YOUR LEVEL SCORE: ";
		tempInt = GameVars->getLevelScore();
		painInTheAss.str("");
		painInTheAss << tempInt;
		tempString += painInTheAss.str();
		GameVars->fontArial24.drawText(200, viewscoretext+ offsetAmt*textspacing, tempString);
		offsetAmt++;

		// TOTAL SCORE
		tempString = "";
		tempString = "YOUR TOTAL SCORE: ";
		tempInt = GameVars->getTotalScore();
		painInTheAss.str("");
		painInTheAss << tempInt;
		tempString += painInTheAss.str();
		GameVars->fontArial24.drawText(200, viewscoretext+ offsetAmt*textspacing, tempString);
		offsetAmt++;
		
		if(myMenu != NULL)
			myMenu->Draw();
		//mInterface.Draw();
		break;

	case GB_FINISHED:
		//gamePlay->draw();
		//mInterface.Draw();
		break;

	case GB_YOUWIN:
		doEndGameDraw();
		break;

	default:
		break;
	}
	return false;
}

bool playGame::initialize()
{
	finishNow = false;
	finishing = false;
	string tempString;
	int playerCurrentLevel;
	youDiedImage = new oglTexture2D();
	youDiedImage->loadImage("youdied.png", 1024,768);
	doneDead = false;

	gamePlay = new gameBoard();

	//check to see if a level has been specified (initialized to -1)
	playerCurrentLevel = GameVars->getLevelSpecified();
	if(playerCurrentLevel < 0)
	{
		//if a level has not been specified, set to max level
		playerCurrentLevel = GameVars->getPlayerMaxLevel();
	}

	// debug brute force of level
	// abcxyz
	// playerCurrentLevel = 0;
	GameVars->setLevel(playerCurrentLevel);
	GameVars->setLevelSpecified(-1);
	
	tempString = GameVars->getFilename(playerCurrentLevel);
	gamePlay->LoadGameMapFromFile(tempString);
	//gamePlay->LoadGameMapFromFile(levelList[GameVars->getCurrentLevel()]->getFile());
	mInterface.GetCurrentMapLogicBank();
	mInterface.GetCurrentLevelBytes();

	//=====================================================
	// Register the gameBoard callback with the interface!
	mInterface.SetExecuteHandler(BE::CreateFunctionPointer3R(gamePlay, &gameBoard::interfaceHasFiredExecuteOrder));
	mInterface.SetAbortHandler(BE::CreateFunctionPointer0R(gamePlay, &gameBoard::interfaceHasFiredAbortOrder));
	mInterface.SetResetHandler(BE::CreateFunctionPointer0R(gamePlay, &gameBoard::interfaceHasFiredResetOrder));
	mInterface.SetHelpHandler(BE::CreateFunctionPointer0R(this, &playGame::launchHelpState));
	mInterface.SetSpeedUpHandler(BE::CreateFunctionPointer0R(this, &playGame::speedUp));
	mInterface.SetSlowDownHandler(BE::CreateFunctionPointer0R(this, &playGame::slowDown));

	gamePlay->SetInterfaceAdvanceHandler(BE::CreateFunctionPointer2R(&mInterface, &LogicInterface::CommandAdvanced));
	gamePlay->SetInterfaceReprogramHandler(BE::CreateFunctionPointer0R(&mInterface, &LogicInterface::ReprogramReached));
	gamePlay->SetInterfaceClearExecutionListHandler(BE::CreateFunctionPointer0R(&mInterface, &LogicInterface::ClearExecutionList));

	gamePlay->setState(GB_PREGAME);
	pregameRunning = false;
	gameSaved = false;

	//display a menu that shows info and contains advance, replay level and exit buttons

	blackImage = new oglTexture2D();
	blackImage->loadImage("black.png", 373, 75);
	
	// skip tutorial level button
	skipMenu = new MenuSys(5, 440, 200, 75, "blank.png", None, false);
	skipMenu->addButton("buttons\\skipthistutorial.png", "buttons\\skipthistutorialhover.png", "buttons\\skipthistutorialhover.png", CreateFunctionPointer0R(this, &playGame::advance), 30, 535, 200, 40);

	myMenu = new MenuSys(250, 50, "blank.png", None);
	myMenu->addButton("buttons\\advance.png", "buttons\\advancehover.png", "buttons\\advancehover.png", CreateFunctionPointer0R(this, &playGame::advance));
	myMenu->setLastButtonDimensions(100, 50);
	myMenu->setLastButtonPosition(350, backgroundImage->mY+backgroundImage->dY - 60);

	myMenu->addButton("buttons\\exit.png",	 "buttons\\exithover.png", "buttons\\exithover.png", CreateFunctionPointer0R(this, &playGame::exitGame));
	myMenu->setLastButtonDimensions(100, 50);
	myMenu->setLastButtonPosition(600, backgroundImage->mY+backgroundImage->dY - 60);

	myMenu->addButton("buttons\\replaylevel.png",	 "buttons\\replaylevelhover.png", "buttons\\replaylevelhover.png", CreateFunctionPointer0R(this, &playGame::replayLevel));
	myMenu->setLastButtonDimensions(100, 50);
	myMenu->setLastButtonPosition(475, backgroundImage->mY+backgroundImage->dY - 60);

	// compass stuff
	compassOffsetX = 763;
	compassOffsetY = 580;
	compass = new MenuSys(compassOffsetX, compassOffsetY, "blank.png", None, true);

	// rotate map left
	compass->addButton("compass\\rotateleftnormal.png", "compass\\rotatelefthover.png", "compass\\rotatelefthover.png", CreateFunctionPointer0R(this, &playGame::rotatemapleft));
	compass->setLastButtonDimensions(37, 25);
	compass->setLastButtonPosition(compassOffsetX+6, compassOffsetY+5);

	// zoom out
	compass->addButton("compass\\zoomoutnormal.png", "compass\\zoomouthover.png", "compass\\zoomouthover.png", CreateFunctionPointer0R(this, &playGame::zoomout));
	compass->setLastButtonDimensions(35, 25);
	compass->setLastButtonPosition(compassOffsetX+6+37, compassOffsetY+5);
	
	// zoom in
	compass->addButton("compass\\zoominnormal.png", "compass\\zoominhover.png", "compass\\zoominhover.png", CreateFunctionPointer0R(this, &playGame::zoomin));
	compass->setLastButtonDimensions(36, 25);
	compass->setLastButtonPosition(compassOffsetX + 6 + 37 + 35, compassOffsetY+5);

	// zoom in
	compass->addButton("compass\\rotaterightnormal.png", "compass\\rotaterighthover.png", "compass\\rotaterighthover.png", CreateFunctionPointer0R(this, &playGame::rotatemapright));
	compass->setLastButtonDimensions(38, 25);
	compass->setLastButtonPosition(compassOffsetX + 6 + 37 + 35 + 36, compassOffsetY+5);

	////////////////////////////////
	// up left
	compass->addButton("compass\\upleftnormal.png", "compass\\uplefthover.png", "compass\\uplefthover.png", CreateFunctionPointer0R(this, &playGame::panupleft));
	compass->setLastButtonDimensions(48, 45);
	compass->setLastButtonPosition(compassOffsetX+3, compassOffsetY+30+3);

	// up
	compass->addButton("compass\\upnormal.png", "compass\\uphover.png", "compass\\uphover.png", CreateFunctionPointer0R(this, &playGame::panup));
	compass->setLastButtonDimensions(51, 48);
	compass->setLastButtonPosition(compassOffsetX+48+3, compassOffsetY+30);

	// up right
	compass->addButton("compass\\uprightnormal.png", "compass\\uprighthover.png", "compass\\uprighthover.png", CreateFunctionPointer0R(this, &playGame::panupright));
	compass->setLastButtonDimensions(49, 45);
	compass->setLastButtonPosition(compassOffsetX+48+51+3, compassOffsetY+30+3);

	///////////////////////////////
	// left
	compass->addButton("compass\\leftnormal.png", "compass\\lefthover.png", "compass\\lefthover.png", CreateFunctionPointer0R(this, &playGame::panleft));
	compass->setLastButtonDimensions(51, 47);
	compass->setLastButtonPosition(compassOffsetX, compassOffsetY+33+48-3);

	// center
	compass->addButton("compass\\center.png", "compass\\centerhover.png", "compass\\centerhover.png", CreateFunctionPointer0R(this, &playGame::center));
	compass->setLastButtonDimensions(51, 47);
	compass->setLastButtonPosition(compassOffsetX+51, compassOffsetY+33+48-3);

	// right
	compass->addButton("compass\\rightnormal.png", "compass\\righthover.png", "compass\\righthover.png", CreateFunctionPointer0R(this, &playGame::panright));
	compass->setLastButtonDimensions(52, 47);
	compass->setLastButtonPosition(compassOffsetX+51+51, compassOffsetY+33+48-3);

	//////////////////////////////////
	// down left
	compass->addButton("compass\\downleftnormal.png", "compass\\downlefthover.png", "compass\\downlefthover.png", CreateFunctionPointer0R(this, &playGame::pandownleft));
	compass->setLastButtonDimensions(48, 46);
	compass->setLastButtonPosition(compassOffsetX+3, compassOffsetY+33+48+47-3);

	// down
	compass->addButton("compass\\downnormal.png", "compass\\downhover.png", "compass\\downhover.png", CreateFunctionPointer0R(this, &playGame::pandown));
	compass->setLastButtonDimensions(51, 49);
	compass->setLastButtonPosition(compassOffsetX+48+3, compassOffsetY+33+48+47-3);


	// down right
	compass->addButton("compass\\downrightnormal.png", "compass\\downrighthover.png", "compass\\downrighthover.png", CreateFunctionPointer0R(this, &playGame::pandownright));
	compass->setLastButtonDimensions(49, 46);
	compass->setLastButtonPosition(compassOffsetX+48+51+3, compassOffsetY+33+48+47-3);

	// pregame textinfo
	preGameTextOffsetX = 150;
	preGameTextOffsetY = 250;
	preGameTextSpacing = 45;
	preGameTitleDescSpacing = 18;

	Update();
	return true;
}

bool playGame::panleft()		{	gamePlay->panleft();	return true;}
bool playGame::panright()		{	gamePlay->panright();	return true;}
bool playGame::panup()			{	gamePlay->panup();		return true;}
bool playGame::pandown()		{	gamePlay->pandown();	return true;}
bool playGame::panupleft()		{	gamePlay->panupleft();	return true;}
bool playGame::panupright()		{	gamePlay->panupright();	return true;}
bool playGame::pandownleft()	{	gamePlay->pandownleft();return true;}
bool playGame::pandownright()	{	gamePlay->pandownright();return true;}
bool playGame::zoomout()		{	gamePlay->zoomout();	return true;}
bool playGame::zoomin()			{	gamePlay->zoomin();		return true;}
bool playGame::center()			
{	
	gamePlay->center();		
	gamePlay->resetZoom();
	return true;
}
bool playGame::rotatemapleft()	{	gamePlay->rotateMapLeft(); return true;}
bool playGame::rotatemapright()	{	gamePlay->rotateMapRight(); return true;}

void playGame::processMouse(int x, int y)
{
	switch(curState)
	{
	case GB_LOGICVIEW:
	case GB_EXECUTION:
	case GB_FINISHED:
	case GB_VIEWSCORE:
		gamePlay->processMouse(x, y);
		mInterface.processMouse(x, y);
		compass->processMouse(x, y);
		if(myMenu != NULL)
			myMenu->processMouse(x, y);
		skipMenu->processMouse(x,y);
		break;
	case GB_PREGAME:
	case GB_ROBOTDIED:
		break;
	default:
		break;
	}
}

void playGame::processMouseClick(int button, int state, int x, int y)
{
	switch(curState)
	{
	case GB_LOGICVIEW:
	case GB_EXECUTION:
	case GB_FINISHED:
	case GB_VIEWSCORE:
		gamePlay->processMouseClick(button, state, x, y);
		mInterface.processMouseClick(button, state, x, y);
		compass->processMouseClick(button, state, x, y);
		if(curState == GB_VIEWSCORE)
		{
			if(myMenu != NULL)
				myMenu->processMouseClick(button, state, x, y);
		}
		skipMenu->processMouseClick(button, state, x, y);
		break;
	case GB_PREGAME:
	case GB_ROBOTDIED:
		break;
	default:
		break;
	}
}

void playGame::keyboardInput(unsigned char c, int x, int y)
{
	switch(c)
	{
	case ',':
	case '<':
		slowDown();
		break;
	case '.':
	case '>':
		speedUp();
		break;
	}

	switch(curState)
	{
	case GB_LOGICVIEW:
	case GB_EXECUTION:
		switch(c)
		{
		case 27:
			GSM->addGameState<PauseGameState>();
			this->setStatus(Passive);
			break;
		default:
			break;
		}
		gamePlay->keyboardInput(c, x, y);
		break;
	case GB_PREGAME:
		switch(c)
		{
		case 27:
			gamePlay->setState(GB_LOGICVIEW);
			//doneDead = false;
			break;
		default:
			break;
		}
		break;
	case GB_ROBOTDIED:
		switch(c)
		{
		case 27:
			gamePlay->setState(GB_LOGICVIEW);
			doneDead = true;
			break;
		default:
			break;
		}
		break;
	case GB_YOUWIN:
		switch(c)
		{
		case 27:
			this->setStatus(DeleteMe);
			GSM->addGameState<MainMenuState>();
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

playGame::~playGame()
{
	//below is memory management that may or may not work, we'll see (Oscar)

	vector<levelData*>::iterator itr = levelList.end();
	for (; itr != levelList.begin(); itr--)
	{
		delete(*itr);
		levelList.erase(itr);
	}
}

bool playGame::advance()
{
	// sets the game state to finished to allow advancement through mini state manager
	curState = GB_FINISHED;
	gamePlay->setState(curState);

	return true;
}

bool playGame::exitGame()
{
	//delete this state while creating a Main Menu State
	GSM->addGameState<MainMenuState>();
	this->setStatus(DeleteMe);

	return true;
}

bool playGame::replayLevel()
{
	// reset the state to pregame of the current level
	curState = GB_PREGAME;
	gamePlay->setState(curState);

	// reset the map for a fresh start for the replay
	gamePlay->resetMap();

	GameVars->updatePlayerFile();

	// is there a way to clear the instruction list?
	// find/use same code as when user clicks clear button on interface
	
	////////////////////
	// complete and total map reset
	mInterface.ClearExecutionList();
	mInterface.ResetExecutionMode();
	
	delete gamePlay;			
	gamePlay = new gameBoard();	

	int levelCounter = GameVars->getCurrentLevel();
	string tempString;
	tempString = GameVars->getFilename(GameVars->getCurrentLevel());
	gamePlay->LoadGameMapFromFile(tempString);
	mInterface.GetCurrentMapLogicBank();
	mInterface.GetCurrentLevelBytes();
	GameVars->commandsProcessed = 0;
	GameVars->totalCommandsProcessed = 0;
	curState = GB_PREGAME;
	GameVars->didYouKnowI++;
	if(GameVars->didYouKnowI == GameVars->didYouKnow.end())
	{
		GameVars->didYouKnowI = GameVars->didYouKnow.begin();
	}


	//=====================================================
	// Register the gameBoard callback with the interface!
	mInterface.SetExecuteHandler(BE::CreateFunctionPointer3R(gamePlay, &gameBoard::interfaceHasFiredExecuteOrder));
	mInterface.SetAbortHandler(BE::CreateFunctionPointer0R(gamePlay, &gameBoard::interfaceHasFiredAbortOrder));
	mInterface.SetResetHandler(BE::CreateFunctionPointer0R(gamePlay, &gameBoard::interfaceHasFiredResetOrder));
	mInterface.SetHelpHandler(BE::CreateFunctionPointer0R(this, &playGame::launchHelpState));
	mInterface.SetSpeedUpHandler(BE::CreateFunctionPointer0R(this, &playGame::speedUp));
	mInterface.SetSlowDownHandler(BE::CreateFunctionPointer0R(this, &playGame::slowDown));
	gamePlay->SetInterfaceAdvanceHandler(BE::CreateFunctionPointer2R(&mInterface, &LogicInterface::CommandAdvanced));
	gamePlay->SetInterfaceReprogramHandler(BE::CreateFunctionPointer0R(&mInterface, &LogicInterface::ReprogramReached));
	gamePlay->SetInterfaceClearExecutionListHandler(BE::CreateFunctionPointer0R(&mInterface, &LogicInterface::ClearExecutionList));


	return true;
}

void playGame::doEndGameDraw()
{
	if(endGamePics.size() == 0)
	{
		// load in alot of pictures
		int numPics = 45;
		oglTexture2D* tempPic;
		for(int x = 0; x < numPics; x++)
		{
			string filename;
			stringstream filenumber;
			// build a variable filename
			filenumber.clear();
			filenumber << x;
			filename = "ending\\frames\\ending00";
			filename += filenumber.str();
			filename += ".png";
			tempPic = new oglTexture2D;
			tempPic->loadImage(filename, 1024, 768);
			endGamePics.push_back(tempPic);
		}
		endGameAnimation = endGamePics.begin();
	}
	else
	{
		// iterate through the pictures drawing them
		(*endGameAnimation)->drawImage(0, 0);
		glColor3ub(0, 0, 0);
		GameVars->fontArial24.drawText(200,  80, "We apologize for the crappy ending,");
		GameVars->fontArial24.drawText(200, 120, "we needed to save space.");
		GameVars->fontArial24.drawText(200, 160, "Press ESC to return to the main menu.");
		endGameAnimation++;
		if(endGameAnimation == endGamePics.end())
			endGameAnimation = endGamePics.begin();
		timer = clock();
		startTime = clock();
		while(timer < startTime + 50)
		{
			timer = clock();
		}		
	}
}

void playGame::drawLevelInfo()
{
	string tempString;
	int playerCurrentLevel;

	int textOffsetX = 10;
	int textOffsetY = 10;
	int textSpacing = 20;
	int offsetAmt = 0;

	blackImage->mX = 0;
	blackImage->mY = 0;
	blackImage->drawImageFaded(0.75, 1024, 85);
	
	glColor3ub(255, 0, 0);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	//this wont work if we're coming in out of a level select
	playerCurrentLevel = GameVars->getCurrentLevel();

	stringstream levelNumText;

	levelNumText << "Level " << playerCurrentLevel << " - " << GameVars->getLevelName(playerCurrentLevel);
	offsetAmt += GameVars->fontArialRed14.drawText(textOffsetX, textOffsetY + offsetAmt*textSpacing, levelNumText.str(), 87);


	// description
	tempString = GameVars->getDesc(playerCurrentLevel);
	offsetAmt += GameVars->fontArialRed14.drawText(textOffsetX, textOffsetY + offsetAmt*textSpacing, tempString, 87);
	
}

bool playGame::launchHelpState()
{
	GSM->addGameState<helpScreenState>();
	return true;
}

bool playGame::speedUp()
{
	// get the current game speed
	int speed = gamePlay->getProcessSpeed();

	//speed up the process time by lowering the timer
	// make sure it doesn't get so fast the user can't tell what's going on
	if(speed >= 200)
	{
		speed -= 100;
	}
	else if(speed == 100)
	{
		speed = 50;
	}
	else if(speed == 50)
	{
		speed = 25;
	}
	else if(speed == 25)
	{
		speed = 10;
	}

	// set the speed to the newly altered value
	gamePlay->setProcessSpeed(speed);
	return true;
}

bool playGame::slowDown()
{
	// get the current game speed
	int speed = gamePlay->getProcessSpeed();

	//slow down the process time by raising the timer
	if(speed == 10)
	{
		speed = 25;
	}
	else if (speed == 25)
	{
		speed = 50;
	}
	else if(speed == 50)
	{
		speed = 100;
	}
	else
	{
		speed += 100;
	}

	// make sure it doesn't get too slow
	if(speed > 1000)
	{
		speed = 1000;
	}

	// set the speed to the newly altered value
	gamePlay->setProcessSpeed(speed);
	return true;
}
