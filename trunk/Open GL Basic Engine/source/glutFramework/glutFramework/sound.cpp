#include "sound.h"
#include <iostream>

#define NULL			0
#define NUM_BUFFERS		8
#define NUM_SOURCES		1


soundEffect::soundEffect()
{
	currentBuffer = 0;
	Init();
}

soundEffect::~soundEffect()
{
	Shutdown();
}

int soundEffect::LoadAndAssignWAV(char* _fileName, ALuint _buffer)
{
	int			error;
	ALenum		format; 
	ALsizei		size;
	ALsizei		freq; 
	ALboolean	loop;
	ALvoid*		data;

	// Load in the WAV file from disk
	alutLoadWAVFile(_fileName, &format, &data, &size, &freq, &loop); 
	if ((error = alGetError()) != AL_NO_ERROR) 
	{ 
		std::cout << "Error Loading Wav File" << std::endl;
		return 0; 
	}

	// Copy the new WAV data into the buffer
	alBufferData(_buffer,format,data,size,freq); 
	if ((error = alGetError()) != AL_NO_ERROR) 
	{ 
		std::cout << "Error Copying Wav File into Buffer" << std::endl;
		std::cout << "Attempted to load file: " << _fileName << std::endl;
		return 0; 
	}

	// Unload the WAV file
	alutUnloadWAV(format,data,size,freq); 
	if ((error = alGetError()) != AL_NO_ERROR) 
	{ 
		std::cout << "Error Unloading Wav File" << std::endl;
		return 0;
	}

	return 1;
}

int soundEffect::Init(void)
{
	int			error;
	
	ALfloat		listenerPos[]={0.0,0.0,0.0};				// At the origin
	ALfloat		listenerVel[]={0.0,0.0,0.0};				// The velocity (no doppler here)
	ALfloat		listenerOri[]={0.0,0.0,-1.0, 0.0,1.0,0.0};	// LookAt then Up

	// Init openAL
	alutInit(0, NULL);	
	// Clear Error Code
	alGetError(); 

	// Create the buffers
	alGenBuffers(NUM_BUFFERS, buffers);
	if ((error = alGetError()) != AL_NO_ERROR) 
	{ 
		//DisplayOpenALError("alutUnloadWAV :", error);
		return 0;
	}
	
	// Load in the WAV and store it in a buffer
	if (!LoadAndAssignWAV("sound\\click.wav", buffers[0]))
	{
		// Error loading in the WAV so quit
		alDeleteBuffers(NUM_BUFFERS, buffers); 
		return 0;
	}

	// Load in the WAV and store it in a buffer
	if (!LoadAndAssignWAV("sound\\click.wav", buffers[1]))
	{
		// Error loading in the WAV so quit
		alDeleteBuffers(NUM_BUFFERS, buffers); 
		return 0;
	}

	// Generate Sources 
	alGenSources(NUM_SOURCES, source); 
	if ((error = alGetError()) != AL_NO_ERROR) 
	{ 
		//DisplayOpenALError("alutUnloadWAV :", error); 
		return 0; 
	}

	// Attach buffer 0 to our source
	alSourcei(source[0], AL_BUFFER, buffers[0]); 
	if ((error = alGetError()) != AL_NO_ERROR) 
	{ 
		//DisplayOpenALError("alSourcei :", error); 
		return 0; 
	}

	// Set Listener attributes
	// Position ... 
	alListenerfv(AL_POSITION,listenerPos); 
	if ((error = alGetError()) != AL_NO_ERROR) 
	{ 
		//DisplayOpenALError("alListenerfv :", error); 
		return 0;
	}
	// Velocity ... 
	alListenerfv(AL_VELOCITY,listenerVel); 
	if ((error = alGetError()) != AL_NO_ERROR) 
	{ 
		//DisplayOpenALError("alListenerfv :", error); 
		return 0;
	}
	// Orientation ... 
	alListenerfv(AL_ORIENTATION,listenerOri); 
	if ((error = alGetError()) != AL_NO_ERROR) 
	{ 
		//DisplayOpenALError("alListenerfv :", error); 
		return 0;
	}

	// Alll done...
	return 1;
}

