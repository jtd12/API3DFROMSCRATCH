#include <iostream>
#include"gameSetup.hpp"
/* run this program using the console pauser or add your own getch, system("pause") or input loop */


// Fonction pour remplir un triangle en utilisant l'algorithme de scanline


int main(int argc, char** argv) {
	

	
	SDL_Event event;
	setup* game=new setup();
	
	game->init();

	while (game->getGame()->getOpen()) {
	    
		
		game->getGame()->loop();
		
	for (auto &e : game->getGame()->getEvents()) {
        game->processInput(e);
    }
		game->update(game->getGame()->getRenderer());
       	game->update(game);
		game->draw();
       game->getGame()->draw();
    }
	game->getGame()->destroyGameLoop();

    return EXIT_SUCCESS;
}

