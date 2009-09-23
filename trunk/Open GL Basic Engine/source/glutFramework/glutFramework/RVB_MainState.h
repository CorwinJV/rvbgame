#ifndef RVB_MAINSTATE_H
#define RVB_MAINSTATE_H

#include "GameState.h"
#include "oglGameVars.h"
#include "RVB_Map.h"
#include <iostream>

class RVB_MainState : public GameState
{
public:
	RVB_MainState(GameStateManager &Parent, int newID) 
		: GameState(Parent, newID), isLeftMouseDown(false),
		selectedEntityList(NULL)
	{
		init();
	}
	~RVB_MainState();

private:
	static const int mapSize = 20;
	void init();
	RVB_Map* currentMap;

	int panRate;
	int clickedX;
	int clickedY;
	int curMouseX;
	int curMouseY;
	bool isLeftMouseDown;

	int tileWidth;
	double scaleFactor;
	double mapOffsetX;
	double mapOffsetY;

	//RVB_Entity* selectedEntity;
	vector<RVB_Entity*>* selectedEntityList;

	void loadObstacles();

public:
	bool Update();
	bool Draw();

	void drawEntityTargets();
	void drawEntitySelectionCircles();
	void drawMoveTargets();
	void drawMultiSelectionBox();

	void toggleSelectedEntity(RVB_Entity* entityToToggle);
	void setSelectedEntitiesTargets(int tX, int tY);
	void setSelectedEntitiesEnemyTargets(int tX, int tY);
	void setSelectedEntitiesState(higherState newState);
	
	void processMouse(int x, int y);
	void processMouseClick(int button, int state, int x, int y);
	void keyboardInput(unsigned char c, int x, int y);

	void hurtSelectedEntities();
	void healSelectedEntities();	
};


#endif // RVB_MAINSTATE_H