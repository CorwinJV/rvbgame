#ifndef OGLSOUNDSINGLETON_H
#define OGLSOUNDSINGLETON_H

//=======================================
// Singleton For Loading & Playing Sounds
//    This class is based on
// the singleton design pattern
// popularized in the GoF book:
// "Design Patterns".
// - CJV

#define SoundSys oglSoundSingleton::Instance();

class oglSoundSingleton
{
public:
	static oglSoundSingleton* Instance();

protected:
	oglSoundSingleton();
	oglSoundSingleton(const oglSoundSingleton&);
	oglSoundSingleton& operator= (const oglSoundSingleton&);
private:
	static oglSoundSingleton* pinstance;

	//===============================================
	// Non singleton functions and data members here
	//===============================================
};


#endif // OGLSOUNDSINGLETON_H