#ifndef MAPTILE_H
#define MAPTILE_H

#include "oglUtility.h"
//#include "pixmap.h"
#include <cstring>
#include <iostream>
#include "tileEnums.h"

class mapTile
{
protected:
	tileTypeEnum tileType;
	bool isActive;
	bool resetActiveStatus;
	// pointer to whoever is standing on me

public:
	mapTile();
	mapTile(tileTypeEnum nType, bool nisActive);
	~mapTile();
	tileTypeEnum getType();
	bool setType(tileTypeEnum nType);
	bool getIsActive();
	bool setActive(bool nactive);
	bool toggleActive();
	bool update();
	void resetActive();
	void setResetActive(bool newActive);
};

#endif