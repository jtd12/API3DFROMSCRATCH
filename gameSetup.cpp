#include"gameSetup.hpp"

setup::setup()
{

std::vector<vector3d> waypoints = {
    {5000, 1100, -1000}, {1000, 1000, -6700}, {-3000, 2000, -4000},{3000, 2000, 13000}, {-5000, 800, 14000}, {-27000,1000,12000},{-35000,1000,8000},{-41000,1000,7000},{-42000,1000,-2000},{8000, 1100, -12500}
    ,{18000, 1100, -9000},{30000, 1100, 1000},{31000, 1100, 4000},{33000, 1100, 15000},{34000, 1100, 15900},{32500, 1100, 22000},{22000, 1100, 45000},{20000, 1100, 50000},{18000, 1100, 55000},{5000, 1100, 65000},
 
};



car=new vehicule("data/voiture.obj",vector3d(6000,1800,35000));




sky=new skybox();
camera=new Camera(vector3d(0, -4000, -500), vector3d(0, 0, 4000), vector3d(0, -1, 0));  // Caméra
camera->setYaw(3.5f);
camera->setPitch(-0.5f);
pixel_=new pixel();
race=new raceTrack();
collid=new collisions();
carAABB=new AABB();



controlPoints = {
    {20000, 1100, 0}, {0, 1000, -5000}, {-4000, 2000, 14000}, {-40000, 800, 0}, {27000,1000,-500},{7000,1000,65000},{5000,1000,5000},{-4000,1000,-100},{200, 1100, -4000},    // fermé avec hauteur égale
        // répétition du premier
      // optionnel : pour continuité Catmull-Rom // Fermeture
};

race->initializeTrack(controlPoints, 70, 3400.0f); // Générer la piste
trackTriangles = race->generateTrackMesh(race->trackEdges,10.0f);
borderTriangles = race->generateTrackBorders(race->trackEdgesElevated);
terrainTriangles = race->generateTerrain(150000, 150000, 100);

if (TTF_Init() == -1) {
    std::cerr << "Erreur SDL_ttf : " << TTF_GetError() << std::endl;
	return;
}

TTF_Font* font = TTF_OpenFont("data/font/arial.TTF", 34);

if (!font) {
    std::cout << "Impossible de charger la police : " << TTF_GetError() << std::endl;
}

SDL_Color white = {255, 255, 255};

 myButton.push_back( new Button({30, 700, 100, 45}, "avancer", font, white));
 myButton.push_back( new Button({180, 700, 100, 45}, "reculer", font, white));


pannel=new Button({0, 700, 800, 100});

text=new Button({400, 700, 100, 45}, "Vitesse: " + std::to_string(car->getSpeed()) + "km/h", font, white);

 std::vector<std::string> filenames = {
        "data/voiture0.obj",
        "data/voiture1.obj",
        "data/voiture2.obj",
        "data/voiture3.obj",
      
    };

srand((unsigned)time(0)); // initialise la graine une seule fois

for(int i=0;i<9;i++)
{
	std::string filename = filenames[i % filenames.size()];  // cycle des modèles
    mesh* sharedMesh = ModelManager::getModel(filename);
	vector3d pos((i * 800) + 100, 0, 9000);  // position espacée
        vehiculeAI* car = new vehiculeAI(sharedMesh, waypoints, pos);
        int randomSpeed = 1500 + rand() % 1001;  
        car->setSpeed(randomSpeed);
        carAI.push_back(car);
}


int numCubes = 100;  // nombre de cubes
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

    float y = race->getTerrainHeight(x, z);
    vector3d pos(x, y + 1000, z);

    float randAngle = static_cast<float>(rand() % 360);
    float scaleX = 50.0f + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 50.0f;
    float scaleY = 50.0f + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 150.0f;
    float scaleZ = 50.0f + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 70.0f;

    decor* newCube = new decor(pos, vector3d(0, randAngle, 0), "data/decor.obj");
    newCube->scale = {scaleX, scaleY, scaleZ};
    cubes.push_back(newCube);

    cubesCreated++;
}



}




setup::~setup()
{
delete car;
for(int i=0;i<cubes.size();i++)
 delete cubes[i];
delete camera;
delete pixel_;
delete race;

for(int i=0;i<myButton.size();i++)
 delete myButton[i];
 
delete pannel;

for(int i=0;i<carAI.size();i++)
{
	delete carAI[i];
}
}

