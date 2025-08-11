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
		for (int i = 0; i < decor.size(); i++) 
		 delete decor[i];
		delete buffer;
		
		for(int i=0;i<collid.size();i++)  
		 delete collid[i];
		 
		 for(int i=0;i<wheel.size();i++) 
		  delete wheel[i];
	}
	
	void setup::init(std::vector<object*>& vehicule)
	{
	//position=vector3d(0,0,0);
	//position2=vector3d(20,0,50);

	pixel=new drawPixels();
	buffer=new frameBuffer();
	
	for(int i=0;i<4;i++)
	{
	std::string filename = "data/decor" + std::to_string(i) + ".obj";
	std::cout<<"i:"<<std::to_string(i)<<std::endl;
	decor.push_back(new object(vector3d(0,0,-3000), vector3d(0,0,0), vector3d(5,5,5), filename,false));
	}
	
	gravity=1.0f;
	
	for(int i=0;i<5;i++)
	{
		std::string filename = "data/collision" + std::to_string(i) + ".obj";
		std::cout<<"i:"<<std::to_string(i)<<std::endl;
	 	collid.push_back(new object(vector3d(0,0,0), vector3d(0,0,0), vector3d(5,5,5), filename,true));
	}
	 
/*	for(int i=0;i<collid.size();i++)  
	  collid[i]->wireframe=true;*/
	  
    // Chargement unique des modèles
    object* model1 = new object(vector3d(0,0,0), vector3d(0,0,0), vector3d(15,15,15), "data/voiture.obj",false);
    object* model2 = new object(vector3d(0,0,0),vector3d(0,0,0), vector3d(15,15,15), "data/voiture2.obj",false);

    for (int i = 0; i < 6; ++i) {
        vector3d pos=vector3d(-900-(i * 5800.0f), -3000.0f,5000-(i* 400.0f));
        vector3d rot(0.0f, i * 10.0f, 0.0f);

        object* baseModel = (i % 2 == 0) ? model1 : model2;

        // Crée une instance à partir du modèle partagé
        object* instance = new object(pos, rot, vector3d(500,500,500), baseModel);

        vehicule.push_back(instance);
        
    position.push_back(pos);
    rotation.push_back(rot);

    // 4 roues par véhicule
    for (int j = 0; j < 4; ++j) {
        wheel.push_back(new object(vector3d(0, 0, 0), vector3d(0, 0, 0), vector3d(500, 500, 500), "data/roues.obj",false));
    }
}
	
	camera=Camera(vector3d(150,-10,-8000),vector3d(0,0,0),vector3d(0,-1,0));
//camera=Camera(vector3d(10,-10,50),vector3d(0,0,0),vector3d(0,-1,0));
	sky=new skybox();
	
	heightMap=new height();
	heightMap->loadHeightMap("data/Heightmap2.bmp");

	myAnimatedObject=new object(vector3d(0,0,0), vector3d(0, 0, 0), vector3d(3, 3, 3), "data/animations/arme",false,100);
	
	speed=.5f;
	}
	
	
	
	void setup::gestionEvents(setup* g)
	{

	float deltaTime=.3f;
	
    // Réinitialiser la vitesse
     oldPos = camera.getPosition();
     testPos = oldPos;
    
    // On copie la position courante
	
	
		int dx,dy;
		float sensitivity = 0.1f; 
		
		dx=g->getGame()->getMouseX();
		dy=g->getGame()->getMouseY();
		
		if(g->getGame()->getMousePressed())
		{
		camera.rotateYaw(dx  *  sensitivity);
		camera.rotatePitch(-dy  *  sensitivity);
	}
	/*	
    if (up) camera.moveForward(speed * deltaTime);
    if (down) camera.moveForward(-speed * deltaTime);
    if (right) camera.moveRight(-speed * deltaTime);
    if (left) camera.moveRight(speed * deltaTime);*/
	}
	
	void setup::collision(vector3d p1,vector3d p2) {  
    float d = sqrt(((p1.x - p2.x) * (p1.x - p2.x)) +   
             ((p1.y - p2.y) * (p1.y - p2.y)) +   
             ((p1.z - p2.z) * (p1.z - p2.z)));  
             
    if(d<2000)
    {
    	std::cout<<"collision"<<std::endl;
	}
}  


	void setup::update(std::vector<object*>& vehicule,setup* g)
	{
		
		


		myAnimatedObject->getTranslationMatrix().setTranslation((camera.getPosition().x*-1),camera.getPosition().y*-1,camera.getPosition().z*-1);
		myAnimatedObject->getRotationMatrixX().setRotationX(-camera.getRotationX()* (M_PI/180.0f));
		myAnimatedObject->getRotationMatrixY().setRotationY(-camera.getRotationY()* (M_PI/180.0f));
		myAnimatedObject->getRotationMatrixZ().setRotationZ(-camera.getRotationZ()* (M_PI/180.0f));
		myAnimatedObject->updateAnimation(5.0f);
	
		
		for(int i=0;i<vehicule.size();i++)
{
	position[i].y-=100.0f;
	
	if(position[i].y< -5000.0f)
	{
		position[i].y=-5000.0f;
	}
}

		vector3d cameraVelocity; // vitesse de déplacement (x, y, z)
		float dampingFactor = 5.0f; // plus c'est haut, plus l'arrêt est rapide
		float sizeObject=9000.0f;
		float deltaTime = .3f;
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
    if (g->getGame()->getKeys()[SDL_SCANCODE_Z])
        camera.moveForward(speed * deltaTime);
    if (g->getGame()->getKeys()[SDL_SCANCODE_S])
        camera.moveForward(-speed * deltaTime);
    if (g->getGame()->getKeys()[SDL_SCANCODE_Q])
        camera.moveRight(-speed * deltaTime);
    if (g->getGame()->getKeys()[SDL_SCANCODE_D])
        camera.moveRight(speed * deltaTime);

    std::cout << "Pas de collision" << std::endl;
} else {
    // Collision détectée, on empêche le déplacement ou on recule la caméra
    camera.moveForward(-speed * 10.5f * deltaTime);
    std::cout << "Collision avec un objet !" << std::endl;
}

    	

//std::cout << "Objet au centre : " << center.x << ", " << center.z*-1 << ", " << center.y << std::endl;
		//	collision(vector3d(camera.getPosition().x*-1,camera.getPosition().y*-1,camera.getPosition().z*-1),vector3d(collid->));
	//	collision(vector3d(camera.getPosition().x*-1,camera.getPosition().y*-1,camera.getPosition().z*-1),vector3d(collid->getCenter().x,collid->getCenter().z*-1,collid->getCenter().y));
    /*	
		for(int i=0;i<rotation.size();i++)
		  rotation[i].y+=0.05f;
		
		rotation[0].z+=0.1f;*/
	}
	
	
	

	void setup::draw(std::vector<Triangle>& allTriangles,SDL_Renderer* renderer, int screenWidth, int screenHeight, const Camera& camera, std::vector<object*>& vehicule)
{
cam.fov=80.0f * (M_PI / 180.0f);
cam.aspect=static_cast<float>(800) / static_cast<float>(600);
cam.near=1.0f;
cam.far=1000.0f;


	//vehicule[0]->applyMatrix(position,vector3d(0,rotY,rotZ),vector3d(5,5,5));
	//vehicule[1]->applyMatrix(position2,vector3d(0,rotY2,0),vector3d(5,5,5));
	for(int i=0;i<vehicule.size();i++)
{
	vehicule[i]->getTranslationMatrix().setTranslation(position[i].x,position[i].y,position[i].z);
	vehicule[i]->getRotationMatrixX().setRotationX(rotation[i].x);
	vehicule[i]->getRotationMatrixY().setRotationY(rotation[i].y);
	vehicule[i]->getRotationMatrixZ().setRotationZ(rotation[i].z);

}
	
	 vector3d wheelOffsets[4] = {
        vector3d(1500, -300, 1500),  // Avant gauche
        vector3d(-2200, -300, 1500), // Avant droite
        vector3d(1500, -300, -1500), // Arrière gauche
        vector3d(-2200, -300, -1500) // Arrière droite
    };

for(int j=0;j<vehicule.size();j++)
{
	
Matrix4x4 rotXMat;
rotXMat.setRotationX(rotation[j].x);
Matrix4x4 rotYMat;
rotYMat.setRotationY(rotation[j].y);
Matrix4x4 rotZMat;
rotZMat.setRotationZ(rotation[j].z);
Matrix4x4 translationMat;



translationMat.setTranslation(position[j].x, position[j].y, position[j].z);

Matrix4x4 vehicleMatrix = translationMat * rotXMat * rotYMat * rotZMat;

for (int i = 0; i < 4; i++) {
	int wheelIndex = j * 4 + i; // ? Index correct pour chaque roue
    vector3d worldPos = vehicleMatrix.apply(wheelOffsets[i]);

    // ? Met à jour la position de la roue
    wheel[wheelIndex]->setLocation(worldPos);

    // ? Neutralise la rotation (pour qu'elles ne tournent pas en Y)
    wheel[wheelIndex]->setRotation(vector3d(0, rotation[j].y, 0)); // Ou rotationX seulement si tu veux qu'elles tournent en roulant
	
	  if (i == 2 || i == 3) {
        wheel[wheelIndex]->setRotation(vector3d(0,rotation[j].y+3,0));  // Rotation Y de 180 degrés
    }
    // ? Applique les matrices en fonction de position/rotation/scale
    wheel[wheelIndex]->applyMatrix();
}

}
	for(int i=0;i<vehicule.size();i++)
	{
//	vehicule[i]->rotationMatrixY.setRotationY(rot);
    vehicule[i]->draw(renderer, screenWidth, screenHeight, camera, allTriangles);
		
	}
	 for (int i = 0; i < wheel.size(); i++) {
        wheel[i]->draw(renderer, screenWidth, screenHeight, camera, allTriangles);
    }
	
	for (int i = 0; i < decor.size(); i++) 
	 decor[i]->draw(renderer, screenWidth, screenHeight, camera, allTriangles);
	
	for(int i=0;i<collid.size();i++)  
	 collid[i]->draw(renderer, screenWidth, screenHeight, camera, allTriangles);
	 
	 
	 heightMap->getTranslationMatrix().setTranslation(-100000,-21800,-100000);
	 
	 myAnimatedObject->drawAnimatedObject(renderer,screenWidth, screenHeight, camera, allTriangles);
    
}



