#include "rvb_weapon.h"


rvbWeapon::rvbWeapon()
{
}

rvbWeapon::~rvbWeapon()
{
}

// returns the damage the weapon does
int rvbWeapon::getDamage()				
{
	return damage;
}

// returns the range the weapon has
int rvbWeapon::getRange()	
{
	return range;
}

// returns how effective this weapon is at short range
int rvbWeapon::getEffectiveShortRange()	
{
	return effectiveShortRange;
}

// returns how effective this weapon is at medium range
int rvbWeapon::getEffectiveMediumRange()
{
	return effectiveMediumRange;
}

// returns how effective this weapon is at long range
int rvbWeapon::getEffectiveLongRange()
{
	return effectiveLongRange;
}

// checks the amount of ammo the clip can hold
int rvbWeapon::getClipSize()
{
	return clipSize;
}

// returns the total amount of ammo for this weapon
int rvbWeapon::getTotalAmmo()
{
	// add the ammo you have in the clip to whatever you have in reserve
	int totAmmo = clipAmmo + reserveAmmo;

	return totAmmo;
}

// takes ammo from reserve and moves it to ready
void rvbWeapon::reload()
{
	// set the ammo available to zero
	int ammoAvailable = 0;

	// make sure we have ammo to reload
	if(reserveAmmo > 0)
	{
		// if our reserve ammo is smaller than our clip, use whatever we have left
		if(reserveAmmo <= clipSize)
		{
			ammoAvailable = reserveAmmo;
		}
		else	// otherwise fill the clip
		{
			ammoAvailable = clipSize;
		}
		// remove however much ammo was used from the reserve cache
		reserveAmmo -= ammoAvailable;
	}
}

// adds the ammo you've picked up to the reserve ammo
void rvbWeapon::addAmmo(int quantity)
{
	// add however much ammo we've picked up to our reserve cache
	reserveAmmo += quantity;
}