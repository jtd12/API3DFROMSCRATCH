#ifndef DEF_SOUNDS_HPP
#define DEF_SOUNDS_HPP
#include"../force3D/entities/sound/sound.hpp"
#include<iostream>
#include<vector>


class soundSetup
{	
	sound* music;

	public:
		soundSetup();
		~soundSetup();
		bool initAudio();
		bool loadSoundWav(const char* filename);
		void playSoundWav();
		void loadSoundMus(const char* filename);
		void playSoundMus();
		
};


#endif
