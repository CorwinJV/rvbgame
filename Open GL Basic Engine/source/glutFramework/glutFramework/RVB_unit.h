#ifndef RVB_SOLDIER_H
#define RVB_SOLDIER_H

#include "rvb_weapon.h"

enum soldierState{Fire, Cover, Defend, Move};
enum soldierDirection{UP, RIGHT, LEFT, DOWN};

class rvbSoldier
{
public:
	rvbSoldier();
	~rvbSoldier();
	void applyDamage(int damage);			// takes in the damage done and reduces health
	void pickUpAmmo(int amount);			// pickup ammo and add it to your cache
	void pickUpWeapon(rvbWeapon newWeapon);	// pickup weapon and add it to your inventory
	void retreat();							// get out of Dodge!
	void callForBackup();					// call to your team for backup
	void idTarget();						// scan area for targets
	void idFriendly();						// scan area for friendlies
	void getFlag();							// either grab the flag from the enemy base, or return your flag to your base
	void claimFlag();						// bring the flag back to your base to earn a point
	void seekCover();						// check area for obstacles to hide behind
	int checkMyAmmo();						// see how much ammo you currently have
	void seekMoreAmmo();					// seek out more ammo if you are low
	int getHealth();						// returns the soldier's health 
	soldierDirection getDirection();		// returns the direction the soldier is facing
	rvbWeapon getCurWeapon();					// returns the current weapon
	void changeWeapon();					// cycles through weapons in your inventory
	void changeDirection(soldierDirection newDirection);	// changes soldier's direction
	void setState(soldierState newState);	// sets soldier to the state passed in
	soldierState getState();				// gets the current state of the soldier

private:
	int health;						// amount of damage unit can take
	soldierDirection direction;		// direction the soldier is facing
	int	movementSpeed;				// how fast can the soldier move
	int armor;						// does the soldier have any additional armor
	rvbWeapon weaponCache[1];			// stores the weapons you have available
	rvbWeapon curWeapon;				// weapon on hand
	soldierState curState;			// current state of the soldier
};		

#endif