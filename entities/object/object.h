#ifndef DEF_DECOR_H
#define DEF_DECOR_H
#define SDL_MAIN_HANDLED
#include <thread>
#include <mutex>  // Pour utiliser std::mutex
#include"../../maths/vector.h"
#include"../camera/camera.h"
#include"../../maths/matrix.hpp"
#include"../../loader/objloader.hpp"



class vec
{
public: 
	
	 float distance( vector3d& other,vector3d other2)  {
        float dx = other.x - other2.x;
        float dy = other.y - other2.y;
        float dz = other.z - other2.z;
        return std::sqrt(dx * dx + dy * dy + dz * dz);
    }
};

class object
{
	public:
		object(vector3d pos,vector3d rot,vector3d scaling,const std::string& path,bool wireframe);
		object(vector3d pos,vector3d rot,vector3d scaling,object* baseModel);
		~object();
		void update(std::vector<Triangle>& triangles);
		void applyMatrix();
		void update();
		bool cameraInside(const vector3d& camPos) const;
		void draw(SDL_Renderer* renderer, int screenWidth, int screenHeight, const Camera& camera, std::vector<Triangle>& allTriangles);
    	std::vector<Triangle> triangles;
    	void drawAABB(SDL_Renderer* renderer, const Camera& camera,std::vector<Triangle>& allTriangles, int screenWidth, int screenHeigh);
    	vector3d getNormal() const;
    	bool isCloseTo( vector3d other,float threshold,vector3d position) const ;
    	bool isInViewFrustum(const Camera& cam,vector3d position) const;
    	bool isPointInsideAABB(const vector3d& point);
    	Matrix4x4& getTranslationMatrix();
    	Matrix4x4& getRotationMatrixX();
    	Matrix4x4& getRotationMatrixY();
    	Matrix4x4& getRotationMatrixZ();
    	Matrix4x4& getScaleMatrix();
    	void setLocation(vector3d pos);
    	void setRotation(vector3d rot);
    	vector3d getLocation();
     	void computeAABB(); // déclaration de la méthode
     	vector3d getCenter() const;
		AABB aabb;
		AABB* getBoundingBox();
		vector3d getSize() const;
	
	private:
		
		objloader *obj;
		Matrix4x4 translationMatrix;
		Matrix4x4 scaleMatrix;
		Matrix4x4 rotationMatrixX;
		Matrix4x4 rotationMatrixY;
		Matrix4x4 rotationMatrixZ;
		vector3d position;
		vector3d rotation;
		vector3d scale;
		AABB box;
		vec vecteur;
	vector3d aabbMin;
	vector3d aabbMax;

	AABB* boundingBox;
	
};
#endif