void setup::draw(std::vector<object*>& vehicule)
{
	
	pixel->getPixel()->clearBuffer();
    game->FillColor();
    SDL_Texture* texture;
    std::vector<Triangle> allTriangles;
    Uint32* framebuffer = new Uint32[WIDTH * HEIGHT]{};
    float* framebufferDepth = new float[WIDTH * HEIGHT];
    
     sky->drawBackground(framebuffer, WIDTH, HEIGHT);
    Material mat;
    mat.diffuseColor = vector3d(1.0f, 1.0f, 1.0f);

   vector3d v1(-1.0f, -1.0f, 50.0f);  // Bas gauche
vector3d v2( 1.0f, -1.0f, 50.0f);  // Bas droit
vector3d v3( 0.0f,  1.0f, 50.0f);  // Haut
     

    // Transforme et projette ce triangle (le rend)
   // renderTriangles(tri, camera, allTriangles);
    
   // heightMap->drawHeight(1.0f,10.0f,pixel->getPixel(),framebuffer,framebufferDepth, WIDTH, HEIGHT, camera);
    
for(int i=0;i<vehicule.size();i++)
    allTriangles.reserve(vehicule[i]->triangles.size());

	draw(allTriangles, game->getRenderer(),  WIDTH, HEIGHT,  camera,  vehicule);

    buffer->createTexture(game->getRenderer(),texture,WIDTH,HEIGHT);
    
 
	buffer->initializeZBuffer(framebufferDepth);
   
   	heightMap->drawHeight(1000,15000, pixel->getPixel(),framebuffer, framebufferDepth, WIDTH, HEIGHT, camera);
	
     //	if (isTriangleVisible(normal, car.getPosition(), tri[0])) {
//	drawTriangle(allTriangles, framebuffer, framebufferDepth,
            // WIDTH, HEIGHT,
            // v1, v2, v3,
             //camera);


    pixel->draw( allTriangles,framebuffer,framebufferDepth, WIDTH, HEIGHT,  camera);
    

	buffer->updateTexture(game->getRenderer(),texture,framebuffer,WIDTH);
	
	buffer->destroyTexture(texture,framebuffer,framebufferDepth);


}

gameLoop * setup::getGame()
{
	return game;
}

