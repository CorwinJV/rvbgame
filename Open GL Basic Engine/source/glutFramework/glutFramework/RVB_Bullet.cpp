#include "RVB_Bullet.h"

//	contructors and destructor
RVB_Bullet::RVB_Bullet(double xPos, double yPos, double damage_n, double xSpeed_n, double ySpeed_n, double speed_n, double range_n, RVB_BulletType type_n)
{
	bulletXPos = xPos;
	bulletYPos = yPos;
	damage = damage_n;
	xSpeed = xSpeed_n;
	ySpeed = ySpeed_n;
	speed = speed_n;
	range = range_n;
	type = type_n;
	active = true;

	switch(type)
	{
	case pistol:
		bulletImage = GameVars->pistolShotImg;
		break;
	case shotty:
		bulletImage = GameVars->shottyShotImg;
		break;
	case riffle:
		bulletImage = GameVars->riffleShotImg;
		break;
	default:
		break;
	}
}

RVB_Bullet::RVB_Bullet()
{
}

RVB_Bullet::~RVB_Bullet()
{
}

bool RVB_Bullet::draw(double scaleFactor, double tileWidth, int mapOffsetX, int mapOffsetY)
{
	if(type == shotty)
	{
		bulletImage->drawImage(8, 8,(bulletXPos * scaleFactor * tileWidth) + mapOffsetX, (bulletYPos * scaleFactor * tileWidth) + mapOffsetY);
	}
	else
	{
		bulletImage->drawImage(16, 16, (bulletXPos * scaleFactor * tileWidth) + mapOffsetX, (bulletYPos * scaleFactor * tileWidth) + mapOffsetY);
		cout << "Bullet xPos = " << bulletXPos << " yPos = " << bulletYPos << endl;
		cout << "Bullet xSpeed = " << xSpeed << " ySpeed = " << ySpeed << endl;
	}

	return true;
}

bool	RVB_Bullet::update()
{
	bulletXPos += xSpeed;
	bulletYPos += ySpeed;

	return true;
}

//	calculates the damage done by the bullet based off of the distance it has traveled
//	and the initial damage it is capable of doing
void	RVB_Bullet::calcDamage(double damage_n, double range_n, double distanceTraveled_n)
{
	double	damageTemp = damage_n;
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
void	RVB_Bullet::setDamage(double damage_n)
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

void	RVB_Bullet::setSpeed(double speed_n)
{
	speed = speed_n;
}

void	RVB_Bullet::setRange(double range_n)
{
	range = range_n;
}

void	RVB_Bullet::setBulletPos(int xPos, int yPos)
{
	bulletXPos = xPos;
	bulletYPos = yPos;
}

//	 getters
double	RVB_Bullet::getDistanceTraveled()
{
	return distanceTraveled;
}

double		RVB_Bullet::getDamage()
{
	return damage;
}

double	RVB_Bullet::getSpeed()
{
	return speed;
}

double	RVB_Bullet::getRange()
{
	return range;
}

bool	RVB_Bullet::getActive()
{
	return active;
}

RVB_BulletType	RVB_Bullet::getBulletType()
{
	return type;
}