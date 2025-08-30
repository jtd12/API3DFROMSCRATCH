#ifndef DEF_COMPOSANT_VEHICULE_HPP
#define DEF_COMPOSANT_VEHICULE_HPP
#include"../../loader/objloader.hpp"
#include<vector>


class roue
{
public:
		roue(vector3d pos);
		~roue();
		void update();
		void setAngle(float a);
		vector3d getRotation();
		void setRotationz(float m);
		void setRotationzegal(float m);
		void setRotationy(float m);
		void setRotationyegal(float m);
		void draw(SDL_Renderer* renderer, int screenWidth, int screenHeight, const Camera& camera, std::vector<Triangle>& allTriangles);
		void applyMatrix();
		void setLocation(vector3d pos);

		
	private:
		std::vector<Triangle> triangles;
		objloader *obj;
		vector3d scale;
		Matrix4x4 translationMatrix;
		Matrix4x4 rotationMatrixX;
		vector3d rot_roue;
		Matrix4x4 rotationMatrixZ;
		Matrix4x4 scaleMatrix;
		vector3d position;
		Matrix4x4 rotationMatrixY;
};


class porte_vitre
{
public:
		porte_vitre(const char * filename);
		~porte_vitre();
		void update();
		void draw();
		
	private:
		objloader *obj;
		int porte_vitre_;	
};

class tb
{
public:
		tb(const char * filename);
		~tb();
		void update();
		void draw(SDL_Renderer* renderer, int screenWidth, int screenHeight, const Camera& camera, std::vector<Triangle>& allTriangles);
		void applyMatrix(vector3d position,vector3d rotation);
		
	private:
		objloader *obj;	
		std::vector<Triangle> triangles;
		vector3d scale;
		Matrix4x4 translationMatrix;
		Matrix4x4 rotationMatrixX;
		vector3d rot_roue;
		Matrix4x4 rotationMatrixZ;
		Matrix4x4 scaleMatrix;
		Matrix4x4 rotationMatrixY;
};

class porte
{
	public:
		porte(const char * filename);
		~porte();
		void update();
		void draw();
		void setRotPorte(float r);
		void setRotPorteEgal(float r);
		float getRotPorte();
		
	private:
		objloader *obj;
		porte_vitre *p1_vitre;
		porte_vitre *p2_vitre;
		int porte_;
		float rotPorte;


		
};

class vitres
{
	public:
		vitres();
		~vitres();
		void update();
		void draw();
		
	private:
		int vitres_;
		objloader *obj;
		
};

class volant
{
	public:
		volant();
		~volant();
		void update();
		void draw();
		
	private:
		int volant_;
		objloader *obj;
		
};
#endif
