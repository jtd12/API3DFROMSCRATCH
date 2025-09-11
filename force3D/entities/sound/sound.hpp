#ifndef DEF_SOUND_HPP
#define DEF_SOUND_HPP
#include"C:\library\SDL2_mixer-2.5.3\x86_64-w64-mingw32\include\SDL2\SDL_mixer.h"
#include<iostream>
#include<vector>

extern float GPITCH;
inline float clamp(float val, float minVal, float maxVal);
inline void SDLCALL pitchEffect(int chan, void* stream, int len, void* udata);


class sound
{	
	Mix_Chunk* chunk;
	Mix_Music* musique;

	
	public:
		sound();
		~sound();
		bool initAudio();
		bool loadSoundWav(const char* filename,int volume);
		void playSoundWav(int volume);
		void loadSoundMus(const char* filename);
		void playSoundMus(int volume);
		
};


#endif
