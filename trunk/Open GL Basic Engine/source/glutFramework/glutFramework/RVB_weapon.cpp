#include "rvb_weapon.h"
#include <iostream>
	//int damage;						// amount of damage this weapon does
	//int range;						// the range of the weapon

	//int clipAmmo;					// how much ammo in the clip
	//int clipSize;					// how much ammo your clip can hold

	//double closeRange;				// what is considered close/medium/long for this weapon
	//double mediumRange;				
	//double longRange;

rvbWeapon::rvbWeapon()
{
	std::cout << "If you see this, someone done fucked up." << std::endl;
}

rvbWeapon::rvbWeapon(int damage_n, int range_n, int clipSize_n, int clipAmmo_n, RVB_WeaponType type_n)
{
	damage = damage_n;
	range = range_n;
	clipSize = clipSize_n;
	clipAmmo = clipAmmo_n;
	type = type_n;

	closeRange = range/3;
	mediumRange = 2*closeRange;
	longRange = range;
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

// pass in the distance you are trying to fire at, it returns how accurate this is
double rvbWeapon::getAccuracy(double distance)
{
	// does something interesting
	double accuracy = 0.01;

	// 90% close
	if((distance >= 0) && (distance <= closeRange))
	{
		accuracy = 90.0;
	}
	// 75% medium
	else if (( distance > closeRange) && (distance <= mediumRange))
	{
		accuracy = 75.0;
	}
	// 60% far
	else if (( distance > mediumRange) && (distance <= longRange))
	{
		accuracy = 60.0345;
	}

	return accuracy;  // better than returning inaccuracy
}

// returns the ammount of ammo the clip can hold
int rvbWeapon::getClipSize()
{
	return clipSize;
}

// returns the ammount of ammo left in the clip
int rvbWeapon::getAmmoLeftInClip()
{
	return clipAmmo;
}

// you tell it how much ammo you have on you, it returns how much ammo you have left after reloading the clip
int rvbWeapon::reload(int ammoLeft)
{
	int ammoPool = ammoLeft;

	if(clipAmmo < clipSize)
	{
		if(ammoPool >= (clipSize - clipAmmo))
		{
			ammoPool = ammoPool - (clipSize - clipAmmo);
			clipAmmo = clipSize;
		}
		else
		{
			clipAmmo += ammoPool;
			ammoPool = 0;
		}
	}
	return ammoPool;
}

RVB_Bullet* rvbWeapon::shotFired(int xPos, int yPos, int targetX, int targetY)
{
	double tempXSpeed = targetX - xPos;
	double tempYSpeed = targetY - yPos;

	RVB_Bullet* tempBullet = new RVB_Bullet(damage, tempXSpeed, tempYSpeed, 2 /*speed*/, range, (RVB_BulletType)type);

	return tempBullet;
}