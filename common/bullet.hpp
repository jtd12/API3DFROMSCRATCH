#ifndef DEF_BULLET_HPP
#define DEF_BULLET_HPP
#include"../force3D/entities/object/object.h"
#include"camera.hpp"

class Bullet
{
	vector3d pos;
    vector3d dir;
    float speed;
    int ttl;
    object* mesh; // le cube.obj ou une autre forme
    
	public:
		Bullet(vector3d startPos, vector3d forward, float speed, int ttl, const std::string& filename);
		~Bullet();
		bool update();
		void draw(std::vector<Triangle>& allTriangles,SDL_Renderer* renderer, int screenWidth, int screenHeight,camerasetup* camera);
		void instanceBullet(vector3d startPos, vector3d forward, float speed, int ttl, const std::string& filename);
		
};
#endif
