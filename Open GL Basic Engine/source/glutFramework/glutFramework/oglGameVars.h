#ifndef OGLGAMEVARS_H
#define OGLGAMEVARS_H
#include <fstream>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
// doesn't work for some reason on alot of systems
//#include "GLFT_Font.h"
#include "oglSpriteFont.h"
#include "oglTexture2D.h"

#define GameVars oglGameVars::Instance()
#define imageSize 100

#define entityVisionRadius 20

using namespace std;

//===========================
// oglGameVars
//    This class is based on
// the singleton design pattern
// popularized in the GoF book:
// "Design Patterns".
// - CJV

enum entityType{RED, BLUE, GOD};

enum marbleUI			// enumerator for the cases where the user selects a piece
{ 
	MUI_NULL,			// nothing to draw here, move along
	MUI_SELECTED,		// user selected a piece, show it
	MUI_POSSIBILITY		// user selected a piece, where can they put it?
};

class oglGameVars 
{
public:
    static oglGameVars* Instance();

protected:
    oglGameVars();
    oglGameVars(const oglGameVars&);
    oglGameVars& operator= (const oglGameVars&);
private:
    static oglGameVars* pinstance;

	//===============================================
	// Non singleton functions and data members here
	//===============================================
public:
	void						parseMeIntoRows(vector<std::string*> *storageContainer, std::string stringToParse, int numCharsPerRow, bool autoFeed = true);
	void						loadFonts();
	void						init();

	oglSpriteFont				fontArial32;
	oglSpriteFont				fontArial24;
	oglSpriteFont				fontArial18;
	oglSpriteFont				fontArial16;
	oglSpriteFont				fontArial12;
	oglSpriteFont				fontDigital64;
	oglSpriteFont				fontOurs;
	oglSpriteFont				fontDigital200;
	oglSpriteFont				fontDigital32;
	oglSpriteFont				fontDigital16;
	oglSpriteFont				fontDigital12;
	oglSpriteFont				fontArialRed12;
	oglSpriteFont				fontArialRed14;


	oglTexture2D*				bacardi;

	//=======================
	// Marble Game
	oglTexture2D*				emptyImage;			// no marble at this position
	oglTexture2D*				marbleImage;		// marble occupying the space
	oglTexture2D*				selectedImage;		// user selected a marble to move
	oglTexture2D*				possibilityImage;	// highlight empty spaces that the user can move to
	oglTexture2D*				boardImage;			// the board we're playing on
	oglTexture2D*				hintButton;			// image to display the hint button
	oglTexture2D*				undoButton;			// image to display the undo button
	oglTexture2D*				resetButton;		// image to display the reset button

	//=======================
	// RockPaperScissors Game
	oglTexture2D*				RPSBG;	// background for RPS game
	oglTexture2D*				RPSRock;	// rock image
	oglTexture2D*				RPSPaper;	// paper image
	oglTexture2D*				RPSScissors;	// scissors image
	oglTexture2D*				playerWins;
	oglTexture2D*				cpuWins;
	oglTexture2D*				tieHand;
	
	//=======================
	// RedvBlue Game
	oglTexture2D*				rvbTile;
	oglTexture2D*				rvbObstacle;
	oglTexture2D*				rvbNullTile;
	oglTexture2D*				rvbBlackTile;

	oglTexture2D*				rvbEntityArrow;
	oglTexture2D*				rvbEntityBlue;
	oglTexture2D*				rvbEntityBlueN;
	oglTexture2D*				rvbEntityRed;
	oglTexture2D*				rvbEntityRedN;
	oglTexture2D*				rvbEntitySelected;
	oglTexture2D*				rvbEntityTarget;
	oglTexture2D*				rvbSelectionPix;
	oglTexture2D*				rvbHeyYouWithTheFace;

	oglTexture2D*				redPathImg;
	oglTexture2D*				bluePathImg;
	oglTexture2D*				redActive;
	oglTexture2D*				blueActive;
	oglTexture2D*				godActive;
	oglTexture2D*				fog;

	oglTexture2D*				redPixel;
	oglTexture2D*				greenPixel;
	oglTexture2D*				yellowPixel;
	oglTexture2D*				healthBorder;

	oglTexture2D*				deadIcon;

	oglTexture2D*				pistolShotImg;
	oglTexture2D*				riffleShotImg;
	oglTexture2D*				shottyShotImg;



	double						getDistanceToTarget(double xPos, double yPos, double targetXPos, double targetYPos);
	double						dAbs(double something);
	entityType					mySide;

private:

};

#endif // OGLGAMEVARS_H
