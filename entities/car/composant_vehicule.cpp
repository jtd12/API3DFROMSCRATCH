#include"composant_vehicule.hpp"
	
roue::roue(vector3d pos)
{
		obj=new objloader();
		obj->load("data/roues.obj",triangles);
		position=pos;
		scale=vector3d(40,40,40);
}

roue::~roue()
{
	delete obj;
}
void roue::update()
{
	
}

void roue::setLocation(vector3d pos)
{
	position=pos;
}

vector3d roue::getRotation()
{
	return rot_roue;
}

void roue::setRotationz(float m)
		{
			rot_roue.z+=m;
			
		}
	
			void roue::setRotationzegal(float m)
		{
			rot_roue.z=m;
		}
		
			void roue::setRotationy(float m)
		{
			rot_roue.y+=m;
			
		}
		
		
			void roue::setRotationyegal(float m)
		{
			rot_roue.y=m;
		}

	


void roue::draw(SDL_Renderer* renderer, int screenWidth, int screenHeight, const Camera& camera, std::vector<Triangle>& allTriangles) {


  	Matrix4x4 viewProjectionMatrix = camera.getProjectionMatrix() * camera.getViewMatrix(camera,1);
	Matrix4x4 roueTransform = translationMatrix * rotationMatrixX * rotationMatrixY * rotationMatrixZ * scaleMatrix;
	Matrix4x4 finalMatrix = viewProjectionMatrix * roueTransform;
	
	std::vector<vector3d> vertexNormals;  // Stockage des normales lissées
	int numVertices=200;
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
}


void roue::applyMatrix()
{
	translationMatrix.setTranslation(position.x, position.y, position.z);
    rotationMatrixY.setRotationY(rot_roue.y);
    rotationMatrixZ.setRotationZ(rot_roue.z);
    scaleMatrix.setScaling(scale.x, scale.y, scale.z);
    //obj->draw(pRenderer, 800, 600, camera);
    
}

tb::tb(const char * filename)
{
obj=new objloader();
obj->load(filename,triangles);
scale=vector3d(50,50,50);
}

tb::~tb()
{
	delete obj;
}
void tb::update()
{
	
}

void tb::applyMatrix(vector3d position,vector3d rotation)
{
	translationMatrix.setTranslation(position.x, position.y, position.z);
	rotationMatrixY.setRotationY(rotation.y);
    scaleMatrix.setScaling(scale.x, scale.y, scale.z);
    //obj->draw(pRenderer, 800, 600, camera);
    
}


void tb::draw(SDL_Renderer* renderer, int screenWidth, int screenHeight, const Camera& camera, std::vector<Triangle>& allTriangles)
{
	Matrix4x4 viewProjectionMatrix = camera.getProjectionMatrix() * camera.getViewMatrix(camera,1);
	Matrix4x4 roueTransform = translationMatrix * rotationMatrixX * rotationMatrixY * rotationMatrixZ * scaleMatrix;
	Matrix4x4 finalMatrix = viewProjectionMatrix * roueTransform;
	
	std::vector<vector3d> vertexNormals;  // Stockage des normales lissées
	int numVertices=2000;
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
}


porte_vitre::porte_vitre(const char * filename)
{


}
porte_vitre::~porte_vitre()
{
	delete obj;
}
void porte_vitre::update()
{
	
}
void porte_vitre::draw()
{
	
}
void porte::setRotPorte(float r)
{
	rotPorte+=r;
}
void porte::setRotPorteEgal(float r)
{
	rotPorte=r;
}
float porte::getRotPorte()
{
	return rotPorte;
}
porte::porte(const char * filename)
{

}
porte::~porte()
{

}
void porte::update()
{

}
void porte::draw()
{
	
}

vitres::vitres()
{

}
vitres::~vitres()
{
	delete obj;
}

void vitres::update()
{
	
}
void vitres::draw()
{

}


volant::volant()
{

}
volant::~volant()
{
	delete obj;
}

void volant::update()
{
	
}

void volant::draw()
{

}
