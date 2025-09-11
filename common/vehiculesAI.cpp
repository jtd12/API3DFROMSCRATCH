#include"vehiculesAI.hpp"



std::unordered_map<std::string, mesh*> ModelManager::models;

   mesh* ModelManager:: getModel(const std::string& filename) {
        auto it = models.find(filename);
        if (it != models.end())
            return it->second;  // déjà chargé

        mesh* newMesh = new mesh();
        objloader::load(filename, newMesh->triangles); // charge le fichier OBJ
        models[filename] = newMesh;
        return newMesh;
    }
    
vehiculesAISetup::vehiculesAISetup(mesh* sharedMesh, const std::vector<vector3d>& waypoints, int startIndex)
    {
 
        init(sharedMesh, waypoints,startIndex);
	}
	
	
vehiculesAISetup::~vehiculesAISetup()
    {
 
        
	}
	
	

  void vehiculesAISetup::init(mesh* sharedMesh, const std::vector<vector3d>& waypoints, int startIndex)
    {
    // Mesh partagé pour le véhicule
     carMesh = sharedMesh;
    triangles = carMesh->triangles;

    // Waypoints
    waypoints_ = waypoints;
	radius=400.0f; // ex: 2.0f
    // Position, rotation, scale
    position = waypoints[startIndex] + vector3d(0, -700, 0);;
    rotation = vector3d(0,0,0);
    scale = vector3d(100,100,100);
    speed = 0;
    direction = vector3d(1,0,0);

    // Crée l'objet voiture
    object* model = new object(position, rotation, scale, sharedMesh, false);
    vehicule_.push_back(model);
     for (int i = 0; i < 4; i++)
        wheel.push_back(new object(vector3d(0, 0, 0), vector3d(0, 0, 0), scale, "data/roues.obj", false));

   if (!waypoints.empty() && startIndex >= 0 && startIndex < waypoints.size()) {
        int nextIndex = (startIndex + 1) % waypoints.size();
        direction = (waypoints[nextIndex] - position).normalize();
        currentWaypointIndex = nextIndex;
    }
   
        
	}
	
	
float vehiculesAISetup::getRadius()
{
		return radius;
}
	
void vehiculesAISetup::setSpeed(float s)
	{
	speed=s;	
	}
	

float vehiculesAISetup::getSpeed()
{
	return speed;
}

vector3d vehiculesAISetup::getPosition()
    {
    	return position;
	}
	
void vehiculesAISetup::setLocation(vector3d loc)
{
	position=loc;
}

void vehiculesAISetup::setLocationInc(vector3d loc)
{
	position+=loc;
}

void vehiculesAISetup::setLocation(float locY)
{
	position.y=locY;
}

void vehiculesAISetup::setGravity()
	{
		position.y-=50.5f;

	}
	
void vehiculesAISetup::update()
	{
	
	
	followWayPoints(0.5f);
		
//	for(int i=0;i<2;i++)
	 // wheel[i]->setRotationyegal(rot.y);
	  
	for(int i=2;i<4;i++)
	{
	  //wheel[i]->setRotationyegal(rot.y-3.0f);
	}

	}
	
	vector3d vehiculesAISetup::getForwardVector() const {
    float yaw = rotation.y; // Angle de rotation autour de l'axe Y

    // Calcul du vecteur avant à partir de l'angle Yaw
    return vector3d(
        cos(yaw),  // Direction en X
        0.0f,      // Pas de changement en Y (reste au sol)
        sin(yaw)   // Direction en Z
    ).normalize(); // Normalisation pour éviter les problèmes d’échelle
}


	
	
