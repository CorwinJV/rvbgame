#include "RVB_Bullet.h"

//	contructors and destructor
RVB_Bullet::RVB_Bullet(int damage_n, double direction_n, double speed_n, double range_n, RVB_BulletType type_n)
{
	damage = damage_n;
	direction = direction_n;
	speed = speed_n;
	range = range_n;
	type = type_n;
}

RVB_Bullet::RVB_Bullet()
{
}

RVB_Bullet::~RVB_Bullet()
{
}

//	calculates the damage done by the bullet based off of the distance it has traveled
//	and the initial damage it is capable of doing
void	RVB_Bullet::calcDamage(int damage_n, double range_n, double distanceTraveled_n)
{
	int		damageTemp = damage_n;
	double	distanceTemp = distanceTraveled_n;
	double  rangeTemp = range_n;

	distanceTemp = distanceTemp - (rangeTemp/2);

	if(distanceTemp < 1)
	{
		distanceTemp = 1;
	}

	damage = ((((rangeTemp/2) + 1) - distanceTemp)  / (rangeTemp / 2));
}

//	setters
void	RVB_Bullet::setDamage(int damage_n)
{
	damage = damage_n;
}

void	RVB_Bullet::setDistanceTraveled(double distance_n)
{
	distanceTraveled = distance_n;
}

void	RVB_Bullet::setBulletType(RVB_BulletType type_n)
{
	type = type_n;
}

void	RVB_Bullet::setDirection(double direction_n)
{
	direction = direction_n;
}

void	RVB_Bullet::setSpeed(double speed_n)
{
	speed = speed_n;
}

void	RVB_Bullet::setRange(double range_n)
{
	range = range_n;
}

//	 getters
double	RVB_Bullet::getDistanceTraveled()
{
	return distanceTraveled;
}

int		RVB_Bullet::getDamage()
{
	return damage;
}

double	RVB_Bullet::getSpeed()
{
	return speed;
}

double	RVB_Bullet::getDirecion()
{
	return direction;
}

double	RVB_Bullet::getRange()
{
	return range;
}

RVB_BulletType	RVB_Bullet::getBulletType()
{
	return type;
}