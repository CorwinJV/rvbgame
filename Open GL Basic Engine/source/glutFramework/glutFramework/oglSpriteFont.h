#ifndef OGLSPRITEFONT_H
#define OGLSPRITEFONT_H

//=================================
// oglSpriteFont
//    This class is designed to
// meet most interfaces set forth
// by the GLFT_Font class. 
// It's a Sprite Font class that
// allows us to load in custom
// sprite-fonts (non-true-type)
// and draw them on the screen.
// -Corwin VanHook
// 4/8/09
//
#include <string>
#include <vector>
#include "oglTexture2D.h"

class oglSpriteFont
{
public:
	oglSpriteFont();
	oglSpriteFont(const std::string& filename, unsigned int size);

	~oglSpriteFont();

	void init();

	void open(const std::string& filename, unsigned int size);
    int drawText(float x, float y, const std::string& str, int numColumns = 0);
	void drawParsedText(float x, float y);
	void parseMeIntoRows(std::vector<std::string*> *storageContainer, std::string stringToParse, int numCharsPerRow, bool autoFeed);

	// drawText
	// open
	void more()
	{
		//kernTweak += 0.01;
		//std::cout << "kernTweak = " << kernTweak << "   kernTweakb = " << kernTweakb << std::endl;
	}
	void less()
	{
		//kernTweak -= 0.01;
		//std::cout << "kernTweak = " << kernTweak << "   kernTweakb = " << kernTweakb << std::endl;
	}
	void lessb()
	{
		//kernTweakb += 0.01;
		//std::cout << "kernTweak = " << kernTweak << "   kernTweakb = " << kernTweakb << std::endl;
	}
	void moreb()
	{
		//kernTweakb -= 0.01;
		//std::cout << "kernTweak = " << kernTweak << "   kernTweakb = " << kernTweakb << std::endl;
	}

private:
	std::vector<char> drawMe;

	oglTexture2D* fontImage;
	int charWidth;
	int charHeight;
	int charSize;
	int numColumns;
	int maxAsciiCount;
	int fontLineSpacing;
	double kernAmount;
	double kernTweak;
	double kernTweakb;

	void parseText(const std::string& parseMe);
};
#endif
