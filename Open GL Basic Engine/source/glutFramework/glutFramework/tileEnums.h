#ifndef TILEENUMS_H
#define TILEENUMS_H

#define numTileTypes 40
enum tileTypeEnum{ 
				TEmpty,			// 0 empty
				TDefault,		// 1 default floor tile
				TRaised1,		// 2 raised 1 level
				TRaised2,		// 3 raised 2 levels
				TRaised3,		// 4 raised 3 levels
				TRaised4,		// 5 raised 4 levels
				THalfTopL,		// 6 half top, left
				THalfTopR,		// 7 half top, right
				THalfBottomL,	// 8 half bottom, left
				THalfBottomR,	// 9 half bottom, right
				TGap,			// 10 gap
				TElectric,		// 11 electric tile
				TElectricTL,	// 12 electric wall top left
				TElectricTR,	// 13 electric wall top right
				TElectricBL,	// 14 electric wall bottom left
				TElectricBR,	// 15 electric wall bottom right
				TIce,			// 16 ice tile
				TWater,			// 17 water tile
				TSwitchTL,		// 18 switch top left
				TSwitchTR,		// 19 switch top right
				TSwitchBL,		// 20 switch bottom left
				TSwitchBR,		// 21 switch bottom right
				TSwitch,		// 22 switch whole square
				TProgramTL,		// 23 reprogram spot top left
				TProgramTR,		// 24 reprogram spot top right
				TProgramBL,		// 25 reprogram spot bottom left
				TProgramBR,		// 26 reprogram spot bottom right
				TProgram,		// 27 reprogram spot
				TBreakableTL,	// 28 breakable top leftle
				TBreakableTR,	// 29 breakable top right
				TBreakableBL,	// 30 breakable bottom left
				TBreakableBR,	// 31 breakable bottom right
				TSolid,			// 32 Solid Block
				TBreakable,		// 33 Solid Breakable Square
				TStart,			// 34 start square
				TEnd,			// 35 end square
				TDoorTL,		// 36 door top left
				TDoorTR,		// 37 door top right
				TDoorBL,		// 38 door bottom left
				TDoorBR,		// 39 door bottom right
				TTeleport,		// 40 teleporter
};
#endif

#define GLUT_LEFT_BUTTON		0
#define GLUT_MIDDLE_BUTTON		1
#define GLUT_RIGHT_BUTTON		2
#define GLUT_WHEEL_UP			3
#define GLUT_WHEEL_DOWN			4
#define GLUT_XBUTTON1			5
#define GLUT_XBUTTON2			6