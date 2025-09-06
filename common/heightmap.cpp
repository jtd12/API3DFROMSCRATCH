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
	heightMap->getTranslationMatrix().setTranslation(-100000,-11000,-100000);
}

void heightmapsetup::draw(std::vector<Triangle>& allTriangles,SDL_Renderer* renderer, int screenWidth, int screenHeight, camerasetup* camera,drawPixels* pixel,Uint32* framebuffer,float* framebufferDepth)
{
	heightMap->drawHeight(TERRAINHEIGHT,TERRAINSIZE, pixel->getPixel(),framebuffer, framebufferDepth, screenWidth, screenHeight, *camera->getCamera());
	
}

float heightmapsetup::getHeightAt(float x, float z, float size, float h)
{
	return heightMap->getHeightAt(x,z,size,h);
}
