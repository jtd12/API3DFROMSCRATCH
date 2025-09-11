#include"sounds.hpp"

soundSetup::soundSetup()
{
	music=new sound();
}

soundSetup::~soundSetup()
{
	delete music;
}

		
		
bool soundSetup::initAudio() {
    return music->initAudio();
}

bool soundSetup::loadSoundWav(const char* filename)
{

 return music->loadSoundWav(filename,25);
}

void soundSetup::loadSoundMus(const char* filename)
{

music->loadSoundMus(filename);
}

void soundSetup::playSoundWav()
{
	music->playSoundWav(25);
}


void soundSetup::playSoundMus()
{
	music->playSoundMus(60);
}
