#ifndef DEF_CAR_AI
#define DEF_CAR_AI
#include"composant_vehicule.hpp"
#include"objloader.hpp"
#include<vector>

class vehiculeAI
{
	objloader *obj;
	std::vector<Triangle> triangles;
	std::vector<roue *> wheel;
	Matrix4x4 translationMatrix;
	Matrix4x4 rotationMatrixX;
	Matrix4x4 rotationMatrixY;
	Matrix4x4 rotationMatrixZ;
	Matrix4x4 scaleMatrix;
	vector3d rot;
	vector3d scale;
	vector3d position;
    vector3d direction;
    float speed;
    int currentWaypointIndex;
    float yaw = 0.0f;

    std::vector<vector3d> waypoints_;
	
	public:

    vehiculeAI(const std::string&  filename,const std::vector<vector3d>& waypoints,vector3d pos);
 
	~vehiculeAI();
	
	void update();
	
	void applyMatrix();

	void draw(SDL_Renderer* renderer, int screenWidth, int screenHeight, const Camera& camera, std::vector<Triangle>& allTriangles);

    void followWayPoints(float deltaTime);
    
    vector3d getPosition();
	void setLocation(float loc);
	void setSpeed(float s);
	bool isInViewFrustum(const Camera& cam) const;
};

#endif
