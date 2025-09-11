#include"captureVideo.hpp"

captureVideo::captureVideo(const std::string& folder)
{
frameIndex=0;
outputFolder=folder;

}

captureVideo::~captureVideo()
{
delete renderTarget;	
}



void captureVideo::update(Uint32* framebuffer,SDL_Renderer* renderer, int width, int height,int repeatCount) {
	if (!framebuffer) return;
	

	
	 std::vector<Uint32> argbBuffer(framebuffer, framebuffer + width * height);
	  int numPixels = width * height;
	    for (int i = 0; i < numPixels; i++) {
	        Uint32 pixel = argbBuffer[i];
	        Uint8 r = (pixel >> 24) & 0xFF;
	        Uint8 g = (pixel >> 16) & 0xFF;
	        Uint8 b = (pixel >> 8) & 0xFF;
	        Uint8 a = pixel & 0xFF;
	        argbBuffer[i] = (a << 24) | (r << 16) | (g << 8) | b;
	    }

	
	 SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormatFrom(
	        argbBuffer.data(), width, height, 32, width * sizeof(Uint32), SDL_PIXELFORMAT_ARGB8888
	    );

    if (!surface) {
        std::cerr << "Erreur création surface : " << SDL_GetError() << std::endl;
        return;
    }

    // Sauvegarder en PNG
for (int i = 0; i < repeatCount; i++) {
    std::ostringstream filename;
   filename << outputFolder << "/frame_" << std::setw(5) << std::setfill('0') << frameIndex++ << ".png";

    if (IMG_SavePNG(surface, filename.str().c_str()) != 0) {
        std::cerr << "Erreur IMG_SavePNG : " << IMG_GetError() << std::endl;
    }
}

    SDL_FreeSurface(surface);
}
