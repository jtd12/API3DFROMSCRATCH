#ifndef DEF_GAMELOOP_HPP
#define DEF_GAMELOOP_HPP
#include"C:\library\SDL2-2.0.12\x86_64-w64-mingw32\include\SDL2\SDL.h"
//#include"gameSetup.hpp"


class gameLoop
{
	public:
		gameLoop();
		~gameLoop();
		bool init();
		void update();
		void loop();
		void draw();
		void FillColor();
		void destroyGameLoop();
		SDL_Renderer*& getRenderer();
		SDL_Window*& getWindow();
		void setOpen(bool open);
		bool getOpen();
		const Uint8* getKeys();
		int getMouseX();
		int getMouseY();
		bool getMousePressed();
		
	private:
		    SDL_Event events;
		    bool isOpen=true;
		    SDL_Window* pWindow{ nullptr };     
    		SDL_Renderer* pRenderer{ nullptr };
    		const Uint8* keys;
    		int mouseX;
    		int mouseY;
    		bool mouseLeftPressed;
    		

    		//setup* game;
};
#endif
