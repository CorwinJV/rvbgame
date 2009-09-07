//=========================
#include "oglSpriteFont.h"

oglSpriteFont::oglSpriteFont() : fontImage(NULL), charWidth(0),
	charHeight(0), charSize(0), numColumns(0), maxAsciiCount(127),
	fontLineSpacing(30)
{
	init();
}

oglSpriteFont::~oglSpriteFont()
{
	if(fontImage != NULL)
	{
		delete fontImage;
	}
}

oglSpriteFont::oglSpriteFont(const std::string &filename, unsigned int size)
{
	init();
	oglSpriteFont::open(filename, size);
}

void oglSpriteFont::open(const std::string &filename, unsigned int size)
{
	charSize = size * 1.5;
	if(fontImage != NULL)
	{
		delete fontImage;
	}
	fontImage = new oglTexture2D;
	fontImage->loadImage(filename, 1024, 512);
	fontImage->dX = 1024;
	fontImage->dY = 512;

	kernAmount = 35.37 + (64-charSize)*0.4;

	//kernAmount = charSize/64 - 1;
}

int oglSpriteFont::drawText(float x, float y, const std::string &str, int numColumns)
{
	// temp here, this is for figuring this shit out...

	//kernAmount = kernTweakb + (64-charSize)*kernTweak;
	// strip the string into chars
	//parseText(str);
	//int offsetX = x;
	//int offsetY = y;

	// returnNumRows is a return value that specifies the number of 
	// rows that were created when drawing text with a column spec.
	int returnNumRows = 0;

	if(fontImage != NULL)
	{
		// If numColumns was specified, then we're going to parse the string into
		// rows, then draw those rows of strings with an offset for each row.
		if(numColumns != 0)
		{
			std::vector<std::string*> outStrs;
			this->parseMeIntoRows(&outStrs, str, numColumns, true);
			
			// Now that the string has been parsed into a vector of subStrs
			// we can draw the parsed text for each element in the vector
			// (with an offset for new lines)
			std::vector<std::string*>::iterator itr = outStrs.begin();
			for(; itr < outStrs.end(); itr++)
			{
				int offsetAmt = std::distance(outStrs.begin(), itr);
				parseText((*(*itr)));
				// dividing by 1.5 because charSize has a modifier
				// due to pixels-to-points conversion
				drawParsedText(x, y + (offsetAmt * (charSize / 1.5)));
			}

			returnNumRows = outStrs.size();
		}
		else
		{
			parseText(str);
			drawParsedText(x, y);
			returnNumRows = 1;
		}
	}
	return returnNumRows;
}

void oglSpriteFont::drawParsedText(float x, float y)
{
	int offsetX = x;
	int offsetY = y;

	for(int i = 0; i < (int)drawMe.size(); i++)
	{
		// Check the current character (drawMe[i]) to see what 
		// image segment we need to draw in fontImage
		int currentCharacter = drawMe[i];

		// Draw the image segment
		fontImage->mX = offsetX + (i * charWidth) - (i*kernAmount);
		fontImage->mY = offsetY;

		int currentRow = currentCharacter / (numColumns);
		int currentColumn =  currentCharacter % (numColumns);
		double bW = (double)fontImage->dX;
		double bY = (double)fontImage->dY;
		
		fontImage->drawImageSegment((currentColumn * charWidth)/bW, (currentRow * charHeight)/bY,			     ((currentColumn * charWidth) + charWidth)/bW, (currentRow * charHeight)/bY, 
									(currentColumn * charWidth)/bW, ((currentRow * charHeight) + charHeight)/bY, ((currentColumn * charWidth) + charWidth)/bW, ((currentRow * charHeight) + charHeight)/bY, 
									1.0, charSize, charSize);
	}
}

void oglSpriteFont::parseMeIntoRows(std::vector<std::string*> *storageContainer, std::string stringToParse, int numCharsPerRow, bool autoFeed)
{
	storageContainer->clear();
	std::string* tempString = new std::string;
	//*tempString = stringToParse;
	int row = 0;

	if(!autoFeed)
	{
		for(int x = 0; x < (int)(stringToParse.length() / numCharsPerRow); x++)
		{
			*tempString = stringToParse.substr(numCharsPerRow*row, numCharsPerRow);
			storageContainer->push_back(tempString);
			tempString = new std::string;
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
			tempString = new std::string;
			curStart = curEnd + 1;

			if(curStart > (int)stringToParse.length())
			{
				done = true;
			}
		}
	}
}

void oglSpriteFont::init()
{
	charWidth = 64;
	charHeight = 64;
	numColumns = 16;
	charSize = 64;
	fontImage = NULL;
	kernAmount = 15;
	kernTweak = 0.1;
	kernTweakb = 0.1;
}

void oglSpriteFont::parseText(const std::string& parseMe)
{
	drawMe.clear();
	for(int x = 0; x < (int)parseMe.length(); x++)
	{
		drawMe.push_back(*(parseMe.substr(x, 1).c_str()));
	}
}