#include"captureSound.hpp"

captureSoundSetup::captureSoundSetup()
{
    capture_Sound=new captureSound();
}

captureSoundSetup::~captureSoundSetup() {
    delete capture_Sound;
}

bool captureSoundSetup::start(const char* filename, int sampleRate, int channels) {
    return capture_Sound->start(filename,sampleRate,channels);
}

void captureSoundSetup::update() {
  capture_Sound->update();
}

void captureSoundSetup::stop() {
   capture_Sound->stop(); 
}

void captureSoundSetup::captureSegment(int numSamples) {
    capture_Sound->captureSegment(numSamples);
}

captureSound* captureSoundSetup::getCaptureSound()
{
	return capture_Sound;
}
