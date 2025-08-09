#include"skybox.h"


skybox::skybox()
{
	 
		  
  }
 
skybox::~skybox()
{
     
}




void skybox::update()
{

}

float skybox::getRot()
{
return rot;	
}

void skybox::drawSun(Uint32* framebuffer, int screenWidth, int screenHeight, int centerX, int centerY, int radius) {
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w;  // Distance en X
            int dy = radius - h;  // Distance en Y

            if ((dx * dx + dy * dy) <= (radius * radius)) {  // Vérifier si dans le cercle
                Uint8 r = 255;  // Soleil jaune
                Uint8 g = 234;
                Uint8 b = 0;

                Uint32 color = (255 << 24) | (r << 16) | (g << 8) | b;

                int pixelX = centerX + dx;
                int pixelY = centerY + dy;

                // Vérifier si le pixel est dans les limites de l'écran
                if (pixelX >= 0 && pixelX < screenWidth && pixelY >= 0 && pixelY < screenHeight) {
                    framebuffer[pixelY * screenWidth + pixelX] = color;
                }
            }
        }
    }
}

void skybox::drawBackground(Uint32* framebuffer, int screenWidth, int screenHeight) {
    for (int y = 0; y < screenHeight; y++) {
        // Calcul de l'interpolation (de 0 en haut à 1 en bas)
        float t = y / float(screenHeight);

        // Définir les couleurs du ciel et de l'horizon
        Uint8 r = (1.0f - t) * 135 + t * 0;  // Bleu vers noir
        Uint8 g = (1.0f - t) * 206 + t * 0;
        Uint8 b = (1.0f - t) * 235 + t * 0;

        // Définir la couleur et dessiner la ligne
       // SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        //SDL_RenderDrawLine(renderer, 0, y, screenWidth, y);
  		Uint32 color =  (r << 24) | (g << 16) | (b << 8) | 255;
        
        for (int x = 0; x < screenWidth; x++) {
            framebuffer[y * screenWidth + x] = color;
        }
    }
    drawSun(framebuffer, screenWidth, screenHeight, screenWidth / 4, screenHeight / 10, 10);
}	    
 
