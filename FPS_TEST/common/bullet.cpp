#include"bullet.hpp"

Bullet::Bullet(vector3d startPos, vector3d forward, float speed, int ttl, const std::string& filename)
{
	instanceBullet(startPos,  forward,  speed,  ttl,  filename);
}

Bullet::~Bullet()
{
	delete mesh;
}

bool Bullet::update()
{

	    ttl--;

	    if (ttl <= 0) {
	       return false;
	    }
	
	    
	    // Déplacer la balle
	   pos = pos + dir * speed;
	   
    // Mettre à jour la matrice de translation du mesh
       mesh->getTranslationMatrix().setTranslation(
        pos.x * -1,   // ?? si ton moteur utilise la position inversée
        pos.y * -1,
        pos.z * -1
    );
    return true;

}

void Bullet::instanceBullet(vector3d startPos, vector3d forward, float speed, int ttl, const std::string& filename)
{
	    this->pos = startPos;
        this->dir = forward.normalize();
	    this->speed =200.0f; // vitesse
	    this->ttl = 50;     // durée de vie en frames (~3s à 60fps)
	
	    this->mesh = new object(startPos, vector3d(0,0,0), vector3d(0.5f,0.5f,0.5f), filename, false);
	    
	    float yaw   = atan2(forward.x, forward.z);
        float pitch = asin(-forward.y);
		
		mesh->getRotationMatrixY().setRotationY(yaw * M_PI / 180.0f);
	   	mesh->getRotationMatrixX().setRotationX(pitch * M_PI / 180.0f);
	   	mesh->getScaleMatrix().setScaling(2.0f,2.0f,2.0f);
	   	
	   	float r = static_cast<float>(rand() % 256) / 255.0f;
    	float g = static_cast<float>(rand() % 256) / 255.0f;
    	float b = static_cast<float>(rand() % 256) / 255.0f;
	   	mesh->setColor(vector3d(r, g, b));


}

void Bullet::draw(std::vector<Triangle>& allTriangles,SDL_Renderer* renderer, int screenWidth, int screenHeight,camerasetup* camera)
{

    // Dessiner
    mesh->draw(renderer, screenWidth, screenHeight, *camera->getCamera(), allTriangles);

}
