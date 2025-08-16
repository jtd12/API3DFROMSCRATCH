#include"collision.hpp"

collisionsetup::collisionsetup()
{
	
	for(int i=0;i<5;i++)
	{
		std::string filename = "data/collision" + std::to_string(i) + ".obj";
		std::cout<<"i:"<<std::to_string(i)<<std::endl;
	 	collid.push_back(new object(vector3d(0,0,0), vector3d(0,0,0), vector3d(5,5,5), filename,true));
	}
	
	moveForward=false;
    moveBack=false;
    moveRight=false;
    moveLeft=false;
	tir=false;
	reload=false;
}

collisionsetup::~collisionsetup()
{
	for(int i=0;i<collid.size();i++)  
		 delete collid[i];
}

void collisionsetup::update(playersetup* player,camerasetup* camera,float deltaTime)
{
	
	 oldPos = camera->getCamera()->getPosition();
     testPos = oldPos;
     
     bool collisionDetected = false;
	  
	  for (int i = 0; i < collid.size(); i++) {
		    collid[i]->computeAABB();
			vector3d size = collid[i]->getSize();
		    vector3d center = collid[i]->getCenter();
			std::cout<<center<<std::endl;
		    vector3d objectMin = vector3d(center.x, center.y , center.z) -size;
		    vector3d objectMax = vector3d(center.x, center.y , center.z) + size;
		
		    vector3d invTest = vector3d(testPos.x * -1, testPos.y * -1, testPos.z * -1);
		
		   if (collid[i]->aabb.isPointInsideAABB(invTest, objectMin, objectMax)) {
		        collisionDetected = true;
		        break; // on arrête la boucle dès qu'on trouve une collision
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
    for(int i=0;i<collid.size();i++)  
	 collid[i]->draw(renderer, screenWidth, screenHeight, *camera->getCamera(), allTriangles);
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
	  
