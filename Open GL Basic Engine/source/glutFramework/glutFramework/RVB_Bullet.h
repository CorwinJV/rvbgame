#ifndef RVB_BULLET_H
#define RVB_BULLET_H

#include "oglTexture2D.h"
#include "oglGameVars.h"
//#include "RVB_Map.h"

enum RVB_BulletType { pistol, shotty, riffle };

//forward declare Map
class RVB_Map;
class RVB_Entity;

class RVB_Bullet
{
private:
	double	distanceTraveled;		// how far has the bullet traveled since being fired
	double	damage;					// how much damage is the bullet capable of doing
	double	xSpeed;					// speed the bullet moves along the x coord
	double	ySpeed;					// speed the bullet moves along the y coord
	double	speed;					// how fast is the bullet traveling?
	double	range;					// how far can the bullet travel?
	bool	active;					// is the bullet active?
	bool	success;				// did the bullet successfully hit its target?
	double	bulletXPos;				// bullet's position on the x-coordinate
	double	bulletYPos;				// bullet's position on the y-coordinate
	double	sourceX;
	double	sourceY;
	double	rangeWhenFired;			// how far was the bullet from it's target when fired
	RVB_BulletType type;			// what kind of bullet is being fired
	oglTexture2D* bulletImage;		// what picture to use for the bullet
	RVB_Entity* whoFiredMe;			// points at the entity that spawned each bullet
	RVB_Entity* whoAmIBeingShotAt;	// points at the targeted entity

public:
	//	calculates the damage done by the bullet based off of the distance it has traveled
	//	and the initial damage it is capable of doing
	void	calcDamage(double damage_n, double range_n, double distanceTraveled_n);

	// constructor and destructor
	RVB_Bullet(double xPos, double yPos, double damage_n, double xSpeed, double ySpeed, double speed_n, 
				double range_n, RVB_BulletType type_n, RVB_Entity* whoShot, RVB_Entity* target, 
				double distanceFromTarget);
	RVB_Bullet();
	~RVB_Bullet();

	bool draw(double scaleFactor, double tileWidth, int mapOffsetX, int mapOffsetY);
	bool update();

	//	setters
	void	setDamage(double damage_n);
	void	setDistanceTraveled(double distance_n);
	void	setBulletType(RVB_BulletType type_n);
	void	setDirection(double direction_n);
	void	setSpeed(double speed_n);
	void	setRange(double range_n);
	void	setBulletPos(int xPos, int yPos);
	void	setActive(bool active_n);
	void	setSuccess(bool hitTarget);

	//	 getters
	double			getDistanceTraveled();
	double			getDamage();
	double			getSpeed();
	double			getDirecion();
	double			getRange();
	bool			getActive();
	double			getBulletXPos();
	double			getBulletYPos();
	bool			getSuccess();
	double			getRangeWhenFired();

	RVB_BulletType	getBulletType();
};
#endif		RVB_Bullet_H