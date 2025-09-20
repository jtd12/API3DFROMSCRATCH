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
	float baseG = 0.5f + static_cast<float>(rand()) / RAND_MAX * 1.0f;
    float baseR = static_cast<float>(rand()) / RAND_MAX * .5f;
    float baseB = static_cast<float>(rand()) / RAND_MAX * .7f;
            
	heightMap->drawHeight(1000,15000, pixel->getPixel(),framebuffer, framebufferDepth, screenWidth, screenHeight, *camera->getCamera(),baseR,baseG,baseB);
	
}
		