void setup::update(float t)
{
//	car->moveForward(t);
}

void setup::update(SDL_Renderer* renderer)
{
   text->setText("Vitesse: " + std::to_string((int)car->getSpeed()) + "km/h",renderer);	
}

void setup::update()
{

	pannel->setFillColor(vector3d(25,195,155));

	followCamera(car->getPosition(),car->getForwardVector(),camera->getYaw(),camera->getPitch());
	
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
	

	car->update();
	//std::cout << "Position voiture : " << car->getPosition().x << ", " << car->getPosition().y << ", " << car->getPosition().z << std::endl;
	for(int i=0;i<carAI.size();i++)
	{
		carAI[i]->update();
	}
	
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
	

	
	
	carAABB->update(car->getPosition());
	
	 //removeCloseDecor( *camera, 10000);
 	// restoreDecorFarFrom( *camera, 20000);
 	 
for (const auto& triangle : trackTriangles) {
    vector3d v1 = triangle[0];
    vector3d v2 = triangle[1];
    vector3d v3 = triangle[2];
    
 	if (collid->pointInTriangle(car->getPosition(), v1, v2, v3)) {
       car->setLocation(v1.y+200);
}
}

for (const auto& triangle : trackTriangles) {
    vector3d v1 = triangle[0];
    vector3d v2 = triangle[1];
    vector3d v3 = triangle[2];

	for(int i=0;i<carAI.size();i++)
	{
 	if (collid->pointInTriangle(carAI[i]->getPosition(), v1, v2, v3)) {
       carAI[i]->setLocation(v1.y+200);
    }
}
}


for (const auto& triangle : borderTriangles)
{
	vector3d v1 = triangle[0];
    vector3d v2 = triangle[1];
    vector3d v3 = triangle[2];
    
	if (collid->aabbIntersectsTriangle(*carAABB, v1, v2, v3)) {
		  
		 
	car->setSpeed(-15.0f);
		

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

	
	

	
    //carVelocity = carVelocity * -0.5f; // Appliquer un rebond simple
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

	
    camera->setPosition(vector3d(-cameraPos.x,-cameraPos.y,-cameraPos.z));
    camera->setTarget(vector3d(-targetPos.x,-targetPos.y,-targetPos.z));

    
    std::cout<<"cam: "<<camera->getPosition()<<std::endl;

}



void setup::handleMouseMovement(float mouseDeltaX, float mouseDeltaY) {
    float sensitivity = 0.002f; // Sensibilité de la souris
    camera->updateAngles(-mouseDeltaY * sensitivity, -mouseDeltaX * sensitivity);
}


void setup:: processInput( SDL_Event event) {
	
	
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
                camera->moveForward(0.02f); // Molette vers le haut -> avancer
            } else if (event.wheel.y < 0) {
                camera->moveForward(-0.02f); // Molette vers le bas -> reculer
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

void setup::drawCar(std::vector<Triangle>& allTriangles,SDL_Renderer* renderer, int screenWidth, int screenHeight, const Camera& camera,vehicule& car)
{
	car.applyMatrix();
    car.draw(renderer, screenWidth, screenHeight, camera, allTriangles);
}

void setup::drawCar(std::vector<Triangle>& allTriangles,SDL_Renderer* renderer, int screenWidth, int screenHeight, const Camera& camera,std::vector<vehiculeAI*>& car)
{
	for(int i=0;i<car.size();i++)
	{
			if (car[i]->isInViewFrustum(camera)) {
		car[i]->applyMatrix();
	    car[i]->draw(renderer, screenWidth, screenHeight, camera, allTriangles);
		}
	}
}

void setup::drawDecor(std::vector<Triangle>& allTriangles,SDL_Renderer* renderer, int screenWidth, int screenHeight, const Camera& camera, std::vector<decor*>& sceneDecor)
{

 for (auto& tri : allTriangles) {
}
	for(int i=0;i<sceneDecor.size();i++)
	{
if (sceneDecor[i]->isInViewFrustum(camera)) {
	sceneDecor[i]->applyMatrix();
    sceneDecor[i]->draw(renderer, screenWidth, screenHeight, camera, allTriangles);
	}
	}
}

void setup::drawPixels(std::vector<Triangle>& allTriangles,Uint32* framebuffer,float* framebufferDepth, int screenWidth, int screenHeight, const Camera& camera,bool isPlayer)
{
	int triIndex = 0;
	
	   for (auto& tri : allTriangles) {
        tri.avgDepth = (tri.v1.z + tri.v2.z + tri.v3.z) / 3.0f;
        
         
    }

    // **Tri rapide des triangles (éviter `std::stable_sort` si possible)**
    std::sort(allTriangles.begin(), allTriangles.end(), 
              [](const Triangle& t1, const Triangle& t2) {
                  return t1.avgDepth > t2.avgDepth;
              });
              
    
     for (auto& tri : allTriangles) {
     	
     	
      
float distance = (tri.v1 - car->getPosition()).length();

 if (distance < 10000) 
    {
        if (!isPlayer) {
            // Afficher 1 triangle sur 20 pour AI
            if (triIndex % 1 != 0) {
                triIndex++;
                continue;
            }
        }
    }
    
    if (distance > 10000 && distance < 40000) 
    {
        if (!isPlayer) {
            // Afficher 1 triangle sur 20 pour AI
            if (triIndex % 5 != 0) {
                triIndex++;
                continue;
            }
        }
    }
    
       if (distance > 40000 && distance < 80000) 
    {
        if (!isPlayer) {
            // Afficher 1 triangle sur 20 pour AI
            if (triIndex % 100 != 0) {
                triIndex++;
                continue;
            }
        }
    }
    
        if (distance > 80000) 
    {
        if (!isPlayer) {
            // Afficher 1 triangle sur 20 pour AI
            if (triIndex % 500 != 0) {
                triIndex++;
                continue;
            }
        }
    }
        
        Point2D p1 = pixel_->project(tri.v1, screenWidth, screenHeight);
        Point2D p2 = pixel_->project(tri.v2, screenWidth, screenHeight);
        Point2D p3 = pixel_->project(tri.v3, screenWidth, screenHeight);

        // Vérification des limites écran
        if (p1.x < 0 || p1.x >= screenWidth || p1.y < 0 || p1.y >= screenHeight ||
            p2.x < 0 || p2.x >= screenWidth || p2.y < 0 || p2.y >= screenHeight ||
            p3.x < 0 || p3.x >= screenWidth || p3.y < 0 || p3.y >= screenHeight) {
            triIndex++;
            continue;
        }

        vector3d normal = (tri.v3 - tri.v1).crossproduct(tri.v2 - tri.v1).normalize();
        vector3d lightDir = vector3d(0.0f, -1.0f, 1000.0f).normalize();
        float intensity = std::max(0.0f, normal.dotproduct(lightDir));

        Uint8 r = std::min(255, static_cast<int>(tri.material.diffuseColor.x * intensity * 255));
        Uint8 g = std::min(255, static_cast<int>(tri.material.diffuseColor.y * intensity * 255));
        Uint8 b = std::min(255, static_cast<int>(tri.material.diffuseColor.z * intensity * 255));
        
        
    
        // **Remplissage du triangle dans le framebuffer**
        pixel_->fillTriangle(framebuffer, framebufferDepth,
                                        { p1.x, screenHeight - p1.y }, 
                                        { p2.x, screenHeight - p2.y }, 
                                        { p3.x, screenHeight - p3.y },
                                        screenWidth, screenHeight, r, g, b, 
                                        tri.v1.z, tri.v2.z, tri.v3.z);
                                        
          triIndex++;
    

}
}


void setup::drawScene(SDL_Renderer* renderer, int screenWidth, int screenHeight, const Camera& camera, 
               vehicule& car, std::vector<vehiculeAI*>& carAI, std::vector<decor*>& sceneDecor) {
    
    
     
    pixel_->clearBuffer();
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
 std::vector<Triangle> playerTriangles;
    std::vector<Triangle> AIPlayerTriangles;
    std::vector<Triangle> decorTriangles;
     std::vector<Triangle> allTriangles;
     
for(int i=0;i<sceneDecor.size();i++)
    allTriangles.reserve(car.getTriangles().size() + sceneDecor[i]->triangles.size());
	

	drawCar(playerTriangles,renderer, screenWidth, screenHeight,  camera,car);
	
   drawCar(AIPlayerTriangles,renderer, screenWidth, screenHeight,  camera,carAI);
	
	drawDecor(decorTriangles, renderer,  screenWidth, screenHeight,  camera,  sceneDecor);

    // **Création d'une texture pour dessiner le framebuffer**
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, 
                                             SDL_TEXTUREACCESS_STREAMING, screenWidth, screenHeight);
    
    Uint32* framebuffer = new Uint32[screenWidth * screenHeight]{};
    float* framebufferDepth = new float[screenWidth * screenHeight];

    // Initialiser le Z-buffer à une grande valeur
    std::fill(framebufferDepth, framebufferDepth + screenWidth * screenHeight, std::numeric_limits<float>::infinity());
    sky->drawBackground(framebuffer, screenWidth, screenHeight);
    
      
	  
/*	  for (const auto& point : controlPoints) {
        // Projeter les points 3D en 2D (pour l'affichage à l'écran)
        Point2D projectedPoint = camera.project(point);

        // Vérifier si le point est dans les limites de l'écran
        if (projectedPoint.x >= 0 && projectedPoint.x < screenWidth && 
            projectedPoint.y >= 0 && projectedPoint.y < screenHeight) {
            
            // Dessiner un petit cercle ou point pour marquer la position du point de contrôle
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Couleur rouge pour les points
            const int radius = 5;  // Rayon du cercle

            for (int w = 0; w < radius * 2; w++) {
                for (int h = 0; h < radius * 2; h++) {
                    int dx = radius - w; // Distance par rapport au centre
                    int dy = radius - h;
                    if ((dx * dx + dy * dy) <= (radius * radius)) {
                        SDL_RenderDrawPoint(renderer, projectedPoint.x + dx, projectedPoint.y + dy);
                    }
                }
            }
        }
    }
    */
              
	    for (auto& tri : trackTriangles) {
	    		vector3d normal = computeNormal(tri[0], tri[1], tri[2]);
	    		
      //	if (isTriangleVisible(normal, car.getPosition(), tri[0]))
	    race->drawTriangle(allTriangles, pixel_, framebuffer, framebufferDepth, screenWidth, screenHeight, tri[0], tri[1], tri[2], camera,true,false);
	
}
	
	// Affichage des bordures
for (const auto& tri : borderTriangles) {
		vector3d normal = computeNormal(tri[0], tri[1], tri[2]);
      //	if (isTriangleVisible(normal, car.getPosition(), tri[0]))
    race->drawTriangle(allTriangles, pixel_, framebuffer, framebufferDepth, screenWidth, screenHeight, 
                 tri[0], tri[1], tri[2], camera, false,true);

}
	
	for (auto& tri : terrainTriangles) {
			vector3d normal = computeNormal(tri[0], tri[1], tri[2]);
     //	if (isTriangleVisible(normal, car.getPosition(), tri[0])) {
	    race->drawTriangle(allTriangles, pixel_, framebuffer, framebufferDepth, screenWidth, screenHeight, tri[0], tri[1], tri[2], camera,false,false);

}
	

    // **Affichage des triangles triés**
   
    drawPixels( playerTriangles,framebuffer,framebufferDepth, screenWidth, screenHeight,  camera,true);
    drawPixels( AIPlayerTriangles,framebuffer,framebufferDepth, screenWidth, screenHeight,  camera,false);
	drawPixels( decorTriangles,framebuffer,framebufferDepth, screenWidth, screenHeight,  camera,true);
//	drawPixels( allTriangles,framebuffer,framebufferDepth, screenWidth, screenHeight,  camera,true);
	
	SDL_SetHint(SDL_HINT_RENDER_VSYNC, "0");
    // **Mise à jour de la texture**
    SDL_UpdateTexture(texture, NULL, framebuffer, screenWidth * sizeof(Uint32));

    // **Affichage final**
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    
    pannel->renderPanel(renderer);
    text->renderText(renderer);
    
    for(int i=0;i<myButton.size();i++)
    {
     myButton[i]->render(renderer);
    
    if (myButton[i]->getClick()) {
        printf("Bouton cliqué !\n");
        myButton[i]->setClick(false); // Réinitialiser
     }

   }
        
    SDL_RenderPresent(renderer);

    // Nettoyage
    delete[] framebuffer;
    delete[] framebufferDepth;
    SDL_DestroyTexture(texture);
}

void setup::draw(SDL_Renderer* pRenderer)
{


 drawScene(pRenderer, 800, 600,  *camera, 
              *car,carAI, cubes);

}
