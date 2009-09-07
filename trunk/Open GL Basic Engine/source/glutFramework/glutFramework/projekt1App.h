#ifndef PROJEKT1APP_H
#define PROJEKT1APP_H

#include "oglApp.h"
#include "GameStateManager.h"

class projekt1App : public oglApp
{
public:
	projekt1App(std::string title, int sizeX, int sizeY, int argc, char **argv);
	~projekt1App();
	void updateScene(void);
	void drawScene(void);
	void processMouse(int x, int y);
	void processMouseClick(int button, int state, int x, int y);
	void keyboardInput(unsigned char c, int x, int y);

private:
	void initOpenGL();
	
private:
	GameStateManager myStateManager;
};

#endif
