#include"setup.hpp"

int main(int argc,char **argv)
{
	SDL_Event event;
	setup* game=new setup();
	
	game->init();

	while (game->getGame()->getOpen()) {
	    
		
		game->getGame()->loop();
		
		game->gestionEvents(game);
		
       	game->update(game);
		game->draw();
        game->getGame()->draw();
    }
	game->getGame()->destroyGameLoop();
	
	return 0;
}
