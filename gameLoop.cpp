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

	
	game=new setup();
	
    if (SDL_CreateWindowAndRenderer(800, 600, SDL_WINDOW_SHOWN, &pWindow, &pRenderer) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", SDL_GetError());        
        SDL_Quit();         
        return EXIT_FAILURE;
    }
}

void gameLoop::update()
{
	game->update(pRenderer);
    game->update();
}
void gameLoop::inputs()
{
	   game->processInput(events);
}

void gameLoop::loop()
{
	
	bool isOpen{ true };
	
	 while (isOpen) {
        while (SDL_PollEvent(&events)) {
        
            if (events.type == SDL_QUIT) isOpen = false;
          inputs();
          
        }
        
      	update();
		
        SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);  // Noir
        SDL_RenderClear(pRenderer);
		draw();
        SDL_RenderPresent(pRenderer);
        
    }
   
}

void gameLoop::draw()
{
	game->draw(pRenderer);
}

void gameLoop::destroyGameLoop()
{
	
    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    SDL_Quit();
    
    delete game;
	game = nullptr;
}

