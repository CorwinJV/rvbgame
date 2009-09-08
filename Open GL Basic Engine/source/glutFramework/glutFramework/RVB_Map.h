#ifndef RVB_MAP_H
#define RVB_MAP_H

#include "RVB_MapTile.h"
#include "RVB_Entity.h"
#include <vector>
using namespace std;

enum mapDirection { UP, DOWN, LEFT, RIGHT };

class RVB_Map
{
public: // Boilerplate Class Stuff
	RVB_Map();
	RVB_Map(int mapSizeX, int mapSizeY);
	~RVB_Map();

	void pan(enum mapDirection);
	void zoomIn();
	void zoomOut();
	RVB_Entity* cycleEntityDown(RVB_Entity* selectedEntity);
	RVB_Entity* cycleEntityUp(RVB_Entity* selectedEntity);

private: // Private Data Members
	vector<vector<RVB_MapTile*>> mBoard;
	
	double scaleFactor;
	int panRate;
	int mapOffsetX;
	int mapOffsetY;
	int mapWidth;
	int mapHeight;
	static const int tileWidth = 128;
	vector<RVB_Entity*> objectList;
	float mRotation;


public: // Public Interface
	double getScale();
	int getOffsetX();
	int getOffsetY();
	int getBoardWidth();
	int getBoardHeight();
	int getTileWidth();
	void setScale(double newS);
	void setOffsetX(int newX);
	void setOffsetY(int newY);
	void setOffset(int newX, int newY);
	void getGridCoord(int x, int y, int *returnGridX, int *returnGridY);
	void clearEntityTargets();
	void setAllEntityTargets(int mouseX, int mouseY);
	RVB_Entity *getSelectableEntity(int mouseX, int mouseY);
	bool RVB_Map::isThereAnEntityAt(int xPos, int yPos);


	void Update();
	void Draw();
	void drawText();

	void addEntity(entityType newType, int xPos, int yPos, entityDirection newDirection);
	void toggleObstacle(int xPos, int yPos);
	TileType getTileTypeAt(int xPos, int yPos);
	bool isTileValidMove(int xPos, int yPos);
};


#endif // RVB_MAP_H