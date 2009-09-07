#ifndef OALSOUND_H
#define OALSOUND_H

#include "OpenALSound\\Framework.h"

class OALSound
{
public:
	OALSound();
	~OALSound();

	void loadSound(char* fileName);
	void playSound(char* fileName);
	void playSound();	

private:
	// variables
	ALuint      uiBuffer;
	ALuint      uiSource;  
	ALint       iState;

	// functions
	void init();

};

#endif