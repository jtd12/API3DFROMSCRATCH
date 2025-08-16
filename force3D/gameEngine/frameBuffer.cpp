#include"frameBuffer.hpp"

 frameBuffer::frameBuffer()
 {
 	
 }
 
  frameBuffer::~frameBuffer()
 {
 
 }
 
 void frameBuffer::createTexture(SDL_Renderer* renderer,SDL_Texture*& texture,int screenWidth,int screenHeight)
 {
    // **Création d'une texture pour dessiner le framebuffer**
   texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, 
                                             SDL_TEXTUREACCESS_STREAMING, screenWidth, screenHeight);
    
   
    
}

void frameBuffer::initializeZBuffer(float* framebufferDepth)
{
	
  // Initialiser le Z-buffer à une grande valeur
    std::fill(framebufferDepth, framebufferDepth + 800 * 600, std::numeric_limits<float>::infinity());
    
}

void frameBuffer::updateTexture(SDL_Renderer* renderer,SDL_Texture*& texture,Uint32* framebuffer,int screenWidth)
{
	SDL_SetHint(SDL_HINT_RENDER_VSYNC, "0");
    // **Mise à jour de la texture**
    SDL_UpdateTexture(texture, NULL, framebuffer, screenWidth * sizeof(Uint32));

    // **Affichage final**
    SDL_RenderCopy(renderer, texture, NULL, NULL);
   

    SDL_RenderPresent(renderer);
}

void frameBuffer::destroyTexture(SDL_Texture*& texture,Uint32* framebuffer,float* framebufferDepth)
{
	delete[] framebuffer;
    delete[] framebufferDepth;
    SDL_DestroyTexture(texture);
}



