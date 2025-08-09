#ifndef DEF_SKYBOX_H
#define DEF_SKYBOX_H
#include"../../maths/vector.h"
#include<iostream>
#define SDL_MAIN_HANDLED
#include"C:\library\SDL2-2.0.12\x86_64-w64-mingw32\include\SDL2\SDL.h"

class skybox
{
	public:
skybox();
~skybox();
void drawBackground(Uint32* framebuffer, int screenWidth, int screenHeight);
void drawSun(Uint32* framebuffer, int screenWidth, int screenHeight, int centerX, int centerY, int radius);
void update();
float getRot();

	private:

		float rot;

		
};

#endif
