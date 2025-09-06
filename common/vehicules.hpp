#ifndef DEF_VEHICULES_HPP
#define DEF_VEHICULES_HPP
#include"../force3D/entities/object/object.h"
#include"camera.hpp"


class vehiculesetup
{
	std::vector<object*> wheel;
	float gravity;
	vector3d rotation;
	vector3d scale;
	vector3d position;
	float acc, dec,dec2;
	int dir;
	vector3d a;
	vector3d loc;
 	float angle;
	float turnSpeed;
	float speed;
	bool up,down,right,left,control,control2,keyC,keyV;
	float s;
	float maxSpeed;	
	std::vector<Triangle> triangles;
	vector3d lastPosition;
    float stuckTimer = 0.0f;  // temps passé immobile
    bool isStuck = false;
    int currentSegment = 1; // segment Catmull-Rom courant
    float trackT = 0.0f;    // progression [0..1] dans ce segment
	
	public:
		vehiculesetup();
		~vehiculesetup();
		void init(std::vector<object*>& vehicule,vector3d pos,vector3d rot,vector3d s);
		bool isInViewFrustum(camerasetup* camera,std::vector<object*>& vehicule) const;
		void update(std::vector<object*>& vehicule);
		void draw(std::vector<Triangle>& allTriangles,SDL_Renderer* renderer, int screenWidth, int screenHeight,std::vector<object*>& vehicule,camerasetup* camera);
		void setAngle(float a);
		void move();
		void controlUp();
		void controlRight();
		void controlLeft();
		void controlFrein();
		void passiveControlUp_Down();
		void passiveControlRight_Left();
		float getSpeed();
		vector3d getPosition();
		void setLocation(float locY);
		void setLocation(vector3d loc);
		void setSpeed(float s);
		void setControlActif(bool b);
		bool getControlActif();
		void setControlActif2(bool b);
		bool getControlActif2();
	//	vector3d getForwardVector2() const;
		void setGravity();
		bool getKeyC();
		bool getKeyV();
		void setKeyC(bool b);
		void setKeyV(bool b);
		bool getUp();
		bool getDown();
		bool getRight();
		bool getLeft();
		void setUp(bool b);
		void setDown(bool b);
		void setRight(bool b);
		void setLeft(bool b);
		void setDir(int d);
		vector3d getRotation();
		vector3d getForwardVector() const;
		std::vector<Triangle> getTriangles();
		void unstuck();
	
		
};

#endif
