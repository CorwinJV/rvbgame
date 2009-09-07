#include "oglUtility.h"
//#include "pixmap.h"
#include <cstring>
#include <iostream>
#include "mapTile.h"

// default constructor
//class mapTile
//{
//protected:
//	tileTypeEnum tileType;
//	bool isActive;
//	// pointer to whoever is standing on me

mapTile::mapTile()
{
	isActive = false;
	resetActiveStatus = isActive;
	tileType = TEmpty;
}
mapTile::mapTile(tileTypeEnum nType = TEmpty, bool nisActive = false)
{
	tileType = nType;
	isActive = nisActive;
	resetActiveStatus = isActive;
}
mapTile::~mapTile() {}
tileTypeEnum mapTile::getType()
{
	return tileType;
}

bool mapTile::setType(tileTypeEnum nType)
{
	tileType = nType;
	return true;
}

bool mapTile::getIsActive()
{
	return isActive;
}

bool mapTile::setActive(bool nactive)
{
	isActive = nactive;
	return true;
}

bool mapTile::toggleActive()
{
	if(isActive == true)
		isActive = false;
	else
		isActive = true;

	return true;
}

bool mapTile::update()
{

	return true;
}

void mapTile::setResetActive(bool newActive)
{
	resetActiveStatus = newActive;
}

void mapTile::resetActive()
{
	isActive = resetActiveStatus;
}