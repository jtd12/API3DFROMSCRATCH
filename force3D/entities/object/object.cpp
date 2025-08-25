#include"object.h"

object::object(vector3d pos,vector3d rot,vector3d scaling, const std::string& path,bool wireframe)
{
	
		obj=new objloader();
		
		obj->load(path,triangles);
		position=pos;
		rotation=rot;
		scale=scaling;
	scaleMatrix.setScaling(scale.x, scale.y, scale.z);
    translationMatrix.setTranslation(position.x, position.y, position.z);
    rotationMatrixX.setRotationX(rotation.x);
    rotationMatrixY.setRotationY(rotation.y);
    rotationMatrixZ.setRotationZ(rotation.z);
	    
	   for (auto& tri : triangles) {
    tri.wireframe = wireframe; // Propagation du mode wireframe à chaque triangle	
}
}

object::object(vector3d pos,vector3d rot,vector3d scaling, const std::string& path,bool wireframe,int time)
{

		obj=new objloader();
		obj->loadAnimation(animationFrames,path,time);
		position=pos;
		rotation=rot;
		scale=scaling;
	scaleMatrix.setScaling(scale.x, scale.y, scale.z);
    translationMatrix.setTranslation(position.x, position.y, position.z);
    rotationMatrixX.setRotationX(rotation.x);
    rotationMatrixY.setRotationY(rotation.y);
    rotationMatrixZ.setRotationZ(rotation.z);
	    
	   for (auto& tri : triangles) {
    tri.wireframe = wireframe; // Propagation du mode wireframe à chaque triangle	
}
}

object::object(vector3d pos,vector3d rot,vector3d scaling,object* baseModel)
{
	
     position = pos;
    rotation = rot;
    scale = scaling;

    scaleMatrix.setScaling(scale.x, scale.y, scale.z);
    translationMatrix.setTranslation(position.x, position.y, position.z);
    rotationMatrixX.setRotationX(rotation.x);
    rotationMatrixY.setRotationY(rotation.y);
    rotationMatrixZ.setRotationZ(rotation.z);

    // Données partagées
    obj = baseModel->obj; // On ne recharge pas l'obj — on copie juste le pointeur
    triangles = baseModel->triangles; // Copie du mesh (par valeur ici, possible si immutable)
    
    boundingBox = nullptr; // ou copie si nécessaire (à faire si collision)
    
 
}

object::~object()
{

	  delete obj;
}

AABB* object::getBoundingBox()
{
	return boundingBox;
}

void object::update()
{
	
}



void object::updateAnimation(float deltaTime,float speed, int startFrame, int endFrame) {
    timer += deltaTime;
    if (timer >= frameTime) {
        timer = 0.0f;

        currentFrame += speed;

        if (currentFrame > endFrame)
            currentFrame = startFrame;
        if (currentFrame < startFrame)
            currentFrame = endFrame;
    }
}

vector3d object::getSize() const {
    return {
        aabbMax.x - aabbMin.x,
        aabbMax.y - aabbMin.y,
        aabbMax.z - aabbMin.z
    };
}

bool object::checkCollisionWithCamera(vector3d camPos, float cameraRadius) const {
		
        Matrix4x4 modelMatrix = translationMatrix *
                                rotationMatrixX *
                                rotationMatrixY *
                                rotationMatrixZ *
                                scaleMatrix;

        for (const auto& tri : triangles) {
            for (const auto& v : { tri.v1, tri.v2, tri.v3 }) {
                vector3d worldV = modelMatrix.apply(v);

                
            float dx = worldV.x - camPos.x;
            float dy = worldV.y - camPos.y;
            float dz = worldV.z - camPos.z;
            float distSq = dx*dx + dy*dy + dz*dz;

                if (distSq < cameraRadius * cameraRadius) {
                	std::cout << "Collision détectée avec vertex: " << worldV << std::endl;
                    return true; // collision trouvée
                }
            }
        }

        return false; // pas de collision
    }

void object::computeAABB() {
	
	aabbMin = { FLT_MAX, FLT_MAX, FLT_MAX };
    aabbMax = { -FLT_MAX, -FLT_MAX, -FLT_MAX };

    Matrix4x4 modelMatrix = translationMatrix * rotationMatrixX * rotationMatrixY * rotationMatrixZ * scaleMatrix;

    for (const auto& tri : triangles) {
        for (const auto& v : { tri.v1, tri.v2, tri.v3 }) {
            vector3d worldV = modelMatrix.apply(v);

            aabbMin.x = std::min(aabbMin.x, worldV.x);
            aabbMin.y = std::min(aabbMin.y, worldV.y);
            aabbMin.z = std::min(aabbMin.z, worldV.z);

            aabbMax.x = std::max(aabbMax.x, worldV.x);
            aabbMax.y = std::max(aabbMax.y, worldV.y);
            aabbMax.z = std::max(aabbMax.z, worldV.z);
        }
    }
}


vector3d object::getCenter() const {
    return {
        (aabbMin.x + aabbMax.x) / 2.0f,
        (aabbMin.y + aabbMax.y) / 2.0f,
        (aabbMin.z + aabbMax.z) / 2.0f
    };
}


void object::drawAnimatedObject(SDL_Renderer* renderer, int screenWidth, int screenHeight, const Camera& camera, std::vector<Triangle>& allTriangles) {
    Matrix4x4 viewProjectionMatrix = camera.getProjectionMatrix() * camera.getViewMatrix(camera, 1);
    Matrix4x4 modelTransform = translationMatrix * rotationMatrixX * rotationMatrixY * rotationMatrixZ * scaleMatrix;
    Matrix4x4 finalMatrix = viewProjectionMatrix * modelTransform;

    for (auto& tri : animationFrames[currentFrame]) {
        Triangle transformedTri = tri;
        transformedTri.v1 = finalMatrix.apply(tri.v1);
        transformedTri.v2 = finalMatrix.apply(tri.v2);
        transformedTri.v3 = finalMatrix.apply(tri.v3);
        transformedTri.avgDepth = (transformedTri.v1.z + transformedTri.v2.z + transformedTri.v3.z) / 3.0f;

        allTriangles.push_back(transformedTri);
    }
}

void object::draw(SDL_Renderer* renderer, int screenWidth, int screenHeight, const Camera& camera, std::vector<Triangle>& allTriangles) {

    Matrix4x4 viewProjectionMatrix = camera.getProjectionMatrix() * camera.getViewMatrix(camera,1);
	Matrix4x4 modelTransform =   translationMatrix *rotationMatrixX *rotationMatrixY * rotationMatrixZ * scaleMatrix;
	Matrix4x4 finalMatrix = viewProjectionMatrix * modelTransform;

    for (auto& tri : triangles) {
        Triangle transformedTri = tri;
        transformedTri.v1 = finalMatrix.apply(tri.v1);
        transformedTri.v2 = finalMatrix.apply(tri.v2);
        transformedTri.v3 = finalMatrix.apply(tri.v3);
        transformedTri.avgDepth = (transformedTri.v1.z + transformedTri.v2.z + transformedTri.v3.z) / 3.0f;

        allTriangles.push_back(transformedTri);
    }
}


void object::applyMatrix(){
translationMatrix.setTranslation(position.x, position.y, position.z);
rotationMatrixX.setRotationX(rotation.x);
rotationMatrixY.setRotationY(rotation.y);
rotationMatrixZ.setRotationZ(rotation.z);
scaleMatrix.setScaling(scale.x,scale.y,scale.z);
//draw(pRenderer,800,600,camera); // Affichage du modèle
}


void object::setColor(const vector3d& color) {
    for (auto& tri : triangles) {
        tri.material.diffuseColor = color;
    }
}
    
    
 bool object::isCloseTo( vector3d other,float threshold,vector3d position) const{
         return (position - other).length() > threshold; // distance Euclidienne
    }
    

 vector3d object::getNormal() const {
        vector3d avgNormal(0, 0, 0);
        int count = 0;

        for (const auto& tri : triangles) {
            vector3d normal = (tri.v2 - tri.v1).crossproduct(tri.v3 - tri.v1).normalize();
            avgNormal = avgNormal + normal;
            count++;
        }

        return (count > 0) ? (avgNormal / count).normalize() : vector3d(0, 1, 0); // Valeur par défaut
    }


bool object::isInViewFrustum(const Camera& cam,vector3d position) const {
    vector3d camToObj = position - cam.getPosition();
    float dist = camToObj.length();
    if (dist > 10000.0f) return false; // trop loin
    return true;
}

void object::setCurrentFrame(int frame) {
    currentFrame = frame;
}

int object::getCurrentFrame() const {
    return currentFrame;
}

void object::setLocation(vector3d pos)
{
	position=pos;
}

vector3d object::getLocation()
{
	return position;
}

void object::setRotation(vector3d rot)
{
	rotation=rot;
}

Matrix4x4& object::getTranslationMatrix()
{
	return translationMatrix;
}

Matrix4x4& object::getRotationMatrixX()
{
	return rotationMatrixX;
}

Matrix4x4& object::getRotationMatrixY()
{
	return rotationMatrixY;
}    	

Matrix4x4& object::getRotationMatrixZ()
{
	return rotationMatrixZ;
}

Matrix4x4& object::getScaleMatrix()
{
	return scaleMatrix;
}




