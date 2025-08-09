#include"decor.h"

decor::decor(vector3d pos,vector3d rot,const std::string& path)
{
		obj=new objloader();
		
		obj->load(path,triangles);
		rotation=rot;
		scale=vector3d(5,5,5);
		position=pos;
		
		
}

decor::~decor()
{

	  delete obj;
}

AABB* decor::getBoundingBox()
{
	return boundingBox;
}

void decor::update()
{
	
}

void decor::draw(SDL_Renderer* renderer, int screenWidth, int screenHeight, const Camera& camera, std::vector<Triangle>& allTriangles) {

    Matrix4x4 viewProjectionMatrix = camera.getProjectionMatrix() * camera.getViewMatrix(camera,1);
	Matrix4x4 decorTransform =   rotationMatrixY * translationMatrix * scaleMatrix;
	Matrix4x4 finalMatrix = viewProjectionMatrix * decorTransform;

    for (auto& tri : triangles) {
        Triangle transformedTri = tri;
        transformedTri.v1 = finalMatrix.apply(tri.v1);
        transformedTri.v2 = finalMatrix.apply(tri.v2);
        transformedTri.v3 = finalMatrix.apply(tri.v3);
        transformedTri.avgDepth = (transformedTri.v1.z + transformedTri.v2.z + transformedTri.v3.z) / 3.0f;

        allTriangles.push_back(transformedTri);
    }
}


void decor::applyMatrix(){
translationMatrix.setTranslation(position.x, position.y, position.z);
rotationMatrixY.setRotationY(rotation.y);
scaleMatrix.setScaling(scale.x,scale.y,scale.z);
//draw(pRenderer,800,600,camera); // Affichage du modèle
}

 bool decor::isCloseTo( vector3d other,float threshold) const{
         return (position - other).length() > threshold; // distance Euclidienne
    }
    

 vector3d decor::getNormal() const {
        vector3d avgNormal(0, 0, 0);
        int count = 0;

        for (const auto& tri : triangles) {
            vector3d normal = (tri.v2 - tri.v1).crossproduct(tri.v3 - tri.v1).normalize();
            avgNormal = avgNormal + normal;
            count++;
        }

        return (count > 0) ? (avgNormal / count).normalize() : vector3d(0, 1, 0); // Valeur par défaut
    }


bool decor::isInViewFrustum(const Camera& cam) const {
    vector3d camToObj = position - cam.getPosition();
    float dist = camToObj.length();
    if (dist > 10000.0f) return false; // trop loin
    return true;
}




