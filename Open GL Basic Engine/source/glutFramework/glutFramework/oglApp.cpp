#define fullscreen 0

/////////////////////////////////////////////////////////////
/// oglApp.cpp
///		Here's the definition for the OGL Framework.
/// Many of these functions can be overriden but should work
/// well with their default implementations.
///
/////////////////////////////////////////////////////////////

#include "oglApp.h"

//===================================================
// oglApp
//	Class constructor. Sets certain local variables 
// required for the creation of a window and 
// initialization of GLUT
// 
//===================================================
oglApp::oglApp(std::string title, int sizeX, int sizeY, int argc, char **argv)
{
	glutInitWindowSize(sizeX, sizeY);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	if(fullscreen != 1)
	{
		// windowed mode
		glutCreateWindow(title.c_str());
	}
	else
	{
		// full screen mode
		glutGameModeString("1024x768:32@60");
		glutEnterGameMode();
	}

	// Unfortunately the glut callback registration stuff is tricky
	// and has to be done from program.cpp
}



int oglApp::run()
{
	glutMainLoop();
	return 0;
}

void oglApp::drawScene(void)
{
	// Clear the screen to black here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glutSwapBuffers();
}

// glEnable2D and glDisable2D borrowed from a GameDev.net 
// tutorial & thread by: Brandon Fleming
void oglApp::glEnable2D()
{
   int vPort[4];
   glGetIntegerv(GL_VIEWPORT, vPort);

   glMatrixMode(GL_PROJECTION);
   glPushMatrix();
   glLoadIdentity();

   glOrtho(vPort[0], vPort[0]+vPort[2], vPort[1], vPort[1]+vPort[3], -1, 1);
   glMatrixMode(GL_MODELVIEW);
   glPushMatrix();
   glLoadIdentity();
   glTranslatef(0.375, 0.375, 0.);

   glPushAttrib(GL_DEPTH_BUFFER_BIT);
   glDisable(GL_DEPTH_TEST);
}

void oglApp::glDisable2D()
{
   glPopAttrib();
   glMatrixMode(GL_PROJECTION);
   glPopMatrix();   
   glMatrixMode(GL_MODELVIEW);
   glPopMatrix();       
}

void oglApp::processMouse(int x, int y)
{
	
}

void oglApp::processMouseClick(int button, int state, int x, int y)
{

}

void oglApp::keyboardInput(unsigned char c, int x, int y)
{

}