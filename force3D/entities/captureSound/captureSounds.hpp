#ifndef DEF_CAPTURESOUNDS_HPP
#define DEF_CAPTURESOUNDS_HPP
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include<vector>
#include <fstream>
#include <cmath> // pour std::fabs
#define SDL_MAIN_HANDLED
#include"C:\library\SDL2-2.0.12\x86_64-w64-mingw32\include\SDL2\SDL.h"

#pragma pack(push, 1)
struct WAVHeader {
    char riff[4];          // "RIFF"
    uint32_t chunkSize;    // 36 + dataSize
    char wave[4];          // "WAVE"
    char fmt[4];           // "fmt "
    uint32_t subchunk1Size;// 16
    uint16_t audioFormat;  // 1 = PCM
    uint16_t numChannels;  
    uint32_t sampleRate;   
    uint32_t byteRate;     
    uint16_t blockAlign;   
    uint16_t bitsPerSample;
    char data[4];          // "data"
    uint32_t dataSize;     
};
#pragma pack(pop)


class captureSound
{
	SDL_AudioDeviceID dev = 0;
    std::ofstream file;
    Uint32 audioDataSize = 0;
    std::vector<float> audioBuffer; // <-- ajouter ça
    
	public:
		captureSound();
		~captureSound();
		bool start(const char* filename, int sampleRate, int channels);
		void update();
		void stop();
		const char* audioFormatToString(Uint16 format);
		void captureSegment(int numSamples);
		WAVHeader header;
		
		
};

#endif




