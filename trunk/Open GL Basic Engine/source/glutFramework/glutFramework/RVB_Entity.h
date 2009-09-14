#ifndef RVB_ENTITY_H
#define RVB_ENTITY_H

#include <vector>
#include "RVB_Path.h"
#include <ctime>
#include "RVB_weapon.h"
#include "ammoPack.h"

using namespace std; // syphilis

enum entityDirection{ NORTH, NORTHEAST, EAST, SOUTHEAST, SOUTH, SOUTHWEST, WEST, NORTHWEST };

enum entityState { ATTACKING, TAKING_COVER, MOVING, IDLE, SCANNING };
enum higherState { CHASING, EVADING, ATTACKMOVE, HIGHERMOVING, HIGHERATTACKING, HIGHERIDLE, DEAD };

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
	void applyDamage(int damage_n);
	void clearTarget();
	void pickUpAmmo(RVB_WeaponType ammoType, int ammoAmt_n);

	// setters and getters
	int getXPos();
	int getYPos();
	int getTargetX();
	int getTargetY();
	int getHealth();
	entityType getType();
	
	void setTarget(int newX, int newY);
	void setPosition(int newX, int newY);
	void setState(higherState newState);		// this is so the player (us) can give the entities commands whether they like it or not
	void setEnemyTarget(RVB_Entity *newTarget);
	void setHealth(int health_n);
	void drawEntityTarget(int tileWidth, double scaleFactor, double mapOffsetX, double mapOffsetY);
	void reload(int bullets);
	
private:
	double xPos;
	double yPos;
	double targetX;
	double targetY;
	int health;
	entityDirection facingDirection;

	clock_t timeOfLastUpdate;
	clock_t timeSinceLastUpdate;
	clock_t timeOfLastFire;
	clock_t timeSinceLastFire;
	clock_t timeOfLastReload;
	clock_t timeSinceLastReload;
	double timeStep;

	entityType type;
	RVB_Map* board;
	vector<RVB_Entity*>* objectList;

	//=================
	// Smooth Movement
	int movementSourceNodeX;
	int movementSourceNodeY;
	double completionPercentage;
	double defaultMovementIncr;

	rvbPath* myPath;
	RVB_Entity* myEntityTarget;			// this is the entity i am current interested in for whatever reason
										// maybe i want to kill it, maybe i'm running from it
										// maybe i just saw it and we want to be friends

	double fireAtX;						// the x/y coords for the tile i am firing at
	double fireAtY;						// who knows or cares if anything is inside of it

	// ammo variables
	ammoPack myAmmoPack;
	int reloadAmmo;
	int bulletsLeft;

	// weapon variables
	rvbWeapon *myWeapon1;				// first weapon you pickup/start with
	rvbWeapon *myWeapon2;				// second weapon you pickup/start with
	rvbWeapon *currentWeapon;			// weapon you are currently using

	higherState myHigherState;
	entityState myLowerState;

	void doMove();		// old update loop, now just for moving
	void performBrainFunction();
	void doEnemyScan();

	bool scanned;
	int scanDelay;
	bool canStillSeeEnemy();
};


#endif // RVB_ENTITY_H