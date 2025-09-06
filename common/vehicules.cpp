#include"vehicules.hpp"

vehiculesetup::vehiculesetup()
{
	
}

vehiculesetup::~vehiculesetup()
{
	for(int i=0;i<wheel.size();i++) 
	  delete wheel[i];
}


void vehiculesetup::init(std::vector<object*>& vehicule,vector3d pos,vector3d rot,vector3d s)
{
	
	position=pos;
	scale=s;
	rotation=rot;
	object* model = new object(position, rotation, scale, "data/voiture.obj",false);

    vehicule.push_back(model);
	gravity=200.0f;
    // 4 roues par véhicule
    for (int j = 0; j < 4; ++j) {
        wheel.push_back(new object(vector3d(0, 0, 0), vector3d(0, 0, 0), vector3d(50, 50, 50), "data/roues.obj",false));
    }


        
		gravity=200.0f;
		speed=0;
		angle=1.5f;
		maxSpeed=rand() % 180 + 300;
		acc=12.0f;
		dec=3.0f;
		turnSpeed=0.15f;
		a=vector3d(0.0,0.0,0);
		dec2=30.0f;
		dir=0;
		keyC=false;
		keyV=false;
		control=false;
		control2=false;
		up=false;
		down=false;
		right=false;
		left=false;

	


}

void vehiculesetup::update(std::vector<object*>& vehicule)
{
	move();
		
	float deltaTime=1.0f;
	s=speed;
	rotation.y=angle;
		
	vector3d currentPos = getPosition();
    float distanceMoved = (currentPos - lastPosition).length();

    if (distanceMoved < 0.1f) {  // seuil pour voiture bloquée
        stuckTimer += deltaTime;
    } else {
        stuckTimer = 0.0f; // reset si elle bouge
    }

    // 4?? Vérifie si bloquée depuis 5 secondes
    if (stuckTimer >= 20.0f) {
        isStuck = true;
        unstuck(); // repositionne la voiture
        stuckTimer = 0.0f;
    }
    /*
	for(int i=0;i<2;i++)
	  wheel[i]->setRotationyegal(rot.y);
	  
	for(int i=2;i<4;i++)
	{
	  wheel[i]->setRotationyegal(rot.y-3.0f);
	}
	*/

    lastPosition = currentPos;
}

void vehiculesetup:: draw(std::vector<Triangle>& allTriangles,SDL_Renderer* renderer, int screenWidth, int screenHeight,std::vector<object*>& vehicule,camerasetup* camera)
{
		for(int i=0;i<vehicule.size();i++)
{
	vehicule[i]->getTranslationMatrix().setTranslation(position.x,position.y,position.z);
	vehicule[i]->getRotationMatrixX().setRotationX(rotation.x);
	vehicule[i]->getRotationMatrixY().setRotationY(rotation.y);
	vehicule[i]->getRotationMatrixZ().setRotationZ(rotation.z);

}
	
	 vector3d wheelOffsets[4] = {
        vector3d(150, -25, 160),  // Avant gauche
        vector3d(-250, -25, 160), // Avant droite
        vector3d(150, -25, -160), // Arrière gauche
        vector3d(-250, -25, -160) // Arrière droite
    };

for(int j=0;j<vehicule.size();j++)
{
	
Matrix4x4 rotXMat;
rotXMat.setRotationX(rotation.x);
Matrix4x4 rotYMat;
rotYMat.setRotationY(rotation.y);
Matrix4x4 rotZMat;
rotZMat.setRotationZ(rotation.z);
Matrix4x4 translationMat;



translationMat.setTranslation(position.x, position.y, position.z);

Matrix4x4 vehicleMatrix = translationMat * rotXMat * rotYMat * rotZMat;

for (int i = 0; i < 4; i++) {
	int wheelIndex = j * 4 + i; // ? Index correct pour chaque roue
    vector3d worldPos = vehicleMatrix.apply(wheelOffsets[i]);

    // ? Met à jour la position de la roue
    wheel[wheelIndex]->setLocation(worldPos);

    // ? Neutralise la rotation (pour qu'elles ne tournent pas en Y)
    wheel[wheelIndex]->setRotation(vector3d(0, rotation.y, 0)); // Ou rotationX seulement si tu veux qu'elles tournent en roulant
	
	  if (i == 2 || i == 3) {
        wheel[wheelIndex]->setRotation(vector3d(0,rotation.y+3,0));  // Rotation Y de 180 degrés
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

  bool vehiculesetup::isInViewFrustum(camerasetup* camera,std::vector<object*>& vehicule) const {
  	for(int i=0;i<vehicule.size();i++)
	{
    vector3d camToObj = position - camera->getCamera()->getPosition();
    float dist = camToObj.length();
    if (dist >50000.0f) return false; // trop loin
	}
    return true;
}


void vehiculesetup::setGravity()
{
	s=speed;
	rotation.y=angle;
	position.y-=10.5f;
	
/*	for(int i=0;i<2;i++)
	  wheel[i]->setRotationyegal(rot.y);
	  
	for(int i=2;i<4;i++)
	{
	  wheel[i]->setRotationyegal(rot.y-3.0f);
	}
	
*/
}

void vehiculesetup::unstuck() {
    // 1. centre du circuit
 
    // Déplacer la voiture en arrière
    position = vector3d(6000,1700,15000);
	angle=1.5f;
    std::cout << "Unstuck: reculé de 10 unités\n";
}


std::vector<Triangle> vehiculesetup:: getTriangles()
{
	return triangles;
}
    
float vehiculesetup::getSpeed()
{
	return speed;
}

void vehiculesetup::setLocation(vector3d loc)
{
	position=loc;
}

void vehiculesetup::setLocation(float locY)
{
	position.y=locY;
}

void vehiculesetup::setSpeed(float s)
{
	speed=s;
}

vector3d vehiculesetup::getPosition()
{
	return position;
}


bool vehiculesetup::getUp()
{
	return up;
}

bool vehiculesetup::getDown()
{
	return down;
}

bool vehiculesetup::getRight()
{
	return right;
}

bool vehiculesetup::getLeft()
{
	return left;
}


		void vehiculesetup::setUp(bool b)
		{
			up=b;
		}
		
		void vehiculesetup::setDown(bool b)
		{
			down=b;
		}
		
		void vehiculesetup::setRight(bool b)
		{
			right=b;
		}
		
		void vehiculesetup::setLeft(bool b)
		{
			left=b;
		}


	void vehiculesetup::setDir(int d)
	{
		dir=d;
	}

vector3d vehiculesetup::getRotation()
{
return rotation;	
}

 void vehiculesetup::move()
   {
    position.x -= cos(rotation.y) * s *dir ;
    position.z -= sin(rotation.y) * s *dir;
  
   }


void vehiculesetup::controlFrein()
{
	
				
				if(speed-dec2>0)
				{
					speed-=dec2;
				}
				else if(speed+dec2<0)
				{
					speed+=dec2;
				}
				else
				{
					speed=0;
				}
			
}


vector3d vehiculesetup::getForwardVector() const {
    float yaw = rotation.y; // Angle de rotation autour de l'axe Y

    // Calcul du vecteur avant à partir de l'angle Yaw
    return vector3d(
        cos(yaw),  // Direction en X
        0.0f,      // Pas de changement en Y (reste au sol)
        sin(yaw)   // Direction en Z
    ).normalize(); // Normalisation pour éviter les problèmes d’échelle
}



bool vehiculesetup::getKeyC()
{
	return keyC;
}

bool vehiculesetup::getKeyV()
		{
			return keyV;
		}
		

void vehiculesetup::setKeyC(bool b)
		{
			keyC=b;
		}


void vehiculesetup::setKeyV(bool b)
		{
			keyV=b;
		}
		
		
bool vehiculesetup::getControlActif()
{
	return control;
}

void vehiculesetup::setControlActif(bool b)
{
	control=b;
}

bool vehiculesetup::getControlActif2()
{
	return control2;
}

void vehiculesetup::setControlActif2(bool b)
{
	control2=b;
}


void vehiculesetup::controlUp()
{
	
	
						for(int i=0;i<wheel.size();i++)
				{
				
				//	wheel[i]->setRotationz(1.5f);
				}
				
					if( speed<maxSpeed)
			{
			
				if(speed<0)
				{
					
					speed+=dec;
			 }
				else
				{
					speed+=acc;
				}
			}
			
}

void vehiculesetup::controlRight()
{
		
				
				//	wheel[1]->setRotationy(-1.0f);
				//	wheel[3]->setRotationy(-1.0f);
				/*	if(wheel[1]->getRotation().y<-50.0f)
					{
						wheel[1]->setRotationyegal(-50.0f);
					}
						if(wheel[3]->getRotation().y<-50.0f)
					{
						wheel[3]->setRotationyegal(-50.0f);
					}
					*/
					

		angle-=turnSpeed*(speed/maxSpeed);
	
					
				
			
}
void vehiculesetup::controlLeft()
{
	
			
			/*		wheel[1]->setRotationy(1.0f);
					wheel[3]->setRotationy(1.0f);
						if(wheel[1]->getRotation().y>50.0f)
					{
						wheel[1]->setRotationyegal(50.0f);
					}
					if(wheel[3]->getRotation().y>50.0f)
					{
						wheel[3]->setRotationyegal(50.0f);
					}
					*/
					
					angle+=turnSpeed*(speed/maxSpeed);
						
				
				
}


void vehiculesetup::passiveControlRight_Left()
{
	
		//	wheel[1]->setRotationyegal(0.0f);
		//	wheel[3]->setRotationyegal(0.0f);

}

void vehiculesetup::passiveControlUp_Down()
{
	
				if(speed-dec>0)
				{
					speed-=dec;
				}
				else if(speed+dec<0)
				{
					speed+=dec;
				}
				else
				{
					speed=0;
				}
			
}




