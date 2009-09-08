//#include "rvb_unit.h"
//
//// takes in the damage done and reduces health
//void rvbSoldier::applyDamage(int damage)
//{
//	// apply the damage to soldier's health
//	health -= damage;
//}
//
//// pickup ammo and add it to your cache
//void rvbSoldier::pickUpAmmo(int amount)
//{
//}
//
//// pickup weapon and add it to your inventory
//void rvbSoldier::pickUpWeapon(rvbWeapon newWeapon)
//{
//}
//
//// get out of Dodge!
//void rvbSoldier::retreat()
//{
//}
//
//// call to your team for backup
//void rvbSoldier::callForBackup()
//{
//}
//
//// scan area for targets
//void rvbSoldier::idTarget()
//{
//}
//
//// scan area for friendlies
//void rvbSoldier::idFriendly()
//{
//}
//
//// either grab the flag from the enemy base, or return your flag to your base
//void rvbSoldier::getFlag()
//{
//}
//
//// bring the flag back to your base to earn a point
//void rvbSoldier::claimFlag()
//{
//}
//
//// check area for obstacles to hide behind
//void rvbSoldier::seekCover()
//{
//}
//
//// see how much ammo you currently have
//int rvbSoldier::checkMyAmmo()
//{
//	rvbWeapon weaponCheck;
//	int ammoCheck;
//
//	// see which weapon you have on hand
//	weaponCheck = getCurWeapon();
//	// check current weapons available ammo
//	ammoCheck = weaponCheck.getTotalAmmo();
//
//	return ammoCheck;
//}
//
//// seek out more ammo if you are low
//void rvbSoldier::seekMoreAmmo()
//{
//}
//
//// returns the soldier's health 
//int rvbSoldier::getHealth()
//{
//	return health;
//}
//
//// returns the direction the soldier is facing
//soldierDirection rvbSoldier::getDirection()
//{
//	return direction;
//}
//
//// returns the current weapon
//rvbWeapon rvbSoldier::getCurWeapon()
//{
//	return curWeapon;
//}
//
//// changes soldier's direction
//void rvbSoldier::changeDirection(soldierDirection newDirection)
//{
//	direction = newDirection;
//}
//
//// cycles through weapons in your inventory
//void rvbSoldier::changeWeapon()
//{
//	//rvbWeapon changeTo;
//
//	//// see what weapon we're currently using
//	//changeTo = getCurWeapon();
//
//	//// make sure we have two weapons on hand
//	//
//	//// make this if statement work
//	////if((weaponCache[0] != NULL) && (weaponCache[1] != NULL))
//	//if(true)
//	//{
//	//	// if the weapon you're currently using matches the weapon in your cache
//	//	if(weaponCache[0] == changeTo)
//	//	{
//	//		// then set the weapon in the second position
//	//		curWeapon = weaponCache[1];
//	//	}
//	//	else
//	//	{
//	//		// otherwise set it to the weapon in the first position
//	//		curWeapon = weaponCache[0];
//	//	}
//	//}
//}
//
//// sets soldier to the state passed in
//void rvbSoldier::setState(soldierState newState)
//{
//	curState = newState;
//}
//
//// gets the current state of the soldier
//soldierState rvbSoldier::getState()
//{
//	return curState;
//}