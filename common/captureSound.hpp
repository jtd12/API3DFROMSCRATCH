#ifndef DEF_CAPTURESOUND_HPP
#define DEF_CAPTURESOUND_HPP
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include<vector>
#include <fstream>
#include <cmath> // pour std::fabs
#include"../force3D/entities/captureSound/captureSounds.hpp"



class captureSoundSetup
{
	captureSound* capture_Sound;
	public:
		captureSoundSetup();
		~captureSoundSetup();
		bool start(const char* filename, int sampleRate, int channels);
		void update();
		void stop();
		const char* audioFormatToString(Uint16 format);
		void captureSegment(int numSamples);
		captureSound* getCaptureSound();

		
		
};

#endif



