#ifndef DEF_SETUP_HPP
#define DEF_SETUP_HPP
#include"common/camera.hpp"
#include"common/bullet.hpp"
#include"common/vehicules.hpp"
#include"common/player.hpp"
#include"common/heightmap.hpp"
#include"common/decor.hpp"
#include"common/collision.hpp"
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
	std::vector<object*> collid;
	std::vector<Triangle> allTriangles;
	std::vector<object*> vehicule;
	std::vector<Bullet*> bullets;

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