void vehiculesAISetup::followWayPoints(float deltaTime) {
        if (waypoints_.empty()) return;

    vector3d target = waypoints_[currentWaypointIndex];
    vector3d toTarget = (target - position).normalize();

    // Rotation douce
    vector3d dir2D = vector3d(direction.x, 0, direction.z).normalize();
    vector3d target2D = vector3d(toTarget.x, 0, toTarget.z).normalize();

    float dot = dir2D.dotproduct(target2D);
    dot = clamp(dot, -1.0f, 1.0f);
    float angle = acos(dot);

    float cross = dir2D.x * target2D.z - dir2D.z * target2D.x;
    int turnDirection = (cross > 0) ? 1 : -1;

    float turnSpeed = 1.5f * deltaTime;
	float turnAngle = std::min(turnSpeed, angle);
	
	// On accumule l’angle total
	yaw += turnAngle * turnDirection;
	
	// Recalculer direction à partir de l’angle Yaw total
	Matrix4x4 rotYFull;
	rotYFull.setRotationY(yaw);
	direction = rotYFull.apply(vector3d(-1, 0, 0)).normalize();
	rotation.y = yaw;

	std::cout << "Direction: " << direction.x << ", " << direction.y << ", " << direction.z << std::endl;
    // Avancer
    //speed = 100.0f; // par exemple
    position += vector3d(direction.x,direction.y,direction.z) * speed * deltaTime;

    // Passer au waypoint suivant si on est proche
    float distToTarget = (target - position).length();
    if (distToTarget < 2500.0f) {
        currentWaypointIndex = (currentWaypointIndex + 1) % waypoints_.size();
    }
    }
    
    

void vehiculesAISetup::applyMatrix()
{
	if (vehicule_.empty()) return;

    // Boucle sur tous les objets du setup
    for (auto* car : vehicule_) {
	car->getRotationMatrixY().setRotationY(yaw);
	}

}

void vehiculesAISetup::draw(std::vector<Triangle>& allTriangles,SDL_Renderer* renderer, int screenWidth, int screenHeight,camerasetup* camera)
{
	if (vehicule_.empty()) return;

    // Boucle sur tous les objets du setup
    for (auto* car : vehicule_) {
        car->getTranslationMatrix().setTranslation(position.x, position.y, position.z);
        car->getRotationMatrixX().setRotationX(rotation.x);
        car->getRotationMatrixY().setRotationY(rotation.y);
        car->getRotationMatrixZ().setRotationZ(rotation.z);

        // Offsets roues
        vector3d wheelOffsets[4] = {
        vector3d(350, -25, 320),  // Avant gauche
        vector3d(-450, -25, 320), // Avant droite
        vector3d(350, -25, -320), // Arrière gauche
        vector3d(-450, -25, -320) // Arrière droite
        };

        Matrix4x4 rotXMat; rotXMat.setRotationX(rotation.x);
        Matrix4x4 rotYMat; rotYMat.setRotationY(rotation.y);
        Matrix4x4 rotZMat; rotZMat.setRotationZ(rotation.z);
        Matrix4x4 translationMat; translationMat.setTranslation(position.x, position.y, position.z);

        Matrix4x4 vehicleMatrix = translationMat * rotXMat * rotYMat * rotZMat;

        for (int i = 0; i < 4; i++) {
            vector3d worldPos = vehicleMatrix.apply(wheelOffsets[i]);

            wheel[i]->setLocation(worldPos);
            wheel[i]->setRotation(vector3d(0, rotation.y, 0));

            if (i == 2 || i == 3) {
                wheel[i]->setRotation(vector3d(0, rotation.y+3, 0));
            }

            wheel[i]->applyMatrix();
            wheel[i]->draw(renderer, screenWidth, screenHeight, *camera->getCamera(), allTriangles);
        }

        car->draw(renderer, screenWidth, screenHeight, *camera->getCamera(), allTriangles);
    }
	}
	
std::vector<object*>& vehiculesAISetup::getObjects() {
    return vehicule_;
}


bool vehiculesAISetup::isInViewFrustum(vehiculesetup& car) const {
    vector3d camToObj = position - car.getPosition();
    float dist = camToObj.length();
    if (dist > 15000.0f) return false; // trop loin
    return true;
}


