#ifndef RVB_MAPTILE_H
#define RVB_MAPTILE_H

#include "oglGameVars.h"


enum TileType
{
	TT_NULL,
	TT_DEFAULT,
	TT_OBSTACLE
};

class RVB_MapTile
{
public: // Boilerplate Class Stuff
	RVB_MapTile();
	RVB_MapTile(TileType initVal);

	~RVB_MapTile();

private: // Private Data Members
	TileType mStatus;

public:	// Public Interface
	void setTileType(TileType newStatus);
	TileType getTileType();

};

#endif // RVB_MAPTILE_H