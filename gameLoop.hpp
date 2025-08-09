#ifndef DEF_GAMELOOP_HPP
#define DEF_GAMELOOP_HPP
#include"gameSetup.hpp"


class gameLoop
{
	public:
		gameLoop();
		~gameLoop();
		bool init();
		void update();
		void loop();
		void draw();
		void inputs();
		void destroyGameLoop();
		
	private:
		    SDL_Event events;
		    SDL_Window* pWindow{ nullptr };     
    		SDL_Renderer* pRenderer{ nullptr };
    		setup* game;
};
#endif
