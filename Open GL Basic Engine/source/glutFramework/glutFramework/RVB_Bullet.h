#ifndef RVB_BULLET_H
#define RVB_BULLET_H

#include "oglTexture2D.h"
#include "oglGameVars.h"
//#include "RVB_Map.h"

enum RVB_BulletType { pistol, shotty, riffle };

//forward declare Map
class RVB_Map;

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
	int		bulletXPos;				// bullet's position on the x-coordinate
	int		bulletYPos;				// bullet's position on the y-coordinate
	RVB_BulletType type;			// what kind of bullet is being fired
	oglTexture2D* bulletImage;		// what picture to use for the bullet

public:
	//	calculates the damage done by the bullet based off of the distance it has traveled
	//	and the initial damage it is capable of doing
	void	calcDamage(double damage_n, double range_n, double distanceTraveled_n);

	// constructor and destructor
	RVB_Bullet(double damage_n, double xSpeed, double ySpeed, double speed_n, double range_n, RVB_BulletType type_n);
	RVB_Bullet();
	~RVB_Bullet();

	bool draw(double scaleFactor);
	bool update();

	//	setters
	void	setDamage(double damage_n);
	void	setDistanceTraveled(double distance_n);
	void	setBulletType(RVB_BulletType type_n);
	void	setDirection(double direction_n);
	void	setSpeed(double speed_n);
	void	setRange(double range_n);
	void	setBulletPos(int xPos, int yPos);

	//	 getters
	double			getDistanceTraveled();
	double			getDamage();
	double			getSpeed();
	double			getDirecion();
	double			getRange();
	bool			getActive();
	RVB_BulletType	getBulletType();
};
#endif		RVB_Bullet_H