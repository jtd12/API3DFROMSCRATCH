#ifndef DEF_DECOR_HPP
#define DEF_DECOR_HPP
#include"../force3D/entities/object/object.h"
#include"../force3D/loader/objloader.hpp"
#include"camera.hpp"

class decorsetup
{
	std::vector<object*> decor;
		
	public:
		decorsetup();
		~decorsetup();
		void update();
		void draw(std::vector<Triangle>& allTriangles,SDL_Renderer* renderer, int screenWidth, int screenHeight, camerasetup* camera);
};
#endif
