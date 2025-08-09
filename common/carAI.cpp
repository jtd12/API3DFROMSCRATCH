#include"carAI.hpp"

template <typename T>
T clamp(T value, T minVal, T maxVal) {
    return (value < minVal) ? minVal : (value > maxVal) ? maxVal : value;
}


  vehiculeAI::vehiculeAI(const std::string&  filename,const std::vector<vector3d>& waypoints,vector3d pos)
    {
    	obj=new objloader();
		obj->load(filename,triangles);
		waypoints_=(waypoints);
		currentWaypointIndex=(0);
		scale=vector3d(50,50,50);
		speed=0;
		direction = vector3d(1, 0, 0);
	
		for(int i=0;i<4;i++)
		 wheel.push_back(new roue(vector3d(0,2000,5000)));
		 
		if (!waypoints.empty()) {
            position =  waypoints[0]+pos; // Départ au premier waypoint
            direction.normalise(waypoints[1] - waypoints[0]); // Direction initiale
            
        }
        
	}
	
		vehiculeAI::~vehiculeAI()
	{
		delete obj;
	}
	
	
	void vehiculeAI::setSpeed(float s)
	{
	speed=s;	
	}
	
	void vehiculeAI::update()
	{
	
	followWayPoints(0.5f);
		
	for(int i=0;i<2;i++)
	  wheel[i]->setRotationyegal(rot.y);
	  
	for(int i=2;i<4;i++)
	{
	  wheel[i]->setRotationyegal(rot.y-3.0f);
	}

	}
	
	void vehiculeAI::applyMatrix()
	{
	
	translationMatrix.setTranslation(position.x, position.y, position.z);

	rotationMatrixY.setRotationY(yaw);
	scaleMatrix.setScaling(scale.x,scale.y,scale.z);


        vector3d wheelOffsets[4] = {
        vector3d(150, -25, 160),  // Avant gauche
        vector3d(-250, -25, 160), // Avant droite
        vector3d(150, -25, -160), // Arrière gauche
        vector3d(-250, -25, -160) // Arrière droite
    };
		
	for (int i = 0; i < 4; i++) {
		vector3d rotatedOffset = rotationMatrixY.apply(wheelOffsets[i]); 
        // Appliquer la rotation de la voiture aux roues
        wheel[i]->setLocation( position + rotatedOffset);
        wheel[i]->applyMatrix();
   
    //obj->draw(pRenderer, 800, 600, camera);
  }	
	}
	
	
	
	void vehiculeAI::draw(SDL_Renderer* renderer, int screenWidth, int screenHeight, const Camera& camera, std::vector<Triangle>& allTriangles)
	{
	Matrix4x4 viewProjectionMatrix = camera.getProjectionMatrix() * camera.getViewMatrix(camera,1);
	Matrix4x4 carTransform = translationMatrix * rotationMatrixX * rotationMatrixY * rotationMatrixZ * scaleMatrix;
	Matrix4x4 finalMatrix = viewProjectionMatrix * carTransform;
	
	std::vector<vector3d> vertexNormals;  // Stockage des normales lissées
	int numVertices=724;
	obj->smoothNormals(triangles, vertexNormals,numVertices);

    for (auto& tri : triangles) {
        Triangle transformedTri = tri;
        transformedTri.v1 = finalMatrix.apply(tri.v1);
        transformedTri.v2 = finalMatrix.apply(tri.v2);
        transformedTri.v3 = finalMatrix.apply(tri.v3);
        
        transformedTri.n1 = vertexNormals[tri.index1]; // Normale lissée pour le sommet 1
        transformedTri.n2 = vertexNormals[tri.index2]; // Normale lissée pour le sommet 2
        transformedTri.n3 = vertexNormals[tri.index3]; // Normale lissée pour le sommet 3
        
        transformedTri.avgDepth = (transformedTri.v1.z + transformedTri.v2.z + transformedTri.v3.z) / 3.0f;

        allTriangles.push_back(transformedTri);
    }
    
     for (int i = 0; i < wheel.size(); i++) {
        wheel[i]->draw(renderer, screenWidth, screenHeight, camera, allTriangles);
    }
	}

    void vehiculeAI::followWayPoints(float deltaTime) {
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

	std::cout << "Direction: " << direction.x << ", " << direction.y << ", " << direction.z << std::endl;
    // Avancer
    //speed = 100.0f; // par exemple
    position += vector3d(direction.x,direction.y,direction.z) * speed * deltaTime;

    // Passer au waypoint suivant si on est proche
    float distToTarget = (target - position).length();
    if (distToTarget < 1500.0f) {
        currentWaypointIndex = (currentWaypointIndex + 1) % waypoints_.size();
    }
    }
    
    bool vehiculeAI::isInViewFrustum(const Camera& cam) const {
    vector3d camToObj = position - cam.getPosition();
    float dist = camToObj.length();
    if (dist > 28000.0f) return false; // trop loin
    return true;
}


    vector3d vehiculeAI::getPosition()
    {
    	return position;
	}
	
	void vehiculeAI::setLocation(float loc)
	{
		position.y=loc;
	}
