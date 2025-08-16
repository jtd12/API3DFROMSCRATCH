#include"player.hpp"

playersetup::playersetup()
{
	myAnimatedObject=new object(vector3d(0,0,0), vector3d(0, 0, 0), vector3d(3, 3, 3), "data/animations/arme",false,260);
	
    moving=false;
    reload=false;
    tir=false;
}

playersetup::~playersetup()
{
	delete myAnimatedObject;
}

void playersetup::update(camerasetup* camera)
{
		std::srand(std::time(nullptr));
//std::cout<<"changeTimer"<<changeTimer<<std::endl;

		myAnimatedObject->getTranslationMatrix().setTranslation((camera->getCamera()->getPosition().x*-1),camera->getCamera()->getPosition().y*-1,camera->getCamera()->getPosition().z*-1);
		myAnimatedObject->getRotationMatrixX().setRotationX(-camera->getCamera()->getRotationX()* (M_PI/180.0f));
		myAnimatedObject->getRotationMatrixY().setRotationY(-camera->getCamera()->getRotationY()* (M_PI/180.0f));
		myAnimatedObject->getRotationMatrixZ().setRotationZ(-camera->getCamera()->getRotationZ()* (M_PI/180.0f));

		for (int frame = 0; frame < 260; ++frame) {
	    float deltaTime = 0.016f; // ~60 FPS
	    changeTimer += deltaTime;
	    //std::cout<<"changeTimer"<<changeTimer<<std::endl;
	
	    // Toutes les X secondes ? choisir une nouvelle animation
	    if (changeTimer >= changeInterval) {
	        changeTimer = 0.0f;
	        
	        if(moving)
	        {
	        int randomIndex = std::rand() % animationsMoving.size();
	        currentStartFrame = animationsMoving[randomIndex].startFrame;
	        currentEndFrame = animationsMoving[randomIndex].endFrame;
	        
	    	}
	    
		    else if(tir)
			{
				int randomIndex = std::rand() % animationsTir.size();
		        currentStartFrame = animationsTir[randomIndex].startFrame;
		        currentEndFrame = animationsTir[randomIndex].endFrame;
			}
			
			else if(reload)
			{
				int randomIndex = std::rand() % animationsRecharge.size();
		        currentStartFrame = animationsRecharge[randomIndex].startFrame;
		        currentEndFrame = animationsRecharge[randomIndex].endFrame;
			}
			
		    else
		    {
		    	int randomIndex = std::rand() % animationsIdle.size();
		        currentStartFrame = animationsIdle[randomIndex].startFrame;
		        currentEndFrame = animationsIdle[randomIndex].endFrame;
			}
		
		myAnimatedObject->setCurrentFrame(currentStartFrame); // on démarre depuis le début
	}
	    // On met à jour l’animation courante à chaque frame
	    myAnimatedObject->updateAnimation(1.0f, 1.0f,
	        currentStartFrame,
	        currentEndFrame
	    );

	} 
	    
}

void playersetup::draw(std::vector<Triangle>& allTriangles,SDL_Renderer* renderer, int screenWidth, int screenHeight, camerasetup* camera)
{
	 myAnimatedObject->drawAnimatedObject(renderer,screenWidth, screenHeight, *camera->getCamera(), allTriangles);
}

void playersetup::setReload(bool f)
{
	reload=f;
}

void playersetup::setTir(bool f)
{
		tir=f;	
}

void playersetup::setMoved(bool f)
{
	moving=f;
}
		
