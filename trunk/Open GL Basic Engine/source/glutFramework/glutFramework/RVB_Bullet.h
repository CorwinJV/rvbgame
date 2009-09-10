#ifndef RVB_BULLET_H
#define RVB_BULLET_H

#include "RVB_Map.h"

enum RVB_BulletType { pistol, shotty, riffle };

class RVB_Bullet
{
private:
	double	distanceTraveled;		// how far has the bullet traveled since being fired
	int		damage;					// how much damage is the bullet capable of doing
	double	direction;				// which direction is the bullet traveling?
	double	speed;					// how fast is the bullet traveling?
	double	range;					// how far can the bullet travel?
	RVB_BulletType type;			// what kind of bullet is being fired
	RVB_Map* bulletMap;				// pointer to the map

public:
	//	calculates the damage done by the bullet based off of the distance it has traveled
	//	and the initial damage it is capable of doing
	void	calcDamage(int damage_n, double range_n, double distanceTraveled_n);

	// constructor and destructor
	RVB_Bullet(int damage_n, double direction_n, double speed_n, double range_n, RVB_BulletType type_n);
	RVB_Bullet();
	~RVB_Bullet();

	//	setters
	void	setDamage(int damage_n);
	void	setDistanceTraveled(double distance_n);
	void	setBulletType(RVB_BulletType type_n);
	void	setDirection(double direction_n);
	void	setSpeed(double speed_n);
	void	setRange(double range_n);

	//	 getters
	double			getDistanceTraveled();
	int				getDamage();
	double			getSpeed();
	double			getDirecion();
	double			getRange();
	RVB_BulletType	getBulletType();
};
#endif		RVB_Bullet