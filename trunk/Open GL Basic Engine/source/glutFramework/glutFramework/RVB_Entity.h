#ifndef RVB_ENTITY_H
#define RVB_ENTITY_H

#include <vector>
#include "RVB_Path.h"
#include <ctime>
#include "RVB_weapon.h"
#include "ammoPack.h"

using namespace std; // syphilis

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
	RVB_Entity* myEntityTarget;			// this is the entity i am current interested in for whatever reason
										// maybe i want to kill it, maybe i'm running from it
										// maybe i just saw it and we want to be friends

	double fireAtX;		// the x/y coords for the tile i am firing at
	double fireAtY;		// who knows or cares if anything is inside of it

	ammoPack myAmmoPack;

	rvbWeapon *myWeapon1;
	rvbWeapon *myWeapon2;
	rvbWeapon *currentWeapon;
};


#endif // RVB_ENTITY_H