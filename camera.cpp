#include"camera.hpp"

camerasetup::camerasetup()
{
	camera=new Camera(vector3d(100,-1000,-7000),vector3d(0,0,0),vector3d(0,-1,0));
	sensitivity=0.1f;
	speed=.5f;
}

camerasetup::~camerasetup()
{
	delete camera;
}

void camerasetup::rotateCamera(int dx, int dy)
{
	camera->rotateYaw(dx  *  sensitivity);
	camera->rotatePitch(-dy  *  sensitivity);
}

void camerasetup::moveCameraForward(float deltaTime )
{
	 camera->moveForwardSimple(deltaTime);
}

void camerasetup::moveCameraRight(float deltaTime )
{
camera->moveRightSimple(deltaTime);
}

void camerasetup::projectionMode()
{
cam.fov=60.0f * (M_PI / 180.0f);
cam.aspect=static_cast<float>(800) / static_cast<float>(600);
cam.near=10.0f;
cam.far=1000.0f;
}

Camera* camerasetup::getCamera()
{
	return camera;
}

float camerasetup::getSpeed()
{
	return speed;
}
