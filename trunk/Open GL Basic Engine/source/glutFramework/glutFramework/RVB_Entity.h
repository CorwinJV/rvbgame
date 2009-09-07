#ifndef RVB_ENTITY_H
#define RVB_ENTITY_H

#include <vector>
#include "RVB_Path.h"
#include <ctime>

using namespace std;

enum entityDirection{ NORTH, NORTHEAST, EAST, SOUTHEAST, SOUTH, SOUTHWEST, WEST, NORTHWEST };

// circular dependancies require circular referencing and circular declarations
class RVB_Map;
//class RVB_Path;
// circular dependancies require circular referencing and circular declarations

class RVB_Entity
{
public:
	//RVB_Entity();
	RVB_Entity(entityType newType, int newX, int newY, entityDirection newDirection, RVB_Map* parentBoard, vector<RVB_Entity*>* boardObjectList);
	~RVB_Entity();

	void Update();
	void Draw(int tileWidth, double scaleFactor, int mapOffsetX, int mapOffsetY);
	void generatePath();
	int getXPos();
	int getYPos();
	int getTargetX();
	int getTargetY();
	void clearTarget();
	void setTarget(int newX, int newY);
	void setPosition(int newX, int newY);

private:
	int xPos;
	int yPos;
	int targetX;
	int targetY;
	int health;
	entityDirection facingDirection;

	clock_t timeOfLastUpdate;
	clock_t timeSinceLastUpdate;
	double timeStep;

	entityType type;
	RVB_Map* board;
	vector<RVB_Entity*>* objectList;

	rvbPath* myPath;
	
};


#endif // RVB_ENTITY_H