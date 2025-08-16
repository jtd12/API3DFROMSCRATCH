
#include"setup.hpp"

	setup::setup()
	{
		game=new gameLoop();
	}
	
	setup::~setup()
	{
		delete game;
		delete pixel;
		delete sky;
		delete camera;
		delete player;
		delete buffer;
		delete vehicules;
		delete height;
		delete decor;
		delete collision;
		
		 
		
	}
	
	void setup::init()
	{

	pixel=new drawPixels();
	buffer=new frameBuffer();
	
	decor=new decorsetup();
	
	collision=new collisionsetup();
	 
	
	vehicules=new vehiculesetup();
	vehicules->init(vehicule);
	
	height=new heightmapsetup();
	
	player=new playersetup();
	
	camera=new camerasetup();	

	sky=new skybox();

    
	}
	
	
	
	void setup::gestionEvents(setup* g)
	{

	
		int dx,dy;
		float sensitivity = 0.1f; 
		
		dx=g->getGame()->getMouseX();
		dy=g->getGame()->getMouseY();
		
		if(g->getGame()->getMousePressed())
		{
			camera->rotateCamera(dx,dy);

		}

	}
	
	void setup::update(setup* g)
	{
		
	    vehicules->update(vehicule);

		float deltaTime = .1f;
		bool collisionDetected = false;
		
		player->update(camera);
		camera->getCamera()->setGravity(-800,3000);
	
		height->update();
		
		decor->update();
		
		collision->update(player,camera,deltaTime);

	    if (g->getGame()->getKeys()[SDL_SCANCODE_Z])
	    {
	    	collision->setMoveForward(true);
	    }
	    else if (g->getGame()->getKeys()[SDL_SCANCODE_S])
	    {
	    	collision->setMoveBack(true);
	    }
	    else if (g->getGame()->getKeys()[SDL_SCANCODE_Q])
	    {	
		   collision->setMoveLeft(true);
	    }
	    
	    else if (g->getGame()->getKeys()[SDL_SCANCODE_D])
	    {
			collision->setMoveRight(true);
	    }
		else
		{
			collision->setMoveForward(false);
			collision->setMoveBack(false);
			collision->setMoveLeft(false);
			collision->setMoveRight(false);
			
		}
		if (g->getGame()->getKeys()[SDL_SCANCODE_A])
	    {
	    collision->setTir(true);
	        
	    }
	    else
	    {
	    collision->setTir(false);
		}
		
		if (g->getGame()->getKeys()[SDL_SCANCODE_R])
	    {
	    collision->setReload(true);
	        
	    }
	    else
	    {
	    collision->setReload(false);
		}
	
	}
	
	
	void setup::draw(std::vector<Triangle>& allTriangles,SDL_Renderer* renderer, int screenWidth, int screenHeight)
	{

 	camera->projectionMode();

	vehicules->draw( allTriangles, renderer,screenWidth, screenHeight,vehicule,camera);
	
	decor->draw( allTriangles, renderer,screenWidth, screenHeight,camera);
	
	collision->draw(allTriangles, renderer,screenWidth, screenHeight,camera);
	
    player->draw(allTriangles,renderer,screenWidth,screenHeight, camera);
	}



void setup::draw()
{
	
	pixel->getPixel()->clearBuffer();
    game->FillColor();
    SDL_Texture* texture;
    std::vector<Triangle> allTriangles;
    Uint32* framebuffer = new Uint32[WIDTH * HEIGHT]{};
    float* framebufferDepth = new float[WIDTH * HEIGHT];
    
     sky->drawBackground(framebuffer, WIDTH, HEIGHT);

	for(int i=0;i<vehicule.size();i++)
	    allTriangles.reserve(vehicule[i]->triangles.size());

	draw(allTriangles, game->getRenderer(),  WIDTH, HEIGHT);

    buffer->createTexture(game->getRenderer(),texture,WIDTH,HEIGHT);
 
	buffer->initializeZBuffer(framebufferDepth);
	
	height->draw(allTriangles,game->getRenderer(), WIDTH, HEIGHT , camera, pixel, framebuffer, framebufferDepth);

    pixel->draw( allTriangles,framebuffer,framebufferDepth, WIDTH, HEIGHT,  *camera->getCamera());
    

	buffer->updateTexture(game->getRenderer(),texture,framebuffer,WIDTH);
	
	buffer->destroyTexture(texture,framebuffer,framebufferDepth);


}

gameLoop * setup::getGame()
{
	return game;
}

