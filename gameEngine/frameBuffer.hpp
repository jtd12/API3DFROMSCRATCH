#ifndef DEF_FRAME_BUFFER_HPP
#define DEF_FRAME_BUFFER_HPP
#include"C:\library\SDL2-2.0.12\x86_64-w64-mingw32\include\SDL2\SDL.h"
#include <algorithm>

class frameBuffer
{

	
	public:
		frameBuffer();
		~frameBuffer();
		void createTexture(SDL_Renderer* renderer,SDL_Texture*& texture, int screenWidth,int screenHeight);
		void initializeZBuffer(float* framebufferDepth);
		void updateTexture(SDL_Renderer* renderer,SDL_Texture*& texture,Uint32* framebuffer,int screenWidth);
		void destroyTexture(SDL_Texture*& texture,Uint32* framebuffer,float* framebufferDepth);
		
};
#endif
