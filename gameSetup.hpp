#ifndef DEF_GAMESETUP_HPP
#define DEF_GAMESETUP_HPP
#include"common/objloader.hpp"
#include"common/skybox.h"
#include"common/car.h"
#include"common/carAI.hpp"
#include"common/decor.h"
#include"common/camera.h"
#include"common/grid.hpp"
#include"common/racetrack.hpp"
#include"common/boundingBox.hpp"
#include"common/collisionDetection.hpp"
#include"common/GUI.hpp"

class setup
{
	public:
		
	setup();
	~setup();
	void init();
	void update(float t);
	void update();
	void update(SDL_Renderer* renderer);
	void processInput( SDL_Event event);
	void draw(SDL_Renderer* pRenderer);
	void drawCar(std::vector<Triangle>& allTriangles,SDL_Renderer* renderer, int screenWidth, int screenHeight, const Camera& camera,vehicule& car);
	void drawCar(std::vector<Triangle>& allTriangles,SDL_Renderer* renderer, int screenWidth, int screenHeight, const Camera& camera,std::vector<vehiculeAI*>& car);
	void drawDecor(std::vector<Triangle>& allTriangles,SDL_Renderer* renderer, int screenWidth, int screenHeight, const Camera& camera, std::vector<decor*>& sceneDecor);
	void drawPixels(std::vector<Triangle>& allTriangles,Uint32* framebuffer,float* framebufferDepth, int screenWidth, int screenHeight, const Camera& camera);
	void drawScene(SDL_Renderer* renderer, int screenWidth, int screenHeight, const Camera& camera , vehicule& car, std::vector<vehiculeAI*>& carAI,  std::vector<decor*>& sceneDecor
               );
    void removeCloseDecor( Camera& cam, float threshold);
 	void restoreDecorFarFrom(Camera& cam, float threshold);
 	vector3d computeNormal(const vector3d& v1, const vector3d& v2, const vector3d& v3);
	bool isTriangleVisible(const vector3d& normal, const vector3d& cameraPosition, const vector3d& trianglePoint);
	void followCamera(vector3d carPosition, vector3d carForward, float yawInput, float pitchInput);
	void getOrientationVectors(vector3d& forward, vector3d& right, vector3d& up) const;
	void handleMouseMovement(float mouseDeltaX, float mouseDeltaY);
	std::vector<vector3d> generateWaypoints(const std::vector<vector3d>& controlPoints, float spacing);

	private:
	vehicule * car;
	std::vector<vehiculeAI*> carAI;
	raceTrack * race;
	std::vector<decor*> dec;
	std::vector<decor*>::iterator it;
    std::vector<decor*> removedDecor; // Liste des véhicules supprimés
	grid* grid_;
	skybox* sky;
	Camera *camera;
	bool mouseDown;
	int lastX = 0, lastY = 0; // Position précédente de la souris
	pixel* pixel_;
	std::vector<std::vector<vector3d>> trackMesh;
	std::vector<std::pair<vector3d, vector3d>> trackEdges;
	std::vector<vector3d> trackPath;
	std::vector<vector3d> controlPoints;
	std::vector<std::vector<vector3d>> trackTriangles;
	std::vector<std::vector<vector3d>> borderTriangles;
	std::vector<std::vector<vector3d>> terrainTriangles;
	collisions* collid;
	AABB* carAABB;
	std::vector<Button*> myButton;
	Button* pannel;
	Button * text;
	vector3d posCamera;
	vector3d targetCamera;
	vector3d upCamera;

};

#endif

