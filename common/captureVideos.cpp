#include"captureVideos.hpp"

captureVideoSetup::captureVideoSetup()
{
capture_Video=new captureVideo("capture");
}

captureVideoSetup::~captureVideoSetup()
{
	delete capture_Video;
}


void captureVideoSetup::update(Uint32* framebuffer,SDL_Renderer* renderer, int width, int height,int repeat) {
capture_Video->update( framebuffer, renderer, width, height,repeat);
}
