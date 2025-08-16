#ifndef DEF_HEIGHTMAP_HPP
#define DEF_HEIGHTMAP_HPP
#include"force3D/entities/heightmap/heightmap.h"
#include"force3D/pixel/drawPixels.hpp"
#include"camera.hpp"

class heightmapsetup
{
	height* heightMap;
	public:
		heightmapsetup();
		~heightmapsetup();
		void update();
		void draw(std::vector<Triangle>& allTriangles,SDL_Renderer* renderer, int screenWidth, int screenHeight, camerasetup* camera,drawPixels* pixel,Uint32* framebuffer,float* framebufferDepth);
};

#endif
