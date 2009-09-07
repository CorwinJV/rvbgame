#include "TestState.h"
#include <iostream>
#include "GameStateManager.h"


TestState::TestState() : GameState() // constructor
{
}

TestState::TestState(GameStateManager &Parent, int newID) : GameState(Parent, newID)
{

	img = new pixmap("awesome.bmp");

	myMenu = new MenuSys(250, 50, "blankmenu.bmp", Auto);
	//myMenu = new MenuSys();
	myMenu->addButton("button1normal.bmp", "button1down.bmp", "button1over.bmp");
	myMenu->addButton("button2normal.bmp", "button2down.bmp", "button2over.bmp");
	myMenu->addButton("button3normal.bmp", "button3down.bmp", "button3over.bmp");
	myMenu->addButton("button4normal.bmp", "button4down.bmp", "button4over.bmp");
    initialize();
}
TestState::~TestState()
{
	delete img;
}

bool TestState::Update()
{
    int vPort[4];
    glGetIntegerv(GL_VIEWPORT, vPort);

	img->mX += 2;
	if(img->mX > vPort[2])
		img->mX = 0;

	if(img->mX % 50 == 0)
	{
		//if(GSM->getNumStates() < 7)
		{
#ifdef GSMWork
			std::cout << "TestState - attempting to add a new teststate2 " << GSM->getStateCount() << " - " << GSM->getNumStates() << std::endl;
#endif
			// create new temp vector
			// add state to vector
			// merge vectors
			//GSM->addGameState<TestState2>();
		}
	}

	img->mY += 2;

	if(img->mY > vPort[3])
		img->mY = 0;

	//std::cout << "TestState - ID:" << stateID << " img->mX = " << img->mX  << endl;

	myMenu->Update();

	return true;
}

bool TestState::Draw()
{
	if(img != NULL)
		img->drawPixMap();

	myMenu->Draw();

	return true;
}