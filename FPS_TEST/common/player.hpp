#ifndef DEF_PLAYER_HPP
#define DEF_PLAYER_HPP
#include"../force3D/entities/object/object.h"
#include"../force3D/loader/objloader.hpp"
#include"camera.hpp"



struct AnimationRange {
    int startFrame;
    int endFrame;
};



class playersetup
{
	
	std::vector<AnimationRange> animationsIdle = {
        {50, 70},
    	{60, 90},
    	{40, 50},
    	{50, 51},
    	{55, 56},
    	{60, 61},
    	{60, 61},
    
    };
    
    	std::vector<AnimationRange> animationsMoving = {
        {130, 150},
        {20, 22},
        {200, 210},
        {20, 22},
        {20, 22},
        {20, 22},
        
    };
    
    	std::vector<AnimationRange> animationsTir = {
        {10, 100},
        {0, 70},
        
    };
    
    	std::vector<AnimationRange> animationsRecharge = {
        {100, 150},
        {150, 200},
        
    };
    
    float changeTimer = 0.0f;
	float changeInterval = 90.0f; // toutes les 5 secondes
	int currentStartFrame = 0;
	int currentEndFrame = 260;
	bool moving;
	bool reload;
	bool tir;
	object* myAnimatedObject;
	
	public:
		playersetup();
		~playersetup();
		void update(camerasetup* camera);
		void draw(std::vector<Triangle>& allTriangles,SDL_Renderer* renderer, int screenWidth, int screenHeight, camerasetup* camera);
		void setReload(bool f);
		void setTir(bool f);
		void setMoved(bool f);
		object* getMyAnimatedObject();
};

#endif
