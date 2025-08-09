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
            updateTexture(renderer);
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

void Button::renderPanel(SDL_Renderer* renderer)
{
	 SDL_SetRenderDrawColor(renderer, fillColor.x, fillColor.y, fillColor.z, 255); // Rouge
     SDL_RenderFillRect(renderer, &rect);
}

void Button::handleEvent(SDL_Event& event) {
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            int mx = event.button.x;
            int my = event.button.y;

            if (mx >= rect.x && mx <= rect.x + rect.w &&
                my >= rect.y && my <= rect.y + rect.h) {
                isClicked = true;
            
            }
    	}
    }
