#ifndef DEF_CAMERA_HPP
#define DEF_CAMERA_HPP
#include"../force3D/entities/camera/camera.h"
#define M_PI 3.141592653589793

struct Sphere {
    vector3d center;
    float radius;
};
class camerasetup
{
	Camera *camera;
	float sensitivity;
	float speed;
		
	public:
		camerasetup();
		~camerasetup();
		void rotateCamera(int dx, int dy);
		void moveCameraForward(float deltaTime );
		void moveCameraRight(float deltaTime );
		void projectionMode();
		Camera* getCamera();
		float getSpeed();
		
	
};
#endif
