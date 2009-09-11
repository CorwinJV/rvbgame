#include "oglGameVars.h"
#include<fstream>
#include<iostream>
#include <string>
#include <sstream>
#include <cmath>
using namespace std;

oglGameVars* oglGameVars::pinstance = 0;// initialize pointer

oglGameVars* oglGameVars::Instance () 
{
	if (pinstance == 0)  // is it the first call?
	{  
	  pinstance = new oglGameVars; // create sole instance
	}
	return pinstance; // address of sole instance
}

oglGameVars::oglGameVars()
{ 
	init();
}

//========================================
// Non singleton functions here wee.

void oglGameVars::init()
{
	// always had it, always will
	bacardi = new oglTexture2D();
	bacardi->loadImage("buttons/bacardi.png", imageSize, imageSize);

	//=======================
	// Marble Game
	marbleImage		 = new oglTexture2D();
	emptyImage		 = new oglTexture2D();
	selectedImage	 = new oglTexture2D();
	possibilityImage = new oglTexture2D();
	boardImage		 = new oglTexture2D();
	hintButton	     = new oglTexture2D();
	resetButton		 = new oglTexture2D();
	undoButton	     = new oglTexture2D();

	marbleImage->loadImage("marbleNormal.png", imageSize, imageSize);
	emptyImage->loadImage("emptyPeg.png", imageSize, imageSize);
	selectedImage->loadImage("marbleBlue.png", imageSize, imageSize);
	possibilityImage->loadImage("possibleMove.png", imageSize, imageSize);
	boardImage->loadImage("pyramid.png", 768, 768);
	undoButton->loadImage("undoButton.png", 128, 64);
	resetButton->loadImage("resetButton.png", 128, 64);
	hintButton->loadImage("hintButton.png", 128, 64);


	//=======================
	// RockPaperScissors Game
	RPSBG			= new oglTexture2D();
	RPSRock			= new oglTexture2D();
	RPSPaper		= new oglTexture2D();
	RPSScissors		= new oglTexture2D();
	playerWins		= new oglTexture2D();
	cpuWins			= new oglTexture2D();
	tieHand			= new oglTexture2D();

	RPSBG->loadImage("rpsBackground.png", 1024, 768);
	RPSRock->loadImage("rock.png", 128, 128);
	RPSPaper->loadImage("paper.png", 128, 128);
	RPSScissors->loadImage("scissors.png", 128, 128);
	playerWins->loadImage("playerWins.png", 128, 128);
	cpuWins->loadImage("cpuWins.png", 128, 128);
	tieHand->loadImage("tieHand.png", 128, 128);


	//=======================
	// RedvBlue Game
	rvbTile			= new oglTexture2D();
	rvbObstacle		= new oglTexture2D();
	rvbNullTile		= new oglTexture2D();
	rvbBlackTile	= new oglTexture2D();
	rvbEntityArrow	= new oglTexture2D();
	rvbEntityBlue	= new oglTexture2D();
	rvbEntityBlueN	= new oglTexture2D();
	rvbEntityRed	= new oglTexture2D();
	rvbEntityRedN	= new oglTexture2D();
	rvbEntitySelected = new oglTexture2D();
	rvbEntityTarget	= new oglTexture2D();
	rvbSelectionPix = new oglTexture2D();
	redPathImg = new oglTexture2D();
	bluePathImg = new oglTexture2D();
	rvbHeyYouWithTheFace = new oglTexture2D();
	redActive = new oglTexture2D();
	blueActive = new oglTexture2D();
	godActive = new oglTexture2D();
	fog = new oglTexture2D();
	redPixel = new oglTexture2D();
	greenPixel = new oglTexture2D();
	yellowPixel = new oglTexture2D();
	deadIcon = new oglTexture2D();
	healthBorder = new oglTexture2D();
	pistolShotImg = new oglTexture2D();
	riffleShotImg = new oglTexture2D();
	shottyShotImg = new oglTexture2D();


	rvbTile->loadImage("baseTile.png", 128, 128);
	rvbObstacle->loadImage("obstacle.png", 128, 128);
	rvbNullTile->loadImage("nullTile.png", 128, 128);
	rvbBlackTile->loadImage("blackTile.png", 128, 128);
	rvbEntityArrow->loadImage("entityArrow.png", 128, 128);
	rvbEntityBlue->loadImage("entityBlue.png", 128, 128);
	rvbEntityBlueN->loadImage("entityBlueN.png", 128, 128);
	rvbEntityRed->loadImage("entityRed.png", 128, 128);
	rvbEntityRedN->loadImage("entityRedN.png", 128, 128);
	rvbEntitySelected->loadImage("entitySelected.png", 128, 128);
	rvbEntityTarget->loadImage("entityTarget.png", 128, 128);
	rvbSelectionPix->loadImage("greenPixel.png", 1, 1); 
	redPathImg->loadImage("rvbRedPath.png", 128, 128);
	bluePathImg->loadImage("rvbBluePath.png", 128, 128);
	rvbHeyYouWithTheFace->loadImage("entityTargetEntity.png", 128, 128);
	redActive->loadImage("rvbRedActive.png", 1024, 64);
	blueActive->loadImage("rvbBlueActive.png", 1024, 64);
	godActive->loadImage("rvbGodActive.png", 1024, 64);
	fog->loadImage("rvbFog.png", 128, 128);
	redPixel->loadImage("redPixel.png", 1, 1);
	greenPixel->loadImage("greenPixel.png", 1, 1);
	yellowPixel->loadImage("yellowPixel.png", 1, 1);
	deadIcon->loadImage("rvbEntityDead.png", 128, 128);
	healthBorder->loadImage("rvbHealthBorder.png", 128, 128);
	pistolShotImg->loadImage("rvbBulletOrange.png", 16, 16);
	riffleShotImg->loadImage("rvbBulletYellow.png", 16, 16);
	shottyShotImg->loadImage("rvbBulletShotgun.png", 8, 8);
	

	loadFonts();
}

