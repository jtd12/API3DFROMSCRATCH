#include"car.h"

vehicule::vehicule(const std::string& filename,vector3d pos)
{

		obj=new objloader();
		obj->load(filename,triangles);
		tableau_bord=new tb("data/tb.obj");
		for(int i=0;i<4;i++)
		 wheel.push_back(new roue(vector3d(0,2000,5000)));
		position=pos;
		scale=vector3d(50,50,50);
		rot=vector3d(0,0,0);
		speed=0;
		angle=1.5f;
		maxSpeed=rand() % 180 + 220;
		acc=7.0f;
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

vehicule::~vehicule()
{
delete obj;
}


vector3d vehicule::getForwardVector() const {
    float yaw = rot.y; // Angle de rotation autour de l'axe Y

    // Calcul du vecteur avant à partir de l'angle Yaw
    return vector3d(
        cos(yaw),  // Direction en X
        0.0f,      // Pas de changement en Y (reste au sol)
        sin(yaw)   // Direction en Z
    ).normalize(); // Normalisation pour éviter les problèmes d’échelle
}

void vehicule::update()
{
	
	s=speed;
	rot.y=angle;
	

	move();

	position.y-=1.5f;
	
	
	for(int i=0;i<2;i++)
	  wheel[i]->setRotationyegal(rot.y);
	  
	for(int i=2;i<4;i++)
	{
	  wheel[i]->setRotationyegal(rot.y-3.0f);
	}



}
/*
vector3d vehicule::getForwardVector2() const
{
    float cosPitch = cos(rot.x);
    float sinPitch = sin(rot.x);
    float cosYaw = cos(rot.y);
    float sinYaw = sin(rot.y);

vector3d forward;
forward.x = sinYaw * cosPitch;
forward.y = sinPitch;
forward.z = -cosYaw * cosPitch;

    return vector3d(forward.x, forward.y, forward.z).normalize();
}
*/

std::vector<Triangle> vehicule:: getTriangles()
{
	return triangles;
}
    
float vehicule::getSpeed()
{
	return speed;
}

void vehicule::setLocation(vector3d loc)
{
	position=loc;
}

void vehicule::setLocation(float locY)
{
	position.y=locY;
}

void vehicule::setSpeed(float s)
{
	speed=s;
}

vector3d vehicule::getPosition()
{
	return position;
}


bool vehicule::getUp()
{
	return up;
}

bool vehicule::getDown()
{
	return down;
}

bool vehicule::getRight()
{
	return right;
}

bool vehicule::getLeft()
{
	return left;
}


		void vehicule::setUp(bool b)
		{
			up=b;
		}
		
		void vehicule::setDown(bool b)
		{
			down=b;
		}
		
		void vehicule::setRight(bool b)
		{
			right=b;
		}
		
		void vehicule::setLeft(bool b)
		{
			left=b;
		}


	void vehicule::setDir(int d)
	{
		dir=d;
	}

vector3d vehicule::getRotation()
{
return rot;	
}

 void vehicule::move()
   {
    position.x -= cos(rot.y) * s *dir ;
    position.z -= sin(rot.y) * s *dir;
  
   }


bool vehicule::getKeyC()
{
	return keyC;
}
		bool vehicule::getKeyV()
		{
			return keyV;
		}
		
		void vehicule::setKeyC(bool b)
		{
			keyC=b;
		}
		
		void vehicule::setKeyV(bool b)
		{
			keyV=b;
		}
		
		
bool vehicule::getControlActif()
{
	return control;
}

void vehicule::setControlActif(bool b)
{
	control=b;
}

bool vehicule::getControlActif2()
{
	return control2;
}

void vehicule::setControlActif2(bool b)
{
	control2=b;
}


void vehicule::controlUp()
{
	
	
						for(int i=0;i<wheel.size();i++)
				{
				
					wheel[i]->setRotationz(1.5f);
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

void vehicule::controlRight()
{
		
				
					wheel[1]->setRotationy(-1.0f);
					wheel[3]->setRotationy(-1.0f);
					if(wheel[1]->getRotation().y<-50.0f)
					{
						wheel[1]->setRotationyegal(-50.0f);
					}
						if(wheel[3]->getRotation().y<-50.0f)
					{
						wheel[3]->setRotationyegal(-50.0f);
					}
					
					

		angle-=turnSpeed*(speed/maxSpeed);
	
					
				
			
}
void vehicule::controlLeft()
{
	
			
					wheel[1]->setRotationy(1.0f);
					wheel[3]->setRotationy(1.0f);
						if(wheel[1]->getRotation().y>50.0f)
					{
						wheel[1]->setRotationyegal(50.0f);
					}
					if(wheel[3]->getRotation().y>50.0f)
					{
						wheel[3]->setRotationyegal(50.0f);
					}
					
					
					angle+=turnSpeed*(speed/maxSpeed);
						
				
				
}


void vehicule::passiveControlRight_Left()
{
	
			wheel[1]->setRotationyegal(0.0f);
			wheel[3]->setRotationyegal(0.0f);

}

void vehicule::passiveControlUp_Down()
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

void vehicule::controlFrein()
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






void vehicule::ouvrirPorte1()
{

}
void vehicule::ouvrirPorte2()
{

}


void vehicule::draw(SDL_Renderer* renderer, int screenWidth, int screenHeight, const Camera& camera, std::vector<Triangle>& allTriangles) {

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
    tableau_bord->draw(renderer, screenWidth, screenHeight, camera, allTriangles);
}


void vehicule::applyMatrix()
{
	translationMatrix.setTranslation(position.x, position.y, position.z);
	rotationMatrixY.setRotationY(rot.y);
	scaleMatrix.setScaling(scale.x,scale.y,scale.z);
	
	tableau_bord->applyMatrix(position,rot);
	

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
