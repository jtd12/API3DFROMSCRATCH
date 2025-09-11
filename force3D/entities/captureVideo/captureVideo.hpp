#ifndef DEF_CAPTUREVIDEO_HPP
#define DEF_CAPTUREVIDEO_HPP
#define SDL_MAIN_HANDLED
#include"C:\library\SDL2-2.0.12\x86_64-w64-mingw32\include\SDL2\SDL.h"
#include"C:\library\SDL_Image 2.0\include\SDL2\SDL_image.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include<vector>
class captureVideo
{
	int frameIndex;
	int fps;
    Uint32 lastCaptureTime; // timestamp de la dernière frame capturée
	SDL_Texture* renderTarget = nullptr;
	std::string outputFolder;
	
	public:
		captureVideo(const std::string& folder);
		~captureVideo();
		void update(Uint32* framebuffer,SDL_Renderer* renderer, int width, int height,int repeatCount);

		
};

#endif
