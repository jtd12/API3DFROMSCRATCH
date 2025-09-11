#include"GUI.hpp"

Button::Button(SDL_Rect rect_)
{
	rect=rect_;
	fillColor=vector3d(0,0,0);
}

Button::Button(SDL_Rect rect_,const std::string& text_, TTF_Font* font_, SDL_Color color_)
{
	rect=rect_;
	color=vector3d(0,0,0);
	isClicked=false;
	font=font_;
	text=text_;
    textColor=color_;
}

Button::~Button()
{
	delete font;
	delete textTexture;
}

bool Button::getClick()
{
	return isClicked;
}

void Button::setClick(bool b)
{
	isClicked=b;
}

void Button::setColor(vector3d col)
{
color=col;
}

void Button::setFillColor(vector3d fillCol)
{
	fillColor=fillCol;
}

void Button::updateTexture(SDL_Renderer* renderer) {
        if (textTexture) SDL_DestroyTexture(textTexture);  // Supprime l'ancienne texture

        SDL_Color color = {255, 255, 255, 255}; // Blanc
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            SDL_Rect textRect = { 
                rect.x + (rect.w - textSurface->w) / 2, 
                rect.y + (rect.h - textSurface->h) / 2, 
                textSurface->w, 
                textSurface->h 
            };
        SDL_FreeSurface(textSurface);
    }

void Button::setText(const std::string& newText,SDL_Renderer* renderer) {
        if (text != newText) {  // Met à jour uniquement si le texte change
            text = newText;
            //updateTexture(renderer);
        }
    }
	 
void Button::renderText(SDL_Renderer* renderer)
{
	SDL_Color color = {255, 255, 255, 255}; // Blanc
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
     SDL_Rect textRect = { 
                rect.x + (rect.w - textSurface->w) / 2, 
                rect.y + (rect.h - textSurface->h) / 2, 
                textSurface->w, 
                textSurface->h 
            };
    if (textTexture)
	 SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
	SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

void Button::renderText(Uint32* framebuffer, int WIDTH, int HEIGHT)
{
 if (!font || text.empty()) return;

    SDL_Color sdlColor = { 
        (Uint8)textColor.r, 
        (Uint8)textColor.g, 
        (Uint8)textColor.b, 
        255 
    };

     SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), sdlColor);
    if (!textSurface) return;

    // Convertir en 32 bits pour avoir un accès direct aux Uint32
    SDL_Surface* text32 = SDL_ConvertSurfaceFormat(textSurface, SDL_PIXELFORMAT_ARGB8888, 0);
    SDL_FreeSurface(textSurface);
    if (!text32) return;

    int startX = rect.x + (rect.w - text32->w) / 2;
    int startY = rect.y + (rect.h - text32->h) / 2;

    for (int y = 0; y < text32->h; y++) {
        int dstY = startY + y;
        if (dstY < 0 || dstY >= HEIGHT) continue;

        Uint32* srcLine = (Uint32*)((Uint8*)text32->pixels + y * text32->pitch);
        Uint32* dstLine = &framebuffer[dstY * WIDTH + startX];

        int copyWidth = text32->w;
        if (startX + copyWidth > WIDTH) copyWidth = WIDTH - startX;

        for (int x = 0; x < copyWidth; x++) {
            Uint32 pixel = srcLine[x];
            if ((pixel & 0xFF000000) != 0) { // pixels non transparents
                dstLine[x] = pixel;
            }
        }
    }

    SDL_FreeSurface(text32);
}

void Button::render(SDL_Renderer* renderer) {
        SDL_SetRenderDrawColor(renderer, color.x, color.y, color.z, 255); // Rouge
        SDL_RenderFillRect(renderer, &rect);
        
        if (font) {
            SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            
            // Centrage du texte dans le bouton
            SDL_Rect textRect = { 
                rect.x + (rect.w - textSurface->w) / 2, 
                rect.y + (rect.h - textSurface->h) / 2, 
                textSurface->w, 
                textSurface->h 
            };
            
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
    }
}

void Button::render(Uint32* framebuffer, int WIDTH, int HEIGHT)
{
 Uint32 bgColor = (255 << 24) | ((Uint8)color.x << 16) | ((Uint8)color.y << 8) | (Uint8)color.z;
    for (int y = rect.y; y < rect.y + rect.h; y++) {
        if (y < 0 || y >= HEIGHT) continue;
        for (int x = rect.x; x < rect.x + rect.w; x++) {
            if (x < 0 || x >= WIDTH) continue;
            framebuffer[y * WIDTH + x] = bgColor;
        }
    }

    // --- Dessiner le texte ---
    if (font && !text.empty()) {
        SDL_Color sdlColor = { 
            (Uint8)textColor.r, 
            (Uint8)textColor.g, 
            (Uint8)textColor.b, 
            255 
        };

        SDL_Surface* tmp = TTF_RenderText_Solid(font, text.c_str(), sdlColor);
		SDL_Surface* textSurface = SDL_ConvertSurfaceFormat(tmp, SDL_PIXELFORMAT_ARGB8888, 0);
        if (textSurface) {
            Uint32* srcPixels = (Uint32*)textSurface->pixels;
            int pitch = textSurface->pitch / 4; // nombre de pixels par ligne

            // Centrer le texte dans le bouton
            int startX = rect.x + (rect.w - textSurface->w) / 2;
            int startY = rect.y + (rect.h - textSurface->h) / 2;

            for (int y = 0; y < textSurface->h; y++) {
                int dstY = startY + y;
                if (dstY < 0 || dstY >= HEIGHT) continue;

                for (int x = 0; x < textSurface->w; x++) {
                    int dstX = startX + x;
                    if (dstX < 0 || dstX >= WIDTH) continue;

                    Uint32 pixel = srcPixels[y * pitch + x]; // <-- utiliser pitch
                    if ((pixel & 0xFF000000) != 0) { // alpha != 0
                        framebuffer[dstY * WIDTH + dstX] = pixel;
                    }
                }
            }

            SDL_FreeSurface(tmp);
        }
    }
}


void Button::renderPanel(SDL_Renderer* renderer)
{
	 SDL_SetRenderDrawColor(renderer, fillColor.x, fillColor.y, fillColor.z, 255); // Rouge
     SDL_RenderFillRect(renderer, &rect);
}

void Button::renderPanel(Uint32* framebuffer, int WIDTH, int HEIGHT) {
/*rect.x = 50;
rect.y = 50;
rect.w = 200;
rect.h = 100;*/
 Uint32 color = (255 << 24) | ((Uint8)fillColor.x << 16) | ((Uint8)fillColor.y << 8) | (Uint8)fillColor.z; for (int y = rect.y; y < rect.y + rect.h; y++) { if (y < 0 || y >= HEIGHT) continue; for (int x = rect.x; x < rect.x + rect.w; x++) { if (x < 0 || x >= WIDTH) continue; framebuffer[y * WIDTH + x] = color; } } }

void Button::handleEvent(SDL_Event& event) {
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            int mx = event.button.x;
            int my = event.button.y;

            if (mx >= rect.x+100 && mx <= rect.x+100 + rect.w &&
                my >= rect.y+100 && my <= rect.y+100 + rect.h) {
                isClicked = true;
            
            }
    	}
    }
