#ifndef DEF_CAMERA_H
#define DEF_CAMERA_H
#include<stdio.h>
#include<iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>
#include <chrono>
#include <thread>
#include <conio.h> // Pour _kbhit() et _getch()
#include <algorithm>
#include <cfloat>
#include<cmath>
#define MAXLEVEL 4
#include <cstdlib>
#include <algorithm>  // Pour std::swap
#include"../../maths/matrix.hpp"
#include"../../maths/vector.h"



struct CameraComponent
{
	float fov;
	float aspect;
	float near;
	float far;	
};

extern CameraComponent cam;

class Camera {
	
	vector3d position;
	vector3d target;
	vector3d up;
	float pitch; // Rotation vers le haut/bas
    float yaw;   // Rotation gauche/droite
    
	public:
	Camera();
	~Camera();
    Camera(const vector3d& pos, const vector3d& tgt, const vector3d& upVec)
        : position(pos), target(tgt), up(upVec) {}
        
        void getOrientation(vector3d& forward, vector3d& right, vector3d& up) const; 
	    void moveForward(float speed);
	    void moveRight(float speed);
	    void moveUp(float speed);
     	void rotateYaw(float angle);
     	void rotatePitch(float angle);
     	Matrix4x4 getViewMatrix(const Camera& camera,int inv)const;
    	vector3d applyViewMatrix(const vector3d& v, const Camera& camera)const;
     	Matrix4x4 getProjectionMatrix() const;
     	vector3d getPosition() const;
		vector3d getTarget();
		vector3d getUp();
		void updateAngles(float pitchDelta, float yawDelta);
		void setPosition(vector3d pos);
		void setTarget(vector3d pos);
		void setUp(vector3d pos);
		float getYaw();
		float getPitch();
		void setYaw(float y);
		void setPitch(float p);
    vector3d getForward() const;
    vector3d getRight() const;

     
};
#endif