int soundEffect::Init(char* _fileName)
{
		int			error;
	
	ALfloat		listenerPos[]={0.0,0.0,0.0};				// At the origin
	ALfloat		listenerVel[]={0.0,0.0,0.0};				// The velocity (no doppler here)
	ALfloat		listenerOri[]={0.0,0.0,-1.0, 0.0,1.0,0.0};	// LookAt then Up

	// Init openAL
	alutInit(0, NULL);	
	// Clear Error Code
	alGetError(); 

	// Create the buffers
	alGenBuffers(NUM_BUFFERS, buffers);
	if ((error = alGetError()) != AL_NO_ERROR) 
	{ 
		//DisplayOpenALError("alutUnloadWAV :", error);
		return 0;
	}
	
	// Load in the WAV and store it in a buffer
	if (!LoadAndAssignWAV(_fileName, buffers[0]))
	{
		// Error loading in the WAV so quit
		alDeleteBuffers(NUM_BUFFERS, buffers); 
		return 0;
	}

	// Load in the WAV and store it in a buffer
	if (!LoadAndAssignWAV(_fileName, buffers[1]))
	{
		// Error loading in the WAV so quit
		alDeleteBuffers(NUM_BUFFERS, buffers); 
		return 0;
	}

	// Generate Sources 
	alGenSources(NUM_SOURCES, source); 
	if ((error = alGetError()) != AL_NO_ERROR) 
	{ 
		//DisplayOpenALError("alutUnloadWAV :", error); 
		return 0; 
	}

	// Attach buffer 0 to our source
	alSourcei(source[0], AL_BUFFER, buffers[0]); 
	if ((error = alGetError()) != AL_NO_ERROR) 
	{ 
		//DisplayOpenALError("alSourcei :", error); 
		return 0; 
	}

	// Set Listener attributes
	// Position ... 
	alListenerfv(AL_POSITION,listenerPos); 
	if ((error = alGetError()) != AL_NO_ERROR) 
	{ 
		//DisplayOpenALError("alListenerfv :", error); 
		return 0;
	}
	// Velocity ... 
	alListenerfv(AL_VELOCITY,listenerVel); 
	if ((error = alGetError()) != AL_NO_ERROR) 
	{ 
		//DisplayOpenALError("alListenerfv :", error); 
		return 0;
	}
	// Orientation ... 
	alListenerfv(AL_ORIENTATION,listenerOri); 
	if ((error = alGetError()) != AL_NO_ERROR) 
	{ 
		//DisplayOpenALError("alListenerfv :", error); 
		return 0;
	}

	// Alll done...
	return 1;
}

int soundEffect::Update(void)
{
	int		sourceState;
	ALfloat sourcePos[] = {0.0f, 0.0f, 0.0f};

	//// Set the position of our source
	//alSourcefv(source[0], AL_POSITION, sourcePos);
	//// Play it
	//alSourcePlay(source[0]);
	//printf("Source played dead centre\n");
	//
	//// Wait till it has finished
	//alGetSourcei(source[0], AL_SOURCE_STATE, &sourceState);
	//while(sourceState == AL_PLAYING)
	//	alGetSourcei(source[0], AL_SOURCE_STATE, &sourceState);

	//// Set position of our source
	//sourcePos[0] = 5.0f;
	//alSourcefv(source[0], AL_POSITION, sourcePos);
	//// Play it
	//alSourcePlay(source[0]);
	//printf("Source played to the right\n");

	//// Wait till it has finished
	//alGetSourcei(source[0], AL_SOURCE_STATE, &sourceState);
	//while(sourceState == AL_PLAYING)
	//	alGetSourcei(source[0], AL_SOURCE_STATE, &sourceState);

	//// Set position of our source
	//sourcePos[0] = -5.0f;
	//alSourcefv(source[0], AL_POSITION, sourcePos);
	//// Play it
	//alSourcePlay(source[0]);
	//printf("Source played to the left\n");

	//// Wait till it has finished
	//alGetSourcei(source[0], AL_SOURCE_STATE, &sourceState);
	//while(sourceState == AL_PLAYING)
	//	alGetSourcei(source[0], AL_SOURCE_STATE, &sourceState);

	//// Set position of our source
	//sourcePos[0] = -10.0f;
	//alSourcefv(source[0], AL_POSITION, sourcePos);
	//// Play it
	//alSourcePlay(source[0]);
	//printf("Tracking source\n");

	//// Wait till it has finished (As im not taking into account frame time, this will be different on your PC!)
	//alGetSourcei(source[0], AL_SOURCE_STATE, &sourceState);
	//while(sourceState == AL_PLAYING)
	//{
	//	// Move the position
	//	sourcePos[0] += 0.0005f;
	//	alSourcefv(source[0], AL_POSITION, sourcePos);
	//	// Check the state
	//	alGetSourcei(source[0], AL_SOURCE_STATE, &sourceState);
	//}

	// Assign a new buffer
	alSourcei(source[0], AL_BUFFER, buffers[1]); 
	// Play it
	alSourcePlay(source[0]);
	printf("Stereo sample.  Non-Positional\n");

	// Wait till it has finished (As im not taking into account frame time, this will be different on your PC!)
	alGetSourcei(source[0], AL_SOURCE_STATE, &sourceState);
	while(sourceState == AL_PLAYING)
		alGetSourcei(source[0], AL_SOURCE_STATE, &sourceState);

	// Finished
	return 1;
}

int soundEffect::Shutdown(void)
{
	// Delete the sources and the buffers
	alDeleteSources(NUM_SOURCES, source);
	alDeleteBuffers(NUM_BUFFERS, buffers); 
	// Exit OpenAL
	alutExit();

	return 1;
}

void soundEffect::playSound()
{
	Init();
	Update();
	Shutdown();
}

void soundEffect::playSound(char* _fileName)
{
	Init(_fileName);
	Update();
	Shutdown();
}
