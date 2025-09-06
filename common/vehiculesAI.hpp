#ifndef DEF_VEHICLESAI_HPP
#define DEF_VEHICULESAI_HPP
#include"composant_vehicule.hpp"
#include"vehicules.hpp"
#include"../force3D/entities/object/object.h"
#include<vector>
#include <unordered_map>


class ModelManager {
public:
    static std::unordered_map<std::string, mesh*> models;

    static mesh* getModel(const std::string& filename);
};


class vehiculesAISetup
{
	
	std::vector<object*> wheel;
	std::vector<Triangle> triangles;
	vector3d rotation;
	vector3d scale;
	vector3d position;
	std::vector<vector3d> waypoints_;
	mesh* carMesh; 
	float yaw = 0.0f;
	float speed;
	vector3d direction;
	std::vector<object*> vehicule_;
	 
	public:
		vehiculesAISetup(mesh* sharedMesh, const std::vector<vector3d>& waypoints,  int startIndex);
		~vehiculesAISetup();
		void followWayPoints(float deltaTime);
		void init(mesh* sharedMesh, const std::vector<vector3d>& waypoints,  int startIndex);
		bool isInViewFrustum(camerasetup* camera,std::vector<object*>& vehicule) const;
		void update();
		void setGravity();
		void draw(std::vector<Triangle>& allTriangles,SDL_Renderer* renderer, int screenWidth, int screenHeight,camerasetup* camera);
		bool isInViewFrustum(vehiculesetup& car) const;
		int currentWaypointIndex;
		void setSpeed(float s);
		float getSpeed();
		vector3d getPosition();
		void setLocation(vector3d loc);
		void setLocation(float locY);
		vector3d getForwardVector() const;
		std::vector<object*>& getObjects();
		void applyMatrix();
		
};
#endif
