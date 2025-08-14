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

struct AnimationRange {
    int startFrame;
    int endFrame;
};


class setup
{
	
	Camera camera;
	gameLoop *game;
	drawPixels* pixel;
	skybox* sky;
	frameBuffer* buffer;
	vector3d positionWeapon;
	std::vector<object*> wheel;
	std::vector<object*> decor;
	object* myAnimatedObject;
	height* heightMap;
	float gravity;
	float gravityCamera;
	std::vector<object*> collid;
	Matrix4x4 rotationMatrixY;
	std::vector<vector3d> rotation;
	std::vector<vector3d> position;
	bool up,down,right,left;
	std::vector<Triangle> allTriangles;
	vector3d oldPos;
    vector3d testPos;
    bool moved;	
	float speed; // unit�s / seconde
	std::vector<AnimationRange> animations = {
        {0, 50},
        {0, 60},
        {25, 70},
        {50, 90},
        {50, 90},
        {50, 90},
        {50, 90},
        /*{100, 150},
        {150, 200},
        {150, 200},
        {150, 200},
        {150, 200},
        {150, 200},
        {200,220},
        {220,260},
        {220,260},
        {220,260},
        {220,260},
        {220,260},
        {220,260},
        {220,260}*/
    };
    
    float changeTimer = 0.0f;
	float changeInterval = 90.0f; // toutes les 5 secondes
	int currentStartFrame = 0;
	int currentEndFrame = 260;

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
