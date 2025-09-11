#ifndef DEF_SOUND_HPP
#define DEF_SOUND_HPP
#include"../force3D/entities/sound/sound.hpp"
#include"C:\library\SDL2_mixer-2.5.3\x86_64-w64-mingw32\include\SDL2\SDL_mixer.h"
#include<iostream>
#include<vector>

extern float GPITCH;
inline float clamp(float val, float minVal, float maxVal);
inline void SDLCALL pitchEffect(int chan, void* stream, int len, void* udata);


class soundSetup
{	
	Mix_Chunk* chunk;
	Mix_Music* musique;

	
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
