#ifndef DEF_CAPTUREVIDEOS_HPP
#define DEF_CAPTUREVIDEOS_HPP
#define SDL_MAIN_HANDLED
#include"../force3D/entities/captureVideo/captureVideo.hpp"

#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include<vector>

class captureVideoSetup
{
	captureVideo* capture_Video;
	public:
		captureVideoSetup();
		~captureVideoSetup();
		void update(Uint32* framebuffer,SDL_Renderer* renderer, int width, int height,int repeat);

		
};

#endif
