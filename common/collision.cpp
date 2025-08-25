#include"collision.hpp"

collisionsetup::collisionsetup()   // on copie la sphère passée en paramètre
{
	
	for(int i=0;i<4;i++)
	{
		std::string filename = "data/collision" + std::to_string(i) + ".obj";
		std::cout<<"i:"<<std::to_string(i)<<std::endl;
	 	collid.push_back(new object(vector3d(0,0,0), vector3d(0,0,0), vector3d(5,5,5), filename,true));
	}
	
	for(int i=0;i<5;i++)
	{
		std::string filename = "data/collisionPlane" + std::to_string(i) + ".obj";
		std::cout<<"i:"<<std::to_string(i)<<std::endl;
	 	collidPlane.push_back(new object(vector3d(0,0,0), vector3d(0,0,0), vector3d(5,5,5), filename,true));
	}
	
	moveForward=false;
    moveBack=false;
    moveRight=false;
    moveLeft=false;
	tir=false;
	reload=false;
	fly=false;
	gravity=-1500.0f;
	
//	sphere.center = camera->getCamera()->getPosition();
  //  sphere.radius = 200.0f;
}

collisionsetup::~collisionsetup()
{
	for(int i=0;i<collid.size();i++)  
		 delete collid[i];
}
/*
bool collisionsetup::intersect(const Sphere& s, const AABB& b) {
    float dmin = 0;

    if (s.center.x < b.getMin().x) dmin += (s.center.x - b.getMin().x) * (s.center.x - b.getMin().x);
    else if (s.center.x > b.getMax().x) dmin += (s.center.x - b.getMax().x) * (s.center.x - b.getMax().x);

    if (s.center.y < b.getMin().y) dmin += (s.center.y - b.getMin().y) * (s.center.y - b.getMin().y);
    else if (s.center.y > b.getMax().y) dmin += (s.center.y - b.getMax().y) * (s.center.y - b.getMax().y);

    if (s.center.z < b.getMin().z) dmin += (s.center.z - b.getMin().z) * (s.center.z - b.getMin().z);
    else if (s.center.z > b.getMax().z) dmin += (s.center.z - b.getMax().z) * (s.center.z - b.getMax().z);

    return dmin <= s.radius * s.radius;
}*/


void collisionsetup::jump(bool f) {
   fly=f;
}


void collisionsetup::update(playersetup* player,camerasetup* camera,float deltaTime)
{
	 oldPos = camera->getCamera()->getPosition();
     testPos = oldPos;
     vector3d invTest = vector3d(testPos.x * -1, testPos.y * -1, testPos.z * -1);
     
    // vector3d camPos = camera->getCamera()-getPosition();
     bool collisionDetected = false;
     
    if(!fly)
    {
    if (!onGround) {
	       velocityY += gravity * deltaTime;  // accélération vers le bas
	    }
	}    
	    else
	    {
	    	velocityY=3000.0f;
		}
	    
	    
	    camera->getCamera()->setGravity(velocityY* deltaTime);
	
	    // appliquer la vitesse verticale à la position
	  
	  for (int i = 0; i < collid.size(); i++) {
	
		    if (collid[i]->checkCollisionWithCamera(invTest, 5000.0f)) 
		    {
		        collisionDetected = true;
		        std::cout << "Collision caméra avec objet !" << std::endl;
		        break; // on arrête la boucle dès qu'on trouve une collision
			}
		}
			
			for (int i = 0; i < collidPlane.size(); i++) {
		    
		     if (collidPlane[i]->checkCollisionWithCamera(invTest, 4000.0f)) 
			{
			
			    // Corriger la position Y de la caméra
			   // camera->getCamera()->setPosition(vector3d(camera->getCamera()->getPosition().x,collidPlane[i]->getLocation().y,camera->getCamera()->getPosition().z));
			    velocityY = 0.0f;
        		onGround = true;
			
			    std::cout << "Collision avec le sol mesh !" << std::endl;
			    break;
			
		}	
			else
			{
				onGround = false;
			}
			
			if(onGround)
			{
				camera->getCamera()->setPosition(vector3d(camera->getCamera()->getPosition().x,collidPlane[i]->getLocation().y,camera->getCamera()->getPosition().z));
			}
			
			
	  
	}
	
	
	if (!collisionDetected) {
	    // Pas de collision détectée, on peut déplacer la caméra
	    if(moveForward)
	    {
	    	player->setMoved(true);
	        camera->moveCameraForward(camera->getSpeed() * deltaTime);
		}
	    else if (moveBack)
	    {
	    	player->setMoved(true);
	        camera->moveCameraForward(-camera->getSpeed() * deltaTime);
	    }
	    else if (moveLeft)
	    {	
		   player->setMoved(true);
	       camera->moveCameraRight(-camera->getSpeed() * deltaTime );
	    }
	    
	    else if (moveRight)
	    {
			player->setMoved(true);
	        camera->moveCameraRight(camera->getSpeed() * deltaTime );
	    }
	    else
	    {
	    	player->setMoved(false);
		}
		
		if (tir)
	    {
	    player->setTir(true);
	        
	    }
	    else
	    {
	    player->setTir(false);
		}
		
		if (reload)
	    {
	    player->setReload(true);
	        
	    }
	    else
	    {
	    player->setReload(false);
		}
	
	    std::cout << "Pas de collision" << std::endl;
	} else {
	    // Collision détectée, on empêche le déplacement ou on recule la caméra
	    camera->moveCameraForward(-camera->getSpeed() * 10.0f  * deltaTime);
	    std::cout << "Collision avec un objet !" << std::endl;
	}
}

void collisionsetup::draw(std::vector<Triangle>& allTriangles,SDL_Renderer* renderer, int screenWidth, int screenHeight, camerasetup* camera)
{
    //for(int i=0;i<collid.size();i++)  
	 //collid[i]->draw(renderer, screenWidth, screenHeight, *camera->getCamera(), allTriangles);
	 
	for(int i=0;i<collidPlane.size();i++)  
	 collidPlane[i]->draw(renderer, screenWidth, screenHeight, *camera->getCamera(), allTriangles);
}

void collisionsetup::setMoveForward(bool f)
{
	moveForward=f;
}

void collisionsetup::setMoveBack(bool f)
{
	moveBack=f;
}

void collisionsetup::setMoveRight(bool f)
{
	moveRight=f;
}

void collisionsetup::setMoveLeft(bool f)
{
	moveLeft=f;
}

void collisionsetup::setReload(bool f)
{
	reload=f;
}

void collisionsetup::setTir(bool f)
{
	tir=f;
}
	  
