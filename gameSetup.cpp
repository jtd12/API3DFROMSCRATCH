#include"gameSetup.hpp"

setup::setup()
{
game=new gameLoop();

}




setup::~setup()
{

}

void setup::init()
{


	std::vector<vector3d> waypoints = {
    {5000, 1100, -1000}, {1000, 1000, -6700}, {-3000, 2000, -4000},{3000, 2000, 13000}, {-5000, 800, 14000}, {-27000,1000,12000},{-35000,1000,8000},{-41000,1000,7000},{-42000,1000,-2000},{8000, 1100, -12500}
    ,{18000, 1100, -9000},{30000, 1100, 1000},{31000, 1100, 4000},{33000, 1100, 15000},{34000, 1100, 15900},{32500, 1100, 22000},{22000, 1100, 45000},{20000, 1100, 50000},{18000, 1100, 55000},{5000, 1100, 65000},
 
};


controlPoints = {
    {20000, 1100, 0}, {0, 1000, -5000}, {-4000, 2000, 14000}, {-40000, 800, 0}, {27000,1000,-500},{7000,1000,65000},{5000,1000,5000},{-4000,1000,-100},{200, 1100, -4000},    // fermé avec hauteur égale
        // répétition du premier
      // optionnel : pour continuité Catmull-Rom // Fermeture
};
race=new raceTrack();

height=new heightmapsetup();
	
	
for (auto& p : controlPoints) {
   p.y= height->getHeightAt(p.x, p.z, TERRAINHEIGHT, TERRAINSIZE);
	
}


race->initializeTrack(controlPoints, 70, 3400.0f,*height); // Générer la piste

trackTriangles = race->generateTrackMesh(race->trackEdges,10.0f);
borderTriangles = race->generateTrackBorders(race->trackEdgesElevated,*height,TERRAINHEIGHT,TERRAINSIZE-9900,20);
//terrainTriangles = race->generateTerrain(150000, 150000, 100);

	collid=new collisions();
	carAABB=new AABB();
	
	pixel=new drawPixels();
	buffer=new frameBuffer();
	
	if (TTF_Init() == -1) {
    std::cerr << "Erreur SDL_ttf : " << TTF_GetError() << std::endl;
	return;
}

TTF_Font* font = TTF_OpenFont("data/font/arial.TTF", 24);

if (!font) {
    std::cout << "Impossible de charger la police : " << TTF_GetError() << std::endl;
}

SDL_Color white = {255, 255, 255};

 myButton.push_back( new Button({30, 650, 100, 45}, "avancer", font, white));
 myButton.push_back( new Button({180, 650, 100, 45}, "reculer", font, white));


pannel=new Button({0, 650, 800, 100});



	car=new vehiculesetup();
	
	car->init(vehicule,vector3d(6000,1800,15000),vector3d(0,0,0),vector3d(50,50,50));
	

	
	std::vector<std::string> filenames = {
    "data/voiture0.obj",
    "data/voiture1.obj",
    "data/voiture2.obj",
    "data/voiture3.obj",
    "data/voiture4.obj",
  
};

	srand((unsigned)time(0));

for (int i=0; i<9; i++) {
        std::string filename = filenames[i % filenames.size()];  // cycle des modèles
        mesh* sharedMesh = ModelManager::getModel(filename);

        int startIndex = rand() % waypoints.size();
      //  vector3d startPos = waypoints[startIndex];

        // Crée la voiture AI et la stocke dans le vecteur global
        vehiculesAISetup* carAI = new vehiculesAISetup( sharedMesh, waypoints, startIndex);

        // vitesse aléatoire
        carAI->setSpeed(static_cast<float>(rand()) / RAND_MAX * 1000.0f);

        allCarsAI.push_back(carAI);
    }
    
	camera=new camerasetup();	

	sky=new skybox();
	
int numCubes = 50;  // nombre de cubes
float minX = -75000.0f;
float maxX = 75000.0f;
float minZ = -75000.0f;
float maxZ = 75000.0f;
float yBase = 2000.0f;  // hauteur de base, on ajustera avec le terrain si besoin

int cubesCreated = 0;
while (cubesCreated < numCubes) {
    float x = minX + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (maxX - minX);
    float z = minZ + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (maxZ - minZ);
	
	if (!race || race->trackEdges.empty()) {
    std::cerr << "Erreur : piste non initialisée" << std::endl;
    return;
}

    if (race->isPointOnTrack(x, z)) continue;

    float y = height->getHeightAt(x, z, TERRAINHEIGHT, TERRAINSIZE-9900);
    vector3d pos(x, y+100, z);

    float randAngle = static_cast<float>(rand() % 360);
    float scaleX = 50.0f + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 50.0f;
    float scaleY = 50.0f + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 100.0f;
    float scaleZ = 50.0f + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 70.0f;

    decorSetup* newCube = new decorSetup(pos, vector3d(0, randAngle, 0),vector3d(scaleX,scaleY,scaleZ), "data/cubes.obj");
    cubes.push_back(newCube);
    cubesCreated++;
}


int numArbres = 50;  // nombre de cubes
int arbresCreated = 0;
while (arbresCreated < numArbres) {
    float x = minX + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (maxX - minX);
    float z = minZ + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (maxZ - minZ);
	
	if (!race || race->trackEdges.empty()) {
    std::cerr << "Erreur : piste non initialisée" << std::endl;
    return;
}

    if (race->isPointOnTrack(x, z)) continue;

    float y = height->getHeightAt(x, z, TERRAINHEIGHT, TERRAINSIZE-9900);
    vector3d pos(x, y + 100, z);

    float randAngle = static_cast<float>(rand() % 360);
    float scaleX = 300.0f;
    float scaleY = 300.0f;
    float scaleZ = 300.0f;

    decorSetup* newArbre = new decorSetup(pos, vector3d(0, randAngle, 0),vector3d(scaleX,scaleY,scaleZ), "data/arbre.obj");
	arbres.push_back(newArbre);
    arbresCreated++;
}

	
int numTriangles = 150;  // nombre de cubes
int trianglesCreated = 0;
while (trianglesCreated < numTriangles) {
    float x = minX + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (maxX - minX);
    float z = minZ + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (maxZ - minZ);
	
	if (!race || race->trackEdges.empty()) {
    std::cerr << "Erreur : piste non initialisée" << std::endl;
    return;
}

    if (race->isPointOnTrack(x, z)) continue;

    float y = height->getHeightAt(x, z, TERRAINHEIGHT, TERRAINSIZE-9900);
    vector3d pos(x, y + 100, z);

    float randAngle = static_cast<float>(rand() % 360);
    float scaleX = 300.0f;
    float scaleY = 300.0f;
    float scaleZ = 300.0f;

    decorSetup* newTriangle = new decorSetup(pos, vector3d(0, randAngle, 0),vector3d(scaleX,scaleY,scaleZ), "data/triangles.obj");
    triangles.push_back(newTriangle);
    trianglesCreated++;
}

int numAnimal = 100;  // nombre de cubes
int animalCreated = 0;
while (animalCreated < numAnimal) {
    float x = minX + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (maxX - minX);
    float z = minZ + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (maxZ - minZ);
	
	if (!race || race->trackEdges.empty()) {
    std::cerr << "Erreur : piste non initialisée" << std::endl;
    return;
}

    if (race->isPointOnTrack(x, z)) continue;

    float y = height->getHeightAt(x, z, TERRAINHEIGHT, TERRAINSIZE-9900);
    vector3d pos(x, y + 100, z);

    float randAngle = static_cast<float>(rand() % 360);
    float scaleX = 300.0f;
    float scaleY = 300.0f;
    float scaleZ = 300.0f;

    decorSetup* newAnimal = new decorSetup(pos, vector3d(0, randAngle, 0),vector3d(scaleX,scaleY,scaleZ), "data/animal.obj");
    animals.push_back(newAnimal);
    animalCreated++;
}

text.push_back(new Button({400, 650, 100, 45}, "Vitesse: " + std::to_string(car->getSpeed()) + "km/h", font, white));

text.push_back(new Button({100, 550, 100, 45},"Car1 Speed: " + std::to_string(allCarsAI[0]->getSpeed()/3) + "km/h" , font, white));
text.push_back(new Button({100, 500, 100, 45},"Car2 Speed: " + std::to_string(allCarsAI[1]->getSpeed()/3) + "km/h" , font, white));
text.push_back(new Button({100, 450, 100, 45},"Car3 Speed: " + std::to_string(allCarsAI[2]->getSpeed()/3) + "km/h" , font, white));
text.push_back(new Button({100, 400, 100, 45},"Car4 Speed: " + std::to_string(allCarsAI[3]->getSpeed()/3) + "km/h" , font, white));
text.push_back(new Button({100, 350, 100, 45},"Car5 Speed: " + std::to_string(allCarsAI[4]->getSpeed()/3) + "km/h" , font, white));
text.push_back(new Button({100, 300, 100, 45},"Car6 Speed: " + std::to_string(allCarsAI[5]->getSpeed()/3) + "km/h" , font, white));
text.push_back(new Button({100, 250, 100, 45},"Car7 Speed: " + std::to_string(allCarsAI[6]->getSpeed()/3) + "km/h" , font, white));
text.push_back(new Button({100, 200, 100, 45},"Car8 Speed: " + std::to_string(allCarsAI[7]->getSpeed()/3) + "km/h" , font, white));
text.push_back(new Button({100, 150, 100, 45},"Car9 Speed: " + std::to_string(allCarsAI[8]->getSpeed()/3) + "km/h" , font, white));

temps=50;

text.push_back(new Button({500, 500, 100, 45}, std::to_string(temps) , font, white));
	
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
	



void setup::update(SDL_Renderer* renderer)
{
  
    text[0]->setText("Vitesse: " + std::to_string((int)car->getSpeed()) + "km/h",game->getRenderer());
    
    for (size_t i = 0; i < allCarsAI.size(); ++i) {
    int speedInt = static_cast<int>(std::round(allCarsAI[i]->getSpeed()/3)); // arrondi
    text[i+1]->setText(std::string("Car Speed: ") + std::to_string(speedInt) + " km/h", renderer);
    
    text[10]->setText("temps: " + std::to_string((int)temps) ,renderer);
}

}




void setup::update(setup* g)
{
	
	pannel->setFillColor(vector3d(25,195,155));
	
	if(myButton[0]->getClick())
	{
		myButton[0]->setColor(vector3d(255,0,0));
		car->setKeyC(true);
        car->setKeyV(false);
	}
	else
	{
		myButton[0]->setColor(vector3d(50,50,50));
	}
	
	if(myButton[1]->getClick())
	{
		myButton[1]->setColor(vector3d(25,25,220));
		car->setKeyV(true);
        car->setKeyC(false);
	}
	else
	{
		myButton[1]->setColor(vector3d(150,150,150));
	}
	

 if(temps>0)
	temps--;


	carAABB->update(car->getPosition());

//	gestionEvents(g);
	followCamera(car->getPosition(),car->getForwardVector(),camera->getCamera()->getYaw(),camera->getCamera()->getPitch());
	
	car->setGravity();
	height->update();
	
	for (const auto& triangle : trackTriangles) {
    vector3d v1 = triangle[0];
    vector3d v2 = triangle[1];
    vector3d v3 = triangle[2];
    
 	if (collid->pointInTriangle(car->getPosition(), v1, v2, v3)) {
       car->setLocation(v1.y+150);
}
}



	for (const auto& triangle : trackTriangles) {
	    vector3d v1 = triangle[0];
	    vector3d v2 = triangle[1];
	    vector3d v3 = triangle[2];
	
		for(int i=0;i<allCarsAI.size();i++)
		{
	 	if (collid->pointInTriangle(allCarsAI[i]->getPosition(), v1, v2, v3)) {
	       allCarsAI[i]->setLocation(v1.y+200);
	    }
	}
	}
	
	for (int i = 0; i < allCarsAI.size(); i++) 
	{
			allCarsAI[i]->update();
			allCarsAI[i]->setGravity();
	}

	

if(temps<=0)
{
	
	Uint32 currentTime = SDL_GetTicks();

	if (currentTime - lastSpeedChangeTime >= speedChangeInterval)
	{
	    // On change la vitesse de toutes les voitures AI
	    for (auto& car : allCarsAI)
	    {
	        float newSpeed = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (1000 - 100) + 100;
	        car->setSpeed(newSpeed);
	        std::cout << "New speed for car: " << newSpeed << std::endl;
	    }
	
	    lastSpeedChangeTime = currentTime; // reset du timer
	}	

	car->update(vehicule);
	
	if(car->getSpeed()<0.0f && car->getSpeed()>-200)
		{
		
		 car->setUp(false);
		 car->setDown(false);

		 
		}
	
	if( car->getSpeed()<=0.5f && car->getKeyC())
	{
		   	  
		   	   car->setControlActif(true);
		   	   car->setControlActif2(false);
		   }
		   
			if( car->getSpeed()<=0.5f && car->getKeyV())
	{
		   	  
		   	   car->setControlActif2(true);
		   	   car->setControlActif(false);
		   	  
		   }
		  
	
	
		   
	if(car->getUp() && car->getControlActif() )
	{
	
            car->controlUp();
	}
	
	 if(car->getDown() && car->getControlActif2())
	{
			
            car->controlUp();
	}
	
	 if(car->getRight() )
	{
			car->controlRight();
	}
	
	 if(car->getLeft())
	{
			car->controlLeft();
	}
	
		if(car->getUp()==false)
	{
				car->passiveControlUp_Down();
	}
	
	if(car->getDown()==false)
	{
			car->passiveControlUp_Down();
	}
	

for (const auto& triangle : borderTriangles)
{
	vector3d v1 = triangle[0];
    vector3d v2 = triangle[1];
    vector3d v3 = triangle[2];
    
	if (collid->aabbIntersectsTriangle(*carAABB, v1, v2, v3)) {
		  
		 
	car->setSpeed(-55.0f);
		

    //carVelocity = carVelocity * -0.5f; // Appliquer un rebond simple
}

	if (collid->aabbIntersectsTriangle(*carAABB, v1, v2, v3)==false) {
		
	if( car->getControlActif())
	{
		if(car->getUp())
	{
		car->setDir(1);
	}
}

if( car->getControlActif2())
	{
		if(car->getDown())
	{
			car->setDir(-1);
	}
	}
	}

}
}



//	dec->update();

}




void setup::followCamera(vector3d carPosition, vector3d carForward, float yawInput, float pitchInput)
{

 
     float distanceBehind = 4500.0f;  // Distance caméra à la voiture
    float heightOffset = 2500.0f;    // Hauteur de la caméra

    // Position centrale : juste derrière la voiture
    vector3d basePos = carPosition - carForward.normalize() * distanceBehind;

    // Créer une matrice de rotation avec les inputs utilisateur pour orbiter
    Matrix4x4 rotationMatrix;
    rotationMatrix.setRotation(pitchInput, yawInput, 0.0f);

    // Calculer l'offset depuis la position de base vers la position orbitée
    vector3d offset = basePos - carPosition; // vecteur de la voiture vers la caméra sans rotation

    // Appliquer la rotation orbitale
    vector3d rotatedOffset = rotationMatrix.transformVector(offset);

    // Position finale de la caméra (voiture + offset orbitée)
    vector3d cameraPos = carPosition + rotatedOffset;
	cameraPos.y = heightOffset;  // ? on fixe la hauteur ici
    // Regarder toujours la voiture
    vector3d targetPos = carPosition;

	
    camera->getCamera()->setPosition(vector3d(-cameraPos.x,-cameraPos.y,-cameraPos.z));
    camera->getCamera()->setTarget(vector3d(-targetPos.x,-targetPos.y,-targetPos.z));

    
    std::cout<<"cam: "<<camera->getCamera()->getPosition()<<std::endl;

}

void setup::handleMouseMovement(float mouseDeltaX, float mouseDeltaY) {
    float sensitivity = 0.002f; // Sensibilité de la souris
    camera->getCamera()->updateAngles(-mouseDeltaY * sensitivity, -mouseDeltaX * sensitivity);
}



void setup::processInput(SDL_Event event)
{
	for(int i=0;i<myButton.size();i++)
	  myButton[i]->handleEvent(event);
	

	  if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_LEFT) 
                {
                	  car->setRight(true);
				}
				 //game->setAngleView(-0.1f);
                if (event.key.keysym.sym == SDLK_RIGHT)
				{
				     	car->setLeft(true);
				 } 
                //game->setAngleView(0.1f);
                if (event.key.keysym.sym == SDLK_UP) 
                {
                	car->setUp(true);
                		//update(0.5f);
                	
                		//	car->moveForward(700.5f);
                	}
                if (event.key.keysym.sym == SDLK_DOWN) 
                {
                		//update(-0.5f);
                			car->setDown(true);
                	}
                	
            
            
                	  if (event.key.keysym.sym == SDLK_w) 
                {
                
     

                	car->setUp(true);
                	
                }
                	  if (event.key.keysym.sym == SDLK_s) 
                {
                	
                
                	car->setDown(true);
                }
                
                	  if (event.key.keysym.sym == SDLK_a) 
                {
            
            	car->setRight(true);
            }
              	  if (event.key.keysym.sym == SDLK_d) 
                {
         
            car->setLeft(true);
            }
            
                    if (event.key.keysym.sym == SDLK_SPACE) 
                {
                		//update(-0.5f);
                		car->controlFrein();
                	}
                	
                	
                
                
            }
            	 if (event.type == SDL_KEYUP) {
            	 	
            	 	  if (event.key.keysym.sym == SDLK_LEFT) 
                {
                	  car->setRight(false);
				}
				 //game->setAngleView(-0.1f);
                if (event.key.keysym.sym == SDLK_RIGHT)
				{
				     	car->setLeft(false);
				 } 
                //game->setAngleView(0.1f);
                if (event.key.keysym.sym == SDLK_UP) 
                {
                	car->setUp(false);
                		//update(0.5f);
                	
                		//	car->moveForward(700.5f);
                	}
                	if (event.key.keysym.sym == SDLK_DOWN) 
                {
                		//update(-0.5f);
                			car->setDown(false);
                	}
                	
                	
            	  	if (event.key.keysym.sym == SDLK_w) 
                {
                	car->setUp(false);
                
                }
                	  if (event.key.keysym.sym == SDLK_s) 
                {
                	car->setDown(false);
                
                }
                	  if (event.key.keysym.sym == SDLK_a) 
                {
                	car->setRight(false);
            	
            }
              	  if (event.key.keysym.sym == SDLK_d) 
                {
                	car->setLeft(false);
            	
        }
    }
            
 
   if(event.type == SDL_MOUSEWHEEL)
   {
            if (event.wheel.y > 0) {
                camera->getCamera()->moveForward(0.02f); // Molette vers le haut -> avancer
            } else if (event.wheel.y < 0) {
                camera->getCamera()->moveForward(-0.02f); // Molette vers le bas -> reculer
            }
        }
            
            
       if (event.type == SDL_MOUSEBUTTONDOWN)
       {
	   
            if (event.button.button == SDL_BUTTON_LEFT) { // Clique gauche
                mouseDown = true;
                lastX = event.button.x;
                lastY = event.button.y;
            }
           
    }
            
        if(event.type == SDL_MOUSEBUTTONUP)
        {
		
            if (event.button.button == SDL_BUTTON_LEFT) { // Relâchement
                mouseDown = false;
            }
            
        }
            
       if (event.type == SDL_MOUSEMOTION) {
       	 if (mouseDown) {
       	 	int mouseDeltaX = event.motion.x-lastX;  // Déplacement horizontal
            int mouseDeltaY = event.motion.y-lastY;  // Déplacement vertical
       	 	handleMouseMovement( mouseDeltaX, mouseDeltaY) ;
            // Mouvement de la souris
         /*   int dx = event.motion.x-lastX;  // Déplacement horizontal
            int dy = event.motion.y-lastY;  // Déplacement vertical

            // Rotation de la caméra basée sur le mouvement de la souris
            float sensitivity = 0.15f;  // Sensibilité de la souris
            camera->rotateYaw(dx * sensitivity);
            camera->rotatePitch(-dy * sensitivity);
            */
               lastX = event.motion.x;
                lastY = event.motion.y;
            }
  
        }
    
}

