#include"decor.hpp"

decorsetup::decorsetup()
{
    for(int i=0;i<5;i++)
	{
	std::string filename = "data/decor" + std::to_string(i) + ".obj";
	std::cout<<"i:"<<std::to_string(i)<<std::endl;
	decor.push_back(new object(vector3d(0,0,-3000), vector3d(0,0,0), vector3d(5,5,5), filename,false));
	}
}

decorsetup::~decorsetup()
{
     for (int i = 0; i < decor.size(); i++) 
	    delete decor[i];	
}

void decorsetup::update()
{
	
}

void decorsetup::draw(std::vector<Triangle>& allTriangles,SDL_Renderer* renderer, int screenWidth, int screenHeight, camerasetup* camera)
{
   for (int i = 0; i < decor.size(); i++) 
	 decor[i]->draw(renderer, screenWidth, screenHeight, *camera->getCamera(), allTriangles);
}
