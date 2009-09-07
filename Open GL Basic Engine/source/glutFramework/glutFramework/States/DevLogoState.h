#ifndef DEVLOGOSTATE_H
#define DEVLOGOSTATE_H

#include ".\..\GameState.h" 
#include ".\..\oglUtility.h"
//#include ".\..\pixmap.h"
#include ".\..\oglTexture2D.h"
#include <ctime>


class DevLogoState : public GameState
{
private:
	bool theyWantOut;
	//oglTexture2D* logo;
	int logo;
	oglTexture2D* logo1;
	oglTexture2D* logo2;
	oglTexture2D* logo3;
	clock_t timer;
	double opacity;
	int logostate;
	// logo 1
	double devlogofadeinstart;			double devlogofadeinend;
	double devlogofadeoutstart;			double devlogofadeoutend;

	// logo 2
	double logo2fadeinstart;			double logo2fadeinend;
	double logo2fadeoutstart;			double logo2fadeoutend;

	// logo 3
	double logo3fadeinstart;			double logo3fadeinend;
	double logo3fadeoutstart;			double logo3fadeoutend;

	double devlogoduration;
	double logo2duration;
	double logo3duration;

	double devlogopause;
	double logo2pause;
	double logo3pause;

	double pausebetween;

public:

	DevLogoState() {};
	DevLogoState(GameStateManager &Parent, int newID) : GameState(Parent, newID), logo(NULL)
	{
		theyWantOut = false;
		opacity = 0;
		logo1 = new oglTexture2D();
		logo1->loadImage("seeautslogo.png", 1024, 768);

		logo2 = new oglTexture2D();
		logo2->loadImage("logopresents.png", 1024, 768);

		logo3 = new oglTexture2D();
		logo3->loadImage("stupidrobot.png", 1024, 768);

		//logo->mX = -500;
		//logo->mY = 500;
		timer = 0;
		logostate = 0;

		double temptime = 0;

		// the duration numbers are how long for fade in and out
		 devlogoduration	= 2;
		 logo2duration		= 2;
		 logo3duration		= 2;

		 // pause is how long to holod the logoo on the screen at full opacity
		 devlogopause		= 1;
		 logo2pause			= 1;
		 logo3pause			= 1;

		 // how long of a pause between logos
		 pausebetween		= .25;
		 // end values settings

		 
		 devlogofadeinstart = 0;			 
		 devlogofadeinend = devlogoduration;

		 devlogofadeoutstart = devlogofadeinend + devlogopause;			 
		 devlogofadeoutend = devlogofadeoutstart + devlogoduration;

		// logo 2
		 logo2fadeinstart = devlogofadeoutend + pausebetween;			 
		 logo2fadeinend = logo2fadeinstart + logo2duration;
		 logo2fadeoutstart = logo2fadeinend + logo2pause;			 
		 logo2fadeoutend = logo2fadeoutstart + logo2duration;

		// logo 3
		 logo3fadeinstart = logo2fadeoutend + pausebetween;			 
		 logo3fadeinend = logo3fadeinstart + logo3duration;
		 logo3fadeoutstart = logo3fadeinend + logo3pause;			 
		 logo3fadeoutend = logo3fadeoutstart + logo3duration;
	}

	bool Update();
	bool Draw();
	~DevLogoState();
	void keyboardInput(unsigned char c, int x, int y);
};


#endif

