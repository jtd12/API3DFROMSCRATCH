#ifndef DEF_DECOR_H
#define DEF_DECOR_H
#include <thread>
#include <mutex>  // Pour utiliser std::mutex
#include"vector.h"
#include"objloader.hpp"
#include"boundingBox.hpp"

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

class decor
{
	public:
		decor(vector3d pos,vector3d rot,const std::string& path);
		~decor();
		void update();
		void applyMatrix();
		void draw(SDL_Renderer* renderer, int screenWidth, int screenHeight, const Camera& camera, std::vector<Triangle>& allTriangles);
    	std::vector<Triangle> triangles;
    	AABB* getBoundingBox();
    	vector3d getNormal() const;
    	bool isCloseTo( vector3d other,float threshold) const ;
    	vector3d position;
    	bool isInViewFrustum(const Camera& cam) const;

	private:
		objloader *obj;
	
		Matrix4x4 translationMatrix;
		Matrix4x4 scaleMatrix;
		Matrix4x4 rotationMatrixX;
		Matrix4x4 rotationMatrixY;
		Matrix4x4 rotationMatrixZ;
		vector3d scale;
		vector3d rotation;
		AABB* boundingBox;
		vec vecteur;
	

	
};
#endif
