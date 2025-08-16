#ifndef DEF_COLLISION_HPP
#define DEF_COLLISION_HPP
#include"../force3D/collision/boundingBox.hpp"
#include"../force3D/entities/object/object.h"
#include"../force3D/loader/objloader.hpp"
#include"camera.hpp"
#include"player.hpp"

class collisionsetup
{
	std::vector<object*> collid;
	vector3d oldPos;
    vector3d testPos;
    bool moveForward;
    bool moveBack;
    bool moveRight;
    bool moveLeft;
	bool tir;
	bool reload;
	
	public:
	  collisionsetup();
	  ~collisionsetup();
	  void update(playersetup* player,camerasetup* camera,float deltaTime);
	  void draw(std::vector<Triangle>& allTriangles,SDL_Renderer* renderer, int screenWidth, int screenHeight, camerasetup* camera);
	  void setMoveForward(bool f);
	  void setMoveBack(bool f);
	  void setMoveRight(bool f);
	  void setMoveLeft(bool f);
	  void setReload(bool f);
	  void setTir(bool f);
};
#endif
