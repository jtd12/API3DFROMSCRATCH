#include"camera.h"

CameraComponent cam;


Camera::Camera()
{
	
}

Camera::~Camera()
{
	
}

void Camera::setPosition(vector3d pos)
 {
 	position=pos;
 }
 
 void Camera::setGravity(float gravity)
 {
 	position.y-=gravity;

 }
 
 
void Camera::setTarget(vector3d pos)
{
target=pos;			
}

void Camera::setUp(vector3d pos)
{
	up=pos;
}
		
vector3d Camera::getPosition() const
 {
 	return position;
 }

vector3d Camera::getTarget()
		{
			return target;
		}
		
vector3d Camera::getUp()
		{
			return up;
		}
		
		float Camera::getYaw()
		{
			return yaw;
		}
		
		float Camera::getPitch()
		{
			return pitch;
		}
		
		void Camera::setYaw(float y)
		{
			yaw=y;
		}
		
		void Camera::setPitch(float p)
		{
			pitch=p;
		}

    void Camera::updateAngles(float pitchDelta, float yawDelta) {
        pitch += pitchDelta;
        yaw += yawDelta;

        // Limite le pitch pour éviter une rotation excessive (ex: -80° à 80°)
        if(pitch>0.5f)
        {
        	pitch=0.5f;
		}
		if(pitch<-1.5f)
		{
			pitch=-1.5f;
		}
      
    }
    


Matrix4x4 Camera::getProjectionMatrix() const {

	
    Matrix4x4 projectionMatrix;

    // Calcul du facteur d'échelle basé sur le champ de vision (fov)
    float tanHalfFov = tan(cam.fov / 2.0f);

    // Remplissage de la matrice de projection perspective
    projectionMatrix.m[0][0] = 1.0f / (cam.aspect * tanHalfFov);
    projectionMatrix.m[0][1] = 0.0f;
    projectionMatrix.m[0][2] = 0.0f;
    projectionMatrix.m[0][3] = 0.0f;

    projectionMatrix.m[1][0] = 0.0f;
    projectionMatrix.m[1][1] = 1.0f / tanHalfFov;
    projectionMatrix.m[1][2] = 0.0f;
    projectionMatrix.m[1][3] = 0.0f;

    projectionMatrix.m[2][0] = 0.0f;
    projectionMatrix.m[2][1] = 0.0f;
    projectionMatrix.m[2][2] = (cam.far + cam.near) / (cam.near - cam.far);
    projectionMatrix.m[2][3] = (2.0f * cam.far * cam.near) / (cam.near - cam.far);

    projectionMatrix.m[3][0] = 0.0f;
    projectionMatrix.m[3][1] = 0.0f;
    projectionMatrix.m[3][2] = -1.0f;
    projectionMatrix.m[3][3] = 0.0f;

    return projectionMatrix;
}


    Matrix4x4 Camera::getViewMatrix(const Camera& camera,int inv)const {
    	vector3d right, up, forward;
    // Calcul du vecteur direction (forward)
    vector3d camTo=camera.target - camera.position;
    forward = camTo.normalize();
	
	
    // Calcul du vecteur droit (right), qui est perpendiculaire à "forward" et "up"
    right = forward.cross( forward,camera.up).normalize();

    // Calcul du vecteur up final
    up = forward.cross(right, forward);
    
    Matrix4x4 viewMatrix;
    viewMatrix.m[0][0] = right.x;   viewMatrix.m[0][1] = right.y;   viewMatrix.m[0][2] = right.z;   viewMatrix.m[0][3] = -right.dot(vector3d(-right.x,-right.y,-right.z),vector3d(position.x,position.y,position.z));
    viewMatrix.m[1][0] = up.x*inv;      viewMatrix.m[1][1] = up.y*inv;      viewMatrix.m[1][2] = up.z*inv;      viewMatrix.m[1][3] = -up.dot(vector3d(-up.x*inv,-up.y*inv,-up.z*inv),vector3d(position.x,position.y,position.z));
    viewMatrix.m[2][0] = forward.x; viewMatrix.m[2][1] = forward.y; viewMatrix.m[2][2] = forward.z; viewMatrix.m[2][3] = -forward.dot(vector3d(-forward.x,-forward.y,-forward.z),vector3d(position.x,position.y,position.z));
    viewMatrix.m[3][0] = 0;         viewMatrix.m[3][1] = 0;         viewMatrix.m[3][2] = 0;         viewMatrix.m[3][3] = 1;

    return viewMatrix;
    
   /*   vector3d forward = (target - position).normalize();
    vector3d right = up.cross(up, forward).normalize();
    vector3d realUp = forward.cross(right).normalize();

    Matrix4x4 viewMatrix;

    viewMatrix.m[0][0] = right.x;
    viewMatrix.m[0][1] = right.y;
    viewMatrix.m[0][2] = right.z;
    viewMatrix.m[0][3] = -right.dot(position);

    viewMatrix.m[1][0] = realUp.x;
    viewMatrix.m[1][1] = realUp.y;
    viewMatrix.m[1][2] = realUp.z;
    viewMatrix.m[1][3] = -realUp.dot(position);

    viewMatrix.m[2][0] = -forward.x;
    viewMatrix.m[2][1] = -forward.y;
    viewMatrix.m[2][2] = -forward.z;
    viewMatrix.m[2][3] = forward.dot(position);

    viewMatrix.m[3][0] = 0.0f;
    viewMatrix.m[3][1] = 0.0f;
    viewMatrix.m[3][2] = 0.0f;
    viewMatrix.m[3][3] = 1.0f;

    return viewMatrix;*/
}
    
    vector3d Camera::applyViewMatrix(const vector3d& v, const Camera& camera)const {
    vector3d right, up, forward;
   //getViewMatrix(camera, right, up, forward);

    // Translation pour déplacer le vertex par rapport à la position de la caméra
    vector3d translated = v - camera.position;

    // Appliquer la matrice de vue (produit scalaire pour projeter sur chaque axe)
    return {
        translated.dot(translated, right),
        translated.dot(translated, up),
        translated.dot(translated, vector3d(-forward.x,-forward.y,-forward.z))
    };
}
 void Camera::getOrientation(vector3d& forward, vector3d& right, vector3d& up) const {
        forward = vector3d::normalise((target - position));  // direction de la caméra
        right = this->up.cross(this->up,forward);
		right = vector3d::normalise(right); // axe droit
        up = forward.cross(forward,right);
		up=vector3d::normalise(up);      // axe haut
		
	}
		
	void Camera::moveForward(float speed) {
        vector3d forward = (target - position);
		vector3d::normalise(forward);
        position = position + forward * speed;
        target = target + forward * speed;
    }

    void Camera::moveRight(float speed) {
        vector3d forward = (target - position);
		vector3d::normalise(forward);
        vector3d right = up.cross(up,forward);
		vector3d::normalise(right);
        position = position + right * speed;
        target = target + right * speed;
    }


	void Camera::moveForwardSimple(float speed) {
        vector3d forward = (target - position);
        forward.y = 0;
		vector3d::normalise(forward);
        position = position + forward * speed;
        target = target + forward * speed;
    }

    void Camera::moveRightSimple(float speed) {
        vector3d forward = (target - position);
        forward.y = 0;
		vector3d::normalise(forward);
        vector3d right = up.cross(up,forward);
        right.y = 0;
		vector3d::normalise(right);
        position = position + right * speed;
        target = target + right * speed;
    }

	
    void Camera::moveUp(float speed) {
        position = position + vector3d(up.x,up.y,up.z) * speed;
        target = target + vector3d(up.x,up.y,up.z) * speed;
    }

    // Rotation autour de Y (gauche/droite)
    void Camera::rotateYaw(float angle) {
    	
    	
        float rad = angle * (3.14 / 180.0f);
    vector3d forward = target - position;

    // Conserver la composante Y inchangée lors de la rotation
    float cosA = cos(rad);
    float sinA = sin(rad);

    vector3d newForward(
        cosA * forward.x - sinA * forward.z, // Rotation autour de l'axe Y affecte X et Z
        forward.y,  // La composante Y reste inchangée
        sinA * forward.x + cosA * forward.z  // Rotation autour de l'axe Y affecte X et Z
    );

    target = position + newForward;
    }

    // Rotation autour de l'axe X (haut/bas)
    void Camera::rotatePitch(float angle) {
        float rad = angle * (3.14 / 180.0f);
        vector3d forward = (target - position);
        vector3d right = up.cross(up,forward);
		vector3d::normalise(right);

        float cosA = cos(rad);
        float sinA = sin(rad);

        vector3d newForward(
            forward.x,
            cosA * forward.y - sinA * forward.z,
            sinA * forward.y + cosA * forward.z
        );

        target = position + newForward;
    }
    
    vector3d Camera::getForward() const {
    return (target - position).normalize(); // ou normalize()
}

vector3d Camera::getRight() const {
    return getForward().cross(getForward(), up).normalize();
}

float Camera::getRotationX() const {
    // Pitch : rotation haut/bas
    vector3d forward = (target - position).normalize();
    float pitch = asin(forward.y);
    return pitch * (180.0f / M_PI); // conversion en degrés
}

float Camera::getRotationY() const {
    // Yaw : rotation gauche/droite
    vector3d forward = (target - position).normalize();
    float yaw = atan2(forward.x, forward.z);
    return yaw * (180.0f / M_PI); // conversion en degrés
}

float Camera::getRotationZ() const {
    // Roll : inclinaison gauche/droite
    vector3d forward = (target - position).normalize();
    vector3d right = forward.cross(forward, up).normalize();
    vector3d recalculatedUp = right.cross(right, forward).normalize();
    float roll = atan2(recalculatedUp.x, recalculatedUp.y);
    return roll * (180.0f / M_PI); // conversion en degrés
}


