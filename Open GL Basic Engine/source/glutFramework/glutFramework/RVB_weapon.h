#ifndef RVB_WEAPON_H
#define RVB_WEAPON_H


class rvbWeapon
{
private:
	int damage;						// amount of damage this weapon does
	int range;						// the range of the weapon

	int clipAmmo;					// how much ammo in the clip
	int clipSize;					// how much ammo your clip can hold

	double closeRange;				// what is considered close/medium/long for this weapon
	double mediumRange;				
	double longRange;				

public:
	rvbWeapon();
	rvbWeapon(int damage_n, int range_n, int clipSize_n, int clipAmmo_n);
	~rvbWeapon();

	int getDamage();				// returns the damage the weapon does
	int getRange();					// returns the range the weapon has
	double getAccuracy(double distance);	// pass in the distance you are trying to fire at, it returns how accurate this is

	int getClipSize();				// returns the ammount of ammo the clip can hold
	int getAmmoLeftInClip();		// returns the ammount of ammo left in the clip
	
	int reload(int ammoLeft);		// you tell it how much ammo you have on you, it returns how much ammo you have left after reloading the clip
};


#endif