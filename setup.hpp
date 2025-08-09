#ifndef DEF_SETUP_HPP
#define DEF_SETUP_HPP
#include"entities/camera/camera.h"
#include"maths/vector.h"
#include"loader/objloader.hpp"
#include"pixel/drawPixels.hpp"
#include"gameEngine/gameLoop.hpp"
#include"entities/object/object.h"
#include"gameEngine/frameBuffer.hpp"
#include"entities/sky/skybox.h"
#include"collision/boundingBox.hpp"
#include"entities/heightmap/heightmap.h"
#include<iostream>
#define WIDTH 800 
#define HEIGHT 600


inline std::ostream& operator<<(std::ostream& os, const Point2D& p) {
    os << "(" << p.x << ", " << p.y << ")";
    return os;
}

class setup
{
	
	Camera camera;
	gameLoop *game;
	drawPixels* pixel;
	skybox* sky;
	frameBuffer* buffer;
	std::vector<object*> wheel;
	std::vector<object*> decor;
	height* heightMap;
	float gravity;
	std::vector<object*> collid;
	Matrix4x4 rotationMatrixY;
	std::vector<vector3d> rotation;
	std::vector<vector3d> position;
	bool up,down,right,left;
	std::vector<Triangle> allTriangles;
	vector3d oldPos;
    vector3d testPos;
    bool moved;	
	float speed; // unités / seconde
	

	public:
		setup();
		~setup();
		void init(std::vector<object*>& vehicule);
		void update(std::vector<object*>& vehicule,setup* g);
		void draw(std::vector<Triangle>& allTriangles,SDL_Renderer* renderer, int screenWidth, int screenHeight, const Camera& camera, std::vector<object*>& vehicule);
		void draw(std::vector<object*>& vehicule);
		void gestionEvents(setup* g);
		void printObjectPosition(const std::vector<Triangle>& triangles);
		void collision(vector3d p1,vector3d p2);
	
		gameLoop * getGame();
};
#endif
