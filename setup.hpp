#ifndef DEF_SETUP_HPP
#define DEF_SETUP_HPP
#include"camera.hpp"
#include"vehicules.hpp"
#include"player.hpp"
#include"heightmap.hpp"
#include"decor.hpp"
#include"collision.hpp"
#include"force3D/pixel/drawPixels.hpp"
#include"force3D/gameEngine/gameLoop.hpp"
#include"force3D/gameEngine/frameBuffer.hpp"
#include"force3D/entities/sky/skybox.h"
#include<iostream>
#define WIDTH 800 
#define HEIGHT 600



class setup
{
	vehiculesetup* vehicules;
	camerasetup* camera;
	playersetup* player;
	heightmapsetup* height;
	decorsetup* decor;
	collisionsetup* collision;
	gameLoop *game;
	drawPixels* pixel;
	skybox* sky;
	frameBuffer* buffer;
	float gravity;
	std::vector<object*> collid;
	bool up,down,right,left;
	std::vector<Triangle> allTriangles;
	vector3d oldPos;
    vector3d testPos;
	std::vector<object*> vehicule;

	public:
		setup();
		~setup();
		void init();
		void update(setup* g);
		void draw(std::vector<Triangle>& allTriangles,SDL_Renderer* renderer, int screenWidth, int screenHeight);
		void draw();
		void gestionEvents(setup* g);
		gameLoop * getGame();
};
#endif

