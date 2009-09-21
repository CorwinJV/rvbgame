#ifndef RVB_ENTITY_H
#define RVB_ENTITY_H

#include <vector>
#include "RVB_Path.h"
#include <ctime>
#include "RVB_weapon.h"
#include "ammoPack.h"
#include "RVB_Bullet.h"

using namespace std; // syphilis

enum entityDirection{ NORTH, NORTHEAST, EAST, SOUTHEAST, SOUTH, SOUTHWEST, WEST, NORTHWEST };

enum entityState { ATTACKING, TAKING_COVER, MOVING, IDLE, SCANNING };
enum higherState { CHASING, EVADING, ATTACKOPTIMAL, ATTACKMOVE, HIGHERMOVING, HIGHERATTACKING, HIGHERIDLE, DEAD, EXPLORE, SEEKANDDESTROY };

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
	bool entityCanSeeTargetAt(double targetXn, double targetYn);
	bool areThereAnyEntitiesInTheWay(double targetX_n, double targetY_n);
	bool isThereAFriendlyEntityInTheWay(double targetX_n, double targetY_n);
	higherState getHigherState();
	entityState	getLowerState();
	void receivedRequestForHelp(RVB_Entity* entityChasing);

	void setMoveTargetToHidingSpotFrom(RVB_Entity* someEntity);				// sets move target to a hiding spot, otherwise sets move target to -1 if no hiding spot found
	void setMoveTargetToVisibleHidingSpotFrom(RVB_Entity* sometEntity);		// sets move target to a hiding spot, otherwise sets move target to -1 if no hiding spot found
	
private:
	double xPos;						// the entities current X position
	double yPos;						// the entities current Y position
	double targetX;						// the X position of the target entity
	double targetY;						// the Y position of the target entity
	int health;							// the entity's health
	entityDirection facingDirection;
	bool canIShoot;						// wont let the gun shoot unless firing and reload time limits are satisfied

	// timer variables
	clock_t timeOfLastUpdate;
	clock_t timeSinceLastUpdate;
	clock_t timeOfLastFire;
	clock_t timeSinceLastFire;
	clock_t timeOfLastReload;
	clock_t timeSinceLastReload;
	double timeStep;

	entityType type;					// which side (red or blue) is the entity on
	RVB_Map* board;
	vector<RVB_Entity*>* objectList;
	vector<RVB_Bullet*>  shotsFired;	// a vector that keeps track of success rate of shots fired

	// =================
	// Smooth Movement
	bool doneMoving;
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
	void doEnemyScan();						// looks for an enemy and targets the closest one
	bool isAnyoneLookingAtMe();				// checks if there's any enemies that are targetting me
	bool isAnyoneLookingAtMeThatICanSee();	// checks if there's any enemies that are targetting me that i can actually see
	RVB_Entity* whoIsLookingAtMe();			// lets the target know who's looking at them
	bool isAnyoneChasingMe();				// checks if there's any enemies that are chasing me
	bool isAnyoneChasingMeThatICanSee();	// checks if there's any enemies thar are chasing me that i can see
	RVB_Entity* whoIsChasingMe();			// lets the target know who's chasing them
	bool isMyTargetChasingMe();				// checks if my currently selected target is chasing me
	bool isThisEntityVisibleToMyTeam(RVB_Entity* someEntity);	// pass in an entity pointer it lets you know if that entity is visible to your team
	bool isThisSpotVisibleToMyTeam(double checkX, double checkY);	// pass in an x/y coord to see if that spot is visible to your team
	RVB_Entity* getEntityTarget();			// returns a pointer to your currently selected entity pointer

	bool scanned;		// internal shit for scanning
	int scanDelay;		// delay for scanning
	bool canStillSeeEnemy();				// finds out if you can still see your enemy
	void findExplorationTarget();			// finds a valid exploration target
	void runToNearestFriendlyUnit();		// sets movement target to the nearest friendly unit
};


#endif // RVB_ENTITY_H