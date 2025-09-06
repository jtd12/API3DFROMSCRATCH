#ifndef DEF_DECOR_HPP
#define DEF_DECOR_HPP
#include"../force3D/entities/object/object.h"
#include"../force3D/loader/objloader.hpp"
#include"camera.hpp"
#include"vehicules.hpp"
#include"boundingBox.hpp"
class decorSetup
{
	public:
		decorSetup(vector3d pos,vector3d rot,vector3d s,const std::string& path);
		~decorSetup();
		void init(vector3d pos,vector3d rot,vector3d s,const std::string& path);
		void update();
		void applyMatrix();
		void draw(std::vector<Triangle>& allTriangles,SDL_Renderer* renderer, int screenWidth, int screenHeight,camerasetup* camera);
    	std::vector<Triangle> triangles;
    	AABB* getBoundingBox();
    	vector3d getNormal() const;
    	bool isCloseTo( vector3d other,float threshold) const ;
    	bool isInViewFrustum(vehiculesetup& car) const;
		void setScale(vector3d s);
		
	private:
	
		Matrix4x4 translationMatrix;
		Matrix4x4 scaleMatrix;
		Matrix4x4 rotationMatrixX;
		Matrix4x4 rotationMatrixY;
		Matrix4x4 rotationMatrixZ;

		vector3d rotation;
		vector3d scale;
		vector3d position;
		AABB* boundingBox;
		vec vecteur;
		std::vector<object*> decor_;   // ? ton conteneur d’objets décor
	

	
};
#endif
