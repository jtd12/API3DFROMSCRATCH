#ifndef DEF_CAR_H
#define DEF_CAR_H
#include"objloader.hpp"
#include"composant_vehicule.hpp"
#include<vector>

class vehicule
{
	public:
		vehicule(const std::string&  filename,vector3d pos);
		~vehicule();
		void ouvrirPorte1();
		void ouvrirPorte2();
		void update();
		void applyMatrix();
		void draw(SDL_Renderer* renderer, int screenWidth, int screenHeight, const Camera& camera, std::vector<Triangle>& allTriangles);
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
		
	private:
	objloader *obj;
	std::vector<porte *> door;
	std::vector<roue *> wheel;
	tb* tableau_bord;
	vitres *v;
	tb *tab;
	volant * vv;
	Matrix4x4 translationMatrix;
	Matrix4x4 rotationMatrixX;
	Matrix4x4 rotationMatrixY;
	Matrix4x4 rotationMatrixZ;
	Matrix4x4 scaleMatrix;
	vector3d rot;
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


};

#endif
