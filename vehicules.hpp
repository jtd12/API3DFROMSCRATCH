#ifndef DEF_VEHICULES_HPP
#define DEF_VEHICULES_HPP
#include"force3D/entities/object/object.h"
#include"camera.hpp"

class vehiculesetup
{
	std::vector<object*> wheel;
	std::vector<vector3d> rotation;
	std::vector<vector3d> position;
	float gravity;
	
	public:
		vehiculesetup();
		~vehiculesetup();
		void init(std::vector<object*>& vehicule);
		void update(std::vector<object*>& vehicule);
		void draw(std::vector<Triangle>& allTriangles,SDL_Renderer* renderer, int screenWidth, int screenHeight,std::vector<object*>& vehicule,camerasetup* camera);
};

#endif
