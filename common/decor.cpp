#include"decor.hpp"

decorSetup::decorSetup(vector3d pos,vector3d rot,vector3d s,const std::string& path)
{

	init(pos,rot,s,path);
}
void decorSetup::init(vector3d pos,vector3d rot,vector3d s,const std::string& path)
{
	
	position=pos;
	scale=s;
	rotation=rot;
	object* model = new object(position, rotation, scale, path,false);

    decor_.push_back(model);

		float r = static_cast<float>(rand() % 256) / 255.0f;
	    float g = static_cast<float>(rand() % 256) / 255.0f;
	    float b = static_cast<float>(rand() % 256) / 255.0f;
    	
		model->setColor(vector3d(r, g, b));  // si setColor est défini dans object

		
}

decorSetup::~decorSetup()
{

}

AABB* decorSetup::getBoundingBox()
{
	return boundingBox;
}

void decorSetup::update()
{
	
}

void decorSetup::draw(std::vector<Triangle>& allTriangles,SDL_Renderer* renderer, int screenWidth, int screenHeight,camerasetup* camera) {

  		for(int i=0;i<decor_.size();i++)
{
	decor_[i]->getTranslationMatrix().setTranslation(position.x,position.y,position.z);
	decor_[i]->getRotationMatrixX().setRotationX(rotation.x);
	decor_[i]->getRotationMatrixY().setRotationY(rotation.y);
	decor_[i]->getRotationMatrixZ().setRotationZ(rotation.z);

}
	
	for(int i=0;i<decor_.size();i++)
	{
//	vehicule[i]->rotationMatrixY.setRotationY(rot);

      decor_[i]->draw(renderer, screenWidth, screenHeight, *camera->getCamera(), allTriangles);
		
	}
	
}


void decorSetup::applyMatrix(){
translationMatrix.setTranslation(position.x, position.y, position.z);
rotationMatrixY.setRotationY(rotation.y);
scaleMatrix.setScaling(scale.x,scale.y,scale.z);
//draw(pRenderer,800,600,camera); // Affichage du modèle
}

 bool decorSetup::isCloseTo( vector3d other,float threshold) const{
         return (position - other).length() > threshold; // distance Euclidienne
    }
    

 vector3d decorSetup::getNormal() const {
        vector3d avgNormal(0, 0, 0);
        int count = 0;

        for (const auto& tri : triangles) {
            vector3d normal = (tri.v2 - tri.v1).crossproduct(tri.v3 - tri.v1).normalize();
            avgNormal = avgNormal + normal;
            count++;
        }

        return (count > 0) ? (avgNormal / count).normalize() : vector3d(0, 1, 0); // Valeur par défaut
    }


void decorSetup::setScale(vector3d s)
{
	scale=s;
}

bool decorSetup::isInViewFrustum(vehiculesetup& car) const {
    vector3d camToObj = position - car.getPosition();
    float dist = camToObj.length();
    if (dist > 50000.0f) return false; // trop loin
    return true;
}