void oglGameVars::loadFonts()
{
	fontArial32.open	("fonts\\arialSpriteFontBlack.png", 32);
	fontArial24.open	("fonts\\arialSpriteFontBlack.png", 24);
	fontArial18.open	("fonts\\arialSpriteFontBlack.png", 18);
	fontArial16.open	("fonts\\arialSpriteFontBlack.png", 16);
	fontArial12.open	("fonts\\arialSpriteFontBlack.png", 14);
	fontDigital32.open  ("fonts\\digitalSpriteFontBlack.png", 32);
	fontDigital16.open  ("fonts\\digitalSpriteFontBlack.png", 16);
	fontDigital12.open	("fonts\\digitalSpriteFontBlack.png", 12);
	fontOurs.open		("fonts\\schwabenSpriteFontWhite.png", 64);
	fontArialRed12.open	("fonts\\arialSpriteFontRed.png", 12);
	fontArialRed14.open	("fonts\\arialSpriteFontRed.png", 14);
}

void oglGameVars::parseMeIntoRows(vector<std::string*> *storageContainer, std::string stringToParse, int numCharsPerRow, bool autoFeed)
{
	storageContainer->clear();
	string* tempString = new string;
	//*tempString = stringToParse;
	int row = 0;

	if(!autoFeed)
	{
		for(int x = 0; x < (int)(stringToParse.length() / numCharsPerRow); x++)
		{
			*tempString = stringToParse.substr(numCharsPerRow*row, numCharsPerRow);
			storageContainer->push_back(tempString);
			tempString = new string;
			row++;
		}
		*tempString = stringToParse.substr(numCharsPerRow*row, stringToParse.length()%numCharsPerRow);
		storageContainer->push_back(tempString);
	}
	else
	{
		// auto separation
		int curStart = 0;
		int curEnd = numCharsPerRow-1;

		bool done = false;
		while(!done)
		{
			// if we're not on row zero, check the first character of the line
			// if its a space, increment until its not a space
			if(curStart >= numCharsPerRow)
			{
				while(stringToParse.substr(curStart, 1) == " ")
				{
					curStart++;
				}
			}

			curEnd = curStart + numCharsPerRow-1;
			if(curEnd > (int)stringToParse.length())
			{
				curEnd = stringToParse.length();
			}

			// check the last character of the line
			// if its a space, leave it alone
			if(stringToParse.substr(curEnd, 1) == " ")
			{
				// leave curEnd alone
			}
			else
			{
				// if its a character, we have three possibilities
				// #1 it is a character with a space after it in which we're ok to cut and move on
				// #2 its a character with another character after it, in which case we have to go backward
				// and figure out where the cut spot is
				// #3 its a word that's so long it goes on for multiple lines in which case we have to backtrack and just cut anyways

				// #1
				if(curEnd < (int)stringToParse.length())
				{
					if(stringToParse.substr(curEnd+1, 1) == " ")
					{
						// do nothing
					}
					else
					{
						// find a new ending
						while((stringToParse.substr(curEnd, 1) != " ") && (curEnd < (int)stringToParse.length()))
						{
							curEnd++;
						}
						// #2
						if((curEnd - curStart) < numCharsPerRow*2)
						{
							curEnd = curStart + numCharsPerRow;
							// backtrack until we find a space
							while(stringToParse.substr(curEnd,1) != " ")
							{
								curEnd--;
							}
						}
						// #3
						else
						{
							// reset it and just cut
							curEnd = curStart + numCharsPerRow;
						}
					}
				}
			}

			// with the newly calculated curEnd lets chop it and move on
			if(curEnd > (int)stringToParse.length())
			{
				curEnd = stringToParse.length();
			}
		
			*tempString = stringToParse.substr(curStart, curEnd-curStart+1);
			storageContainer->push_back(tempString);
			tempString = new string;
			curStart = curEnd + 1;

			if(curStart > (int)stringToParse.length())
			{
				done = true;
			}
		}
	}
}


double oglGameVars::getDistanceToTarget(double xPos, double yPos, double targetXPos, double targetYPos)
{
	double length = abs(xPos - targetXPos);
	double height = abs(yPos - targetYPos);

	double hypotenuse = sqrt((length * length) + (height * height));

	return hypotenuse;
}