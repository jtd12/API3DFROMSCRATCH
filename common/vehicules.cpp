#include"vehicules.hpp"

vehiculesetup::vehiculesetup()
{
	
}

vehiculesetup::~vehiculesetup()
{
	for(int i=0;i<wheel.size();i++) 
	  delete wheel[i];
}


void vehiculesetup::init(std::vector<object*>& vehicule)
{
	
	object* model1 = new object(vector3d(0,0,0), vector3d(0,0,0), vector3d(15,15,15), "data/voiture.obj",false);
    object* model2 = new object(vector3d(0,0,0),vector3d(0,0,0), vector3d(15,15,15), "data/voiture2.obj",false);

    for (int i = 0; i < 6; ++i) {
        vector3d pos=vector3d(-5700-(i * 5800.0f), -3000.0f,5000-(i* 400.0f));
        vector3d rot(0.0f, i * 10.0f, 0.0f);

        object* baseModel = (i % 2 == 0) ? model1 : model2;

        // Crée une instance à partir du modèle partagé
        object* instance = new object(pos, rot, vector3d(500,500,500), baseModel);

        vehicule.push_back(instance);
        
    position.push_back(pos);
    rotation.push_back(rot);
	gravity=200.0f;
    // 4 roues par véhicule
    for (int j = 0; j < 4; ++j) {
        wheel.push_back(new object(vector3d(0, 0, 0), vector3d(0, 0, 0), vector3d(500, 500, 500), "data/roues.obj",false));
    }
}

}

void vehiculesetup::update(std::vector<object*>& vehicule)
{
		for(int i=0;i<vehicule.size();i++)
		{
			position[i].y-=gravity;
			
			if(position[i].y< -6500.0f)
			{
				position[i].y=-6500.0f;
			}
		}
}

void vehiculesetup:: draw(std::vector<Triangle>& allTriangles,SDL_Renderer* renderer, int screenWidth, int screenHeight,std::vector<object*>& vehicule,camerasetup* camera)
{
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
    vehicule[i]->draw(renderer, screenWidth, screenHeight, *camera->getCamera(), allTriangles);
		
	}
	 for (int i = 0; i < wheel.size(); i++) {
        wheel[i]->draw(renderer, screenWidth, screenHeight, *camera->getCamera(), allTriangles);
    }
}