void setup::drawCarPlayer(std::vector<Triangle>& allTriangles,SDL_Renderer* renderer, int screenWidth, int screenHeight) {
    

 /*	std::vector<Triangle> playerTriangles;
    std::vector<Triangle> AIPlayerTriangles;
    std::vector<Triangle> decorTriangles;
    std::vector<Triangle> arbresTriangles;*/
   

	car->draw( allTriangles, renderer,screenWidth, screenHeight,vehicule,camera);
	
}

void setup::drawCarPlayerAI(std::vector<Triangle>& allTriangles,SDL_Renderer* renderer, int screenWidth, int screenHeight,vehiculesetup& carSimple)
{
for (int i = 0; i < allCarsAI.size(); i++) 
{
	if (allCarsAI[i]->isInViewFrustum(carSimple)) {
 allCarsAI[i]->applyMatrix();
 allCarsAI[i]->draw( allTriangles, renderer,screenWidth, screenHeight,camera);
}
}
}

void setup::draw(std::vector<Triangle>& allTriangles,SDL_Renderer* renderer, int screenWidth, int screenHeight,vehiculesetup& carSimple)
{

for (auto* d : cubes) {
		if (d->isInViewFrustum(carSimple)) {
    d->draw(allTriangles, renderer, screenWidth, screenHeight, camera);
}
}

for (auto* d : arbres) {
		if (d->isInViewFrustum(carSimple)) {
    d->draw(allTriangles, renderer, screenWidth, screenHeight, camera);
}
}

for (auto* d : triangles) {
		if (d->isInViewFrustum(carSimple)) {
    d->draw(allTriangles, renderer, screenWidth, screenHeight, camera);
}
}

for (auto* d : animals) {
		if (d->isInViewFrustum(carSimple)) {
    d->draw(allTriangles, renderer, screenWidth, screenHeight, camera);
}
}


}



