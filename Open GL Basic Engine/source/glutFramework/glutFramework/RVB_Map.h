#ifndef RVB_MAP_H
#define RVB_MAP_H

#include "RVB_MapTile.h"
#include "RVB_Entity.h"
#include "RVB_Bullet.h"
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
	vector<vector<RVB_MapTile*>> mBoard;	// the vector of tiles that comprise the board
	
	double scaleFactor;						// the scale you are viewing the map at
	int	panRate;							// the rate at which you can scroll across the map
	int mapOffsetX;							// based in pixels, off current X, calculated off of scale
	int mapOffsetY;							// based in pixles, off current Y, calculated off of scale
	double cameraCenterX;					// the x position of the camera to stay centered on the map
	double cameraCenterY;					// the y position of the camera to stay centered on the map
	int mapWidth;							// the width of the map in tiles
	int mapHeight;							// the height of the map in tiles
	double currentTileWidth;				// depending on zoom, this is the current width of the tile (height as well)
	double currentTileHeight;				// depending on zoom, this is the current height of the tile
	double overallHeight;					// used to store the current height of the entire board
	double overallWidth;					// used to store the current width of the entire board
	static const int screenHeight = 768;	// fixed screen height in pixels
	static const int screenWidth = 1024;	// fixed screen width in pixles
	static const int tileWidth = 128;		// fixed width of each tile (since square it's height as well)
	static const int tileHeight = tileWidth;// fixed height of each tile ^^^^ 
	vector<RVB_Entity*> objectList;			// the vector of all entities on the board
	vector<RVB_Bullet*> bulletList;			// the vector containing any and all bullets currently on the screen
	float mRotation;						// degree of rotation


public: // Public Interface

	// drawing functions for the map position
	void center();
	void recalcBoard();

	// setters and getters
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
	RVB_Entity *getSelectableEntityAtGridCoord(int gridX, int gridY);
	bool isThereAnEntityAt(int xPos, int yPos);

	void Update();
	void Draw();
	void drawText();

	void addEntity(entityType newType, int xPos, int yPos, entityDirection newDirection);
	void toggleObstacle(int xPos, int yPos);
	TileType getTileTypeAt(int xPos, int yPos);
	bool isTileValidMove(int xPos, int yPos);
	bool areThereAnyEnemiesAt(int xPos, int yPos, entityType whatColorIAm);
	bool areThereAnyFriendsAt(int xPos, int yPos, entityType whatColorIAm);
	void updateBullets();
	void shotFired(RVB_Bullet* newBullet);
};


#endif // RVB_MAP_H