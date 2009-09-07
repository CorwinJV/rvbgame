#include "RVB_MapTile.h"

RVB_MapTile::RVB_MapTile()
: mStatus(TT_NULL)
{
	
}

RVB_MapTile::RVB_MapTile(TileType initVal)
{
	mStatus = initVal;
}

RVB_MapTile::~RVB_MapTile()
{

}


void RVB_MapTile::setTileType(TileType newStatus)
{
	mStatus = newStatus;
}

TileType RVB_MapTile::getTileType()
{
	return mStatus;
}