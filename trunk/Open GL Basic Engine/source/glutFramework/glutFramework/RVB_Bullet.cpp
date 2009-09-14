#include "RVB_Bullet.h"

//	contructors and destructor
RVB_Bullet::RVB_Bullet(double xPos, double yPos, double damage_n, double xSpeed_n, double ySpeed_n, double speed_n, double range_n, RVB_BulletType type_n)
{
	// inherit the values passed in
	bulletXPos = xPos;
	bulletYPos = yPos;
	damage = damage_n;
	xSpeed = xSpeed_n / 5;
	ySpeed = ySpeed_n / 5;
	speed = speed_n;
	range = range_n;
	type = type_n;
	sourceX = bulletXPos;
	sourceY = bulletYPos;

	// set the bullet to active and its distance traveled to 0
	active = true;
	distanceTraveled = 0;

	// set the image type for which gun is shooting it
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
		bulletImage->drawImage(8 * scaleFactor * 3, 8 * scaleFactor* 3, (bulletXPos * scaleFactor * tileWidth) + mapOffsetX, (bulletYPos * scaleFactor * tileWidth) + mapOffsetY);
	}
	else
	{
		bulletImage->drawImage(16 * scaleFactor* 3, 16 * scaleFactor* 3, (bulletXPos * scaleFactor * tileWidth) + mapOffsetX, (bulletYPos * scaleFactor * tileWidth) + mapOffsetY);
		//cout << "Bullet xPos = " << bulletXPos << " yPos = " << bulletYPos << endl;
		//cout << "Bullet xSpeed = " << xSpeed << " ySpeed = " << ySpeed << endl;
	}

	return true;
}

bool	RVB_Bullet::update()
{
	// update/move the bullet along its x and y coordinates
	bulletXPos += xSpeed;
	bulletYPos += ySpeed;

	// check the distance traveled against absolute values of 
	// x and ySpeed in the case that they are negative numbers
	
	//distanceTraveled += (GameVars->dAbs(xSpeed) + GameVars->dAbs(ySpeed)); 
	distanceTraveled = GameVars->getDistanceToTarget(bulletXPos, bulletYPos, sourceX, sourceY);

	// see if the bullet has traveled to its range
	if(distanceTraveled >= range)
	{
		// if so deactive it
		active = false;
	}

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

	if(distanceTemp > (rangeTemp/2))
	{
		distanceTemp = distanceTemp - rangeTemp/2;
	}
	damage *= ((((rangeTemp/2) + 1) - distanceTemp)  / (rangeTemp / 2));
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

void	RVB_Bullet::setActive(bool active_n)
{
	active = active_n;
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

double RVB_Bullet::getBulletXPos()
{
	return bulletXPos;
}

double RVB_Bullet::getBulletYPos()
{
	return bulletYPos;
}