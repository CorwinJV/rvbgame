#include "OALSound.h"

OALSound::OALSound()
{
	init();

}

OALSound::~OALSound()
{
	ALFWShutdownOpenAL();

	ALFWShutdown();
}

void OALSound::init()
{
	ALFWInit();

	if (!ALFWInitOpenAL())
	{
		ALFWprintf("Failed to initialize OpenAL\n");
		ALFWShutdown();
		return;
	}

	alGenBuffers( 1, &uiBuffer );
}

void OALSound::loadSound(char* fileName)
{
	init();
	if (!ALFWLoadWaveToBuffer((char*)ALFWaddMediaPath(fileName), uiBuffer))
	{
		ALFWprintf("Failed to load %s\n", ALFWaddMediaPath(fileName));
	}
	else
	{
		//ALFWprintf("Loaded %s\n", fileName);
	}

}

void OALSound::playSound(char* fileName)
{
	loadSound(fileName);
	playSound();
}
void OALSound::playSound()
{
	// Generate a Source to playback the Buffer
    alGenSources( 1, &uiSource );

	// Attach Source to Buffer
	alSourcei( uiSource, AL_BUFFER, uiBuffer );

	// Play Source
    alSourcePlay( uiSource );
	ALFWprintf("Playing Source ");
		
	do
	{
		Sleep(100);
		ALFWprintf(".");
		// Get Source State
		alGetSourcei( uiSource, AL_SOURCE_STATE, &iState);
	} while (iState == AL_PLAYING);
	ALFWprintf("\n");

	// Clean up by deleting Source(s) and Buffer(s)
	alSourceStop(uiSource);
	alDeleteSources(1, &uiSource);
	alDeleteBuffers(1, &uiBuffer);

}