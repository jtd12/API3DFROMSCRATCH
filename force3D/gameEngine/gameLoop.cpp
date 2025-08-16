#include"gameLoop.hpp"

gameLoop::gameLoop()
{
	init();
}

gameLoop::~gameLoop()
{
	 destroyGameLoop();
}

bool gameLoop::init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", SDL_GetError());
        return EXIT_FAILURE;
    }

	
//	game=new setup();
	
    if (SDL_CreateWindowAndRenderer(800, 600, SDL_WINDOW_SHOWN, &getWindow(), &getRenderer()) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", SDL_GetError());        
        SDL_Quit();         
        return EXIT_FAILURE;
    }
}

void gameLoop::update()
{
//	game->update(pRenderer);
 //   game->update();
}

void gameLoop::loop()
{
	

	  while (SDL_PollEvent(&events)) {
        
            if (events.type == SDL_QUIT)
			{
			 isOpen = false;
          //game.inputs();
          
        }
        
        else if (events.type == SDL_MOUSEBUTTONDOWN) {
        if (events.button.button == SDL_BUTTON_LEFT) {
        	SDL_SetRelativeMouseMode(SDL_TRUE);
            mouseLeftPressed = true;
        }
    } else if (events.type == SDL_MOUSEBUTTONUP) {
        if (events.button.button == SDL_BUTTON_LEFT) {
            mouseLeftPressed = false;
            SDL_SetRelativeMouseMode(SDL_FALSE);
        }
    }
    }
        
	    SDL_GetRelativeMouseState(&mouseX, &mouseY);
	     // SDL_WarpMouseInWindow(getWindow(), 800 / 2, 600 / 2);

       
    
    keys = SDL_GetKeyboardState(NULL);
        
}


void gameLoop::FillColor()
{
	
        SDL_SetRenderDrawColor(getRenderer(), 0, 0, 0, 255);  // Noir
        SDL_RenderClear(getRenderer());
	//game->draw(pRenderer);
}

void gameLoop::draw()
{
	SDL_RenderPresent(getRenderer());
}

void gameLoop::destroyGameLoop()
{
	
    SDL_DestroyRenderer(getRenderer());
    SDL_DestroyWindow(getWindow());
    SDL_Quit();
    
    //delete game;
	//game = nullptr;
}

SDL_Renderer*& gameLoop::getRenderer()
{
	return pRenderer;
}

SDL_Window*& gameLoop::getWindow()
{
	return pWindow;
}

void gameLoop::setOpen(bool open)
{
	isOpen=open;
}

bool gameLoop::getOpen()
{
	return isOpen;
}

const Uint8* gameLoop:: getKeys()
{
	return keys;
}

int gameLoop::getMouseX()
{
	return mouseX;
}

int gameLoop::getMouseY()
{
			return mouseY;
}

bool gameLoop::getMousePressed()
{
	return mouseLeftPressed;
}


