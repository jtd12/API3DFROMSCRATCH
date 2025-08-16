#include"heightmap.hpp"

heightmapsetup::heightmapsetup()
{
	heightMap=new height();
	heightMap->loadHeightMap("data/Heightmap2.bmp");
}

heightmapsetup::~heightmapsetup()
{
	delete heightMap;
}
		
		
void heightmapsetup::update()
{
	heightMap->getTranslationMatrix().setTranslation(-100000,-21800,-100000);
}

void heightmapsetup::draw(std::vector<Triangle>& allTriangles,SDL_Renderer* renderer, int screenWidth, int screenHeight, camerasetup* camera,drawPixels* pixel,Uint32* framebuffer,float* framebufferDepth)
{
	heightMap->drawHeight(1000,15000, pixel->getPixel(),framebuffer, framebufferDepth, screenWidth, screenHeight, *camera->getCamera());
	
}
		
