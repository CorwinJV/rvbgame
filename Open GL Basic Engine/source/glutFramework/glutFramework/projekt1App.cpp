#include "projekt1App.h"
#include "oglGameVars.h"

// start additional state includes for testing purposes, don't NEED to be here, but are anyways
#include "bioWrapper.h"
#include "RPSGame.h"
#include "RVB_MainState.h"
// end additional state includes

#include <iostream>
using namespace std;

projekt1App::projekt1App(std::string title, int sizeX, int sizeY, int argc, char **argv)
	: oglApp(title, sizeX, sizeY, argc, argv)
{
	initOpenGL();

	//Load our starting state into the GameStateManager
	GameVars->Instance();
	myStateManager.addGameState<RVB_MainState>();
	//myStateManager.addGameState<bioWrapper>();			// ours now whores!
	//myStateManager.addGameState<RPSGame>();

	
}

void projekt1App::initOpenGL()
{
    glClearColor(0,0,0,0);//Define our background color

	//glMatrixMode(GL_PROJECTION); // Changes the current matrix to the projection matrix

	//// Sets up the projection matrix for a perspective transform
	//gluPerspective( 45,		// viewing angle
	//				1.0,	// aspect ratio
	//				10.0,	// near clip
	//				200.0);	// far clip

	//glMatrixMode(GL_MODELVIEW);

}

void projekt1App::updateScene(void)
{
	//std::cout << "project1App - Starting Update" << std::endl;
	myStateManager.Update();
}

void projekt1App::drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	myStateManager.Draw();
	glFlush();
	glutSwapBuffers();
}

void projekt1App::processMouse(int x, int y)
{
	myStateManager.processMouse(x, y);
}

void projekt1App::processMouseClick(int button, int state, int x, int y)
{
	myStateManager.processMouseClick(button, state, x, y);
}

void projekt1App::keyboardInput(unsigned char c, int x, int y)
{
	myStateManager.keyboardInput(c, x, y);
}
