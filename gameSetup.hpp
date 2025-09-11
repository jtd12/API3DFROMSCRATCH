#ifndef DEF_GAMESETUP_HPP
#define DEF_GAMESETUP_HPP
#include"common/camera.hpp"
#include"common/vehicules.hpp"
#include"common/vehiculesAI.hpp"
#include"common/decor.hpp"
#include"common/heightmap.hpp"
#include"common/collisionDetection.hpp"
#include"force3D/pixel/drawPixels.hpp"
#include"force3D/gameEngine/gameLoop.hpp"
#include"force3D/gameEngine/frameBuffer.hpp"
#include"force3D/entities/sky/skybox.h"
#include"common/racetrack.hpp"
#include"common/GUI.hpp"
#include"common/captureVideos.hpp"
#include"common/sound.hpp"
#include"common/captureSound.hpp"
#include<iostream>
#define WIDTH 800 
#define HEIGHT 600

class setup
{
	public:
		
	setup();
	~setup();
	void init();
	void update(setup* g);
	void update(SDL_Renderer* renderer);
	void drawCarPlayer(std::vector<Triangle>& allTriangles,SDL_Renderer* renderer, int screenWidth, int screenHeight);
	void drawCarPlayerAI(std::vector<Triangle>& allTriangles,SDL_Renderer* renderer, int screenWidth, int screenHeight,vehiculesetup& carSimple);
	void draw(std::vector<Triangle>& allTriangles,SDL_Renderer* renderer, int screenWidth, int screenHeight,vehiculesetup& carSimple);
	void draw();
	void gestionEvents(setup* g);
	gameLoop * getGame();
	void processInput(SDL_Event event);
	vector3d computeNormal(const vector3d& v1, const vector3d& v2, const vector3d& v3);
	bool isTriangleVisible(const vector3d& normal, const vector3d& cameraPosition, const vector3d& trianglePoint);
	void handleMouseMovement(float mouseDeltaX, float mouseDeltaY);
	void followCamera(vector3d carPosition, vector3d carForward, float yawInput, float pitchInput);
	bool checkCollision(vehiculesAISetup* a, vehiculesAISetup* b);
	bool checkCollision(vehiculesAISetup* a, vehiculesetup* b);
	
	private:
	vehiculesetup* car;
	std::vector<object*> vehicule;
	std::vector<vehiculesAISetup*> allCarsAI;
	std::vector<object*> vehiculeAI;
	camerasetup* camera;
	heightmapsetup* height;
	gameLoop *game;
	drawPixels* pixel;
	skybox* sky;
	frameBuffer* buffer;
	std::vector<Triangle> allTriangles;
	std::vector<std::vector<vector3d>> trackMesh;
	std::vector<std::pair<vector3d, vector3d>> trackEdges;
	std::vector<vector3d> trackPath;
	std::vector<vector3d> controlPoints;
	std::vector<vector3d> waypoints;
	std::vector<std::vector<vector3d>> trackTriangles;
	std::vector<std::vector<vector3d>> borderTriangles;
	std::vector<std::vector<vector3d>> terrainTriangles;
	raceTrackSetup * race;
	bool mouseDown;
	int lastX = 0, lastY = 0; // Position précédente de la souris
	collisions* collid;
	std::vector<Button*> myButton;
	Button* pannel;
	std::vector<Button*>  text;
	AABB* carAABB;
	std::vector<decorSetup*> cubes;
	std::vector<decorSetup*> arbres;
	std::vector<decorSetup*> triangles;
	std::vector<decorSetup*> animals;
	std::vector<decorSetup*> cylindres;
	Uint32 lastSpeedChangeTime = SDL_GetTicks(); // récupère le temps en ms depuis le lancement
	const Uint32 speedChangeInterval = 5000;    // 10 secondes = 10000 ms
	int temps;
	captureVideoSetup* capture_video;
	soundSetup* sounds;
	soundSetup* sounds2;
	captureSoundSetup* capture_sound;
	bool recording;

};

#endif

