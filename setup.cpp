
#include"setup.hpp"


 float kelvinR[19] ={1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,0.907,0.827,0.762,0.711,0.668,0.632,0.602};
 float kelvinG[19] ={0.007,0.126,0.234,0.349,0.454,0.549,0.635,0.710,0.778,0.837,0.890,0.937,0.888,0.839,0.800,0.766,0.738,0.714,0.693};
 float kelvinB[19] ={0.000,0.000,0.010,0.067,0.151,0.254,0.370,0.493,0.620,0.746,0.869,0.988,1.000,1.000,1.000,1.000,1.000,1.000,1.000};
	 
	 
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

		
		player->update(camera);
	//	camera->getCamera()->setGravity(-800,3000);
	
		height->update();
		
		decor->update();
		
		collision->update(player,camera,deltaTime);
		
		for (auto it = bullets.begin(); it != bullets.end();) {
		    if (!(*it)->update()) {
		        delete *it;                // libère le mesh
		        it = bullets.erase(it);    // retire du vecteur
		    } else {
		        ++it;
		    }
		}
		


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
	
	    	
		if (g->getGame()->getKeys()[SDL_SCANCODE_R])
	    {
	    collision->setReload(true);
	        
	    }
	    else
	    {
	    collision->setReload(false);
		}
		
		if (g->getGame()->getKeys()[SDL_SCANCODE_SPACE]) {
			collision->setTir(true);
			vector3d startPos = camera->getCamera()->getPosition() + camera->getCamera()->getForward() * 700.0f;
        	vector3d forward  = camera->getCamera()->getForward();
		  	bullets.push_back(new Bullet(startPos, forward, 200.0f, 50, "data/balle.obj"));
	}
	else
	    {
	    collision->setTir(false);
		}
		
		if (g->getGame()->getKeys()[SDL_SCANCODE_LCTRL] != 0)
		{
			collision->jump(true);
		}
	else {
    collision->jump(false);
}
	
	}
	
	
	void setup::draw(std::vector<Triangle>& allTriangles,SDL_Renderer* renderer, int screenWidth, int screenHeight)
	{

 	camera->projectionMode();

	vehicules->draw( allTriangles, renderer,screenWidth, screenHeight,vehicule,camera);
	
	decor->draw( allTriangles, renderer,screenWidth, screenHeight,camera);
	
	collision->draw(allTriangles, renderer,screenWidth, screenHeight,camera);
	
    player->draw(allTriangles,renderer,screenWidth,screenHeight, camera);
    
    for(int i=0;i<bullets.size();i++)
      bullets[i]->draw(allTriangles,renderer,screenWidth,screenHeight, camera);
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