vector3d setup::computeNormal(const vector3d& v1, const vector3d& v2, const vector3d& v3) {
    vector3d edge1 = v2 - v1;
    vector3d edge2 = v3 - v1;
    return edge1.crossproduct(edge2).normalize();  // Produit vectoriel normalisé
}

bool setup::isTriangleVisible(const vector3d& normal, const vector3d& cameraPosition, const vector3d& trianglePoint) {
    vector3d viewDir = (cameraPosition - trianglePoint).normalize();
    float dotProduct = normal.dotproduct(viewDir);  // Produit scalaire entre la normale et la direction caméra

    return dotProduct > 0;  // Si positif, le triangle est orienté vers la caméra
}

void setup::draw()
{
	 std::vector<Triangle> playerTriangles;
	 std::vector<Triangle> playerAITriangles;
	 std::vector<Triangle> objectTriangles;

	 
    camera->projectionMode();

	pixel->getPixel()->clearBuffer();
    game->FillColor();
    SDL_Texture* texture;
    std::vector<Triangle> allTriangles;
    Uint32* framebuffer = new Uint32[WIDTH * HEIGHT]{};
    float* framebufferDepth = new float[WIDTH * HEIGHT];
    
     sky->drawBackground(framebuffer, WIDTH, HEIGHT);

	for(int i=0;i<vehicule.size();i++)
	    allTriangles.reserve(vehicule[i]->triangles.size());
	

	drawCarPlayer(playerTriangles, game->getRenderer(),  WIDTH, HEIGHT);
	
	drawCarPlayerAI(playerAITriangles, game->getRenderer(),  WIDTH, HEIGHT,*car);
	
	draw(objectTriangles, game->getRenderer(),  WIDTH, HEIGHT,*car);

    buffer->createTexture(game->getRenderer(),texture,WIDTH,HEIGHT);
 
	buffer->initializeZBuffer(framebufferDepth);
	
	    
	    for (auto& tri : trackTriangles) {
	    		vector3d normal = computeNormal(tri[0], tri[1], tri[2]);
	    		
      //	if (isTriangleVisible(normal, car.getPosition(), tri[0]))
	    race->drawTriangle(allTriangles, pixel->getPixel(), framebuffer, framebufferDepth, 800, 600, tri[0], tri[1], tri[2], *camera,true,false);
	
}
	
	// Affichage des bordures
for (const auto& tri : borderTriangles) {
		vector3d normal = computeNormal(tri[0], tri[1], tri[2]);
      //	if (isTriangleVisible(normal, car.getPosition(), tri[0]))
    race->drawTriangle(allTriangles, pixel->getPixel(), framebuffer, framebufferDepth, 800, 600, 
                 tri[0], tri[1], tri[2], *camera, false,true);

}
	
	for (auto& tri : terrainTriangles) {
			vector3d normal = computeNormal(tri[0], tri[1], tri[2]);
     //	if (isTriangleVisible(normal, car.getPosition(), tri[0])) {
	//    race->drawTriangle(allTriangles, pixel->getPixel(), framebuffer, framebufferDepth, 800, 600, tri[0], tri[1], tri[2], *camera,false,false);

}
	
	height->draw(allTriangles,game->getRenderer(), WIDTH, HEIGHT , camera, pixel, framebuffer, framebufferDepth);

    pixel->draw( playerTriangles,framebuffer,framebufferDepth, WIDTH, HEIGHT,  *camera->getCamera(),car->getPosition(),true);
    
    pixel->draw( playerAITriangles,framebuffer,framebufferDepth, WIDTH, HEIGHT,  *camera->getCamera(),car->getPosition(),false,15000,30000,40000);
    
    pixel->draw( objectTriangles,framebuffer,framebufferDepth, WIDTH, HEIGHT,  *camera->getCamera(),car->getPosition(),true);
    
	
	buffer->updateTexture(game->getRenderer(),texture,framebuffer,WIDTH);
	
	pannel->renderPanel(game->getRenderer());
    
	for(int i=0;i<text.size()-1;i++)	
     text[i]->renderText(game->getRenderer());
     
    if(temps>0)
    {
       text[10]->renderText(game->getRenderer());	
	}
     

    for(int i=0;i<myButton.size();i++)
    {
     myButton[i]->render(game->getRenderer());
    
    if (myButton[i]->getClick()) {
        printf("Bouton cliqué !\n");
        myButton[i]->setClick(false); // Réinitialiser
     }
 }
 
	buffer->destroyTexture(texture,framebuffer,framebufferDepth);
}

gameLoop * setup::getGame()
{
	return game;
}


	
