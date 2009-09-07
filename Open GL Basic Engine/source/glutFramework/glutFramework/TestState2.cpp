#include "TestState2.h"
#include <iostream>
#include "GameStateManager.h"

TestState2::TestState2() : GameState() // constructor
{
}

TestState2::TestState2(GameStateManager &Parent, int newID) : GameState() // constructor
{
	GSM = &Parent;
	stateID = newID;
    //graphics = new GraphicsDeviceManager(this);
    //content = new ContentManager(Services);
    // graphics.IsFullScreen = true;

	img = new pixmap("awesome.bmp");

    initialize();
}
TestState2::~TestState2()
{
	delete img;
}

bool TestState2::Update()
{
    int vPort[4];
    glGetIntegerv(GL_VIEWPORT, vPort);

	img->mX += 2;
	if(img->mX > vPort[2])
		img->mX = 0;

	img->mY += 2;
	if(img->mY > vPort[3])
		img->mY = 0;

	//std::cout << "TestState2 - ID:" << stateID << " img->mX = " << img->mX  << endl;

	if(img->mX >= 800)
	{
		//std::cout << "TestState 2 - ID: " << stateID << " Setting currentStatus to DeleteMe" << std::endl;
		currentStatus = DeleteMe;
	}

	return true;
}

bool TestState2::Draw()
{
	if(img != NULL)
		img->drawPixMap();

	return true;
}