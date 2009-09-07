#ifndef RVB_WEAPON_H
#define RVB_WEAPON_H


class rvbWeapon
{
public:
	rvbWeapon();
	~rvbWeapon();
	int getDamage();				// returns the damage the weapon does
	int getRange();					// returns the range the weapon has
	int getEffectiveShortRange();	// returns how effective this weapon is at short range
	int getEffectiveMediumRange();	// returns how effective this weapon is at medium range
	int getEffectiveLongRange();	// returns how effective this weapon is at long range
	int getClipSize();				// checks the amount of ammo the clip can hold
	int getTotalAmmo();				// returns the total amount of ammo for this weapon
	void reload();					// takes ammo from reserve and moves it to ready
	void addAmmo(int quantity);		// adds the ammo you've picked up to the reserve ammo

private:
	int reserveAmmo;				// how much ammo you have on hand for this weapon (not counting current clip)
	int clipAmmo;					// how much ammo in the clip
	int clipSize;					// how much ammo your clip can hold
	int damage;						// amount of damage this weapon does
	int range;						// the range of the weapon
	int effectiveShortRange;		// how effective this weapon is at short range
	int effectiveMediumRange;		// how effective this weapon is at medium range
	int effectiveLongRange;			// how effective this weapon is at long range
};


#endif