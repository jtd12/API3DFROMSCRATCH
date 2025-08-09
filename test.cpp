#include"setup.hpp"

int main(int argc,char **argv)
{

	std::vector<object*> vehicule;
	SDL_Event event;
	setup* game=new setup();
	
	game->init(vehicule);

	while (game->getGame()->getOpen()) {
	    
		
		game->getGame()->loop();
		
		game->gestionEvents(game);
		
       	game->update(vehicule,game);
		game->draw(vehicule);
        game->getGame()->draw();
    }
	game->getGame()->destroyGameLoop();
	
	return 0;
}
