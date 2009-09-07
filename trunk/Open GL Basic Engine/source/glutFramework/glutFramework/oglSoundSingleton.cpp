#include "oglSoundSingleton.h"

oglSoundSingleton* oglSoundSingleton::pinstance = 0; // Initialize pointer

oglSoundSingleton* oglSoundSingleton::Instance()
{
	if(pinstance == 0)
	{
		pinstance = new oglSoundSingleton; // Create the sole instance if this is the first call
	}
	return pinstance;
}

oglSoundSingleton::oglSoundSingleton()
{

}