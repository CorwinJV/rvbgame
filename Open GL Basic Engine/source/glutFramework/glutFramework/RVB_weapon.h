#ifndef RVB_WEAPON_H
#define RVB_WEAPON_H

#include "RVB_Bullet.h"

enum RVB_WeaponType { WEAPON_PISTOL, WEAPON_SHOTTY, WEAPON_RIFFLE };

class rvbWeapon
{
private:
	int damage;						// amount of damage this weapon does
	int range;						// the range of the weapon

	int clipAmmo;					// how much ammo in the clip
	int clipSize;					// how much ammo your clip can hold

	int firingRate;					// how quickly the gun can fire shots
	
	double closeRange;				// what is considered close/medium/long for this weapon
	double mediumRange;				
	double longRange;				
	RVB_WeaponType type;			// what kind of weapon is it

public:
	rvbWeapon();
	rvbWeapon(int damage_n, int range_n, int clipSize_n, int clipAmmo_n, RVB_WeaponType type_n, int fireRate);
	~rvbWeapon();

	// setters and getters
	int getDamage();				// returns the damage the weapon does
	int getRange();					// returns the range the weapon has
	double getAccuracy(double distance);	// pass in the distance you are trying to fire at, it returns how accurate this is
	int getClipSize();				// returns the ammount of ammo the clip can hold
	int getAmmoLeftInClip();		// returns the ammount of ammo left in the clip
	RVB_WeaponType getType();		// what kind of gun is it?
	int getFireRate();				// how fast can the gun fire?
	
	void setAmmoLeftInClip(int ammoLeft);	// sets the amount of ammo left

	
	
	// you tell it how much ammo you have on you, if you have any bullets in your clip, 
	// and what kind of gun you're using it returns how much ammo you have left after reloading the clip
	int reload(int ammoLeft, int ammoInClip, RVB_WeaponType type_n);		

	RVB_Bullet* shotFired(int xPos, int yPos, int targetX, int targetY);
};


#endif