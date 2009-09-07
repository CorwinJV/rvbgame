//#include "oglApp.h"
#include "projekt1App.h"
#include <IL\il.h>

// App object
oglApp* newApp = NULL;

// Function prototypes
void doRendering( void );
void processMouse( int, int ) ;
void processMouseClick(int button, int state, int x, int y);
void keyboardInput(unsigned char c, int x, int y);
// mouse and keyboard stuff

int main(int argc, char **argv)
{

	newApp = new projekt1App("Projekt_01", 1024, 768, argc, argv);
	glutDisplayFunc(doRendering);
	glutIdleFunc(doRendering);
	glutPassiveMotionFunc(processMouse);
	glutMotionFunc(processMouse);
	glutMouseFunc(processMouseClick);
	glutKeyboardFunc(keyboardInput);

	// We're using DevIL library for image loading support
		// So initialize it, will ya?
	ilInit();
	glEnable(GL_TEXTURE_2D);
	gluOrtho2D(0.0, 1024.0, 768.0, 0.0);
	glClearDepth(0.0f);
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Run the game...
	newApp->run();
}

void doRendering( void )
{
	newApp->updateScene();
	newApp->drawScene();
}

void processMouse(int x, int y)
{
	newApp->processMouse(x, y);
}

void processMouseClick(int button, int state, int x, int y)
{
	newApp->processMouseClick(button, state, x, y);
}

void keyboardInput(unsigned char c, int x, int y)
{
	newApp->keyboardInput(c, x, y);
}
