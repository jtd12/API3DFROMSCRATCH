#ifndef DEF_HEIGHTMAP_H
#define DEF_HEIGHTMAP_H
#define SDL_MAIN_HANDLED
#include <thread>
#include <mutex>  // Pour utiliser std::mutex
#include"../../maths/vector.h"
#include"../camera/camera.h"
#include"../../maths/matrix.hpp"
#include"../../loader/objloader.hpp"
#include"../../pixel/pixel.hpp"
#include"../../loader/triangles.hpp"


class height
{
	public:
		height();
		~height();
		void loadHeightMap(const char* filename );
	/*	std::vector<Triangle> convertTriangleStripToTriangles(
    const std::vector<vector3d>& vertices,
   const Material& defaultMaterial);*/
   		std::vector<Triangle> generateTriangles(float size, float h);
   		void drawTriangle(std::vector<Triangle>& allTriangles,pixel* p,Uint32* framebuffer,float* framebufferDepth,int screenWidth, int screenHeight,const vector3d& v1, const vector3d& v2, const vector3d& v3,const Camera& camera);
		void renderTriangles(pixel* pixel_,std::vector<Triangle>& allTriangles,Uint32* framebuffer,float* framebufferDepth,int screenWidth, int screenHeight,const Camera& camera);
		void drawHeight(float size,float h,pixel* pixel_,Uint32* framebuffer,float* framebufferDepth, int screenWidth, int screenHeight, const Camera& camera);
		Matrix4x4& getTranslationMatrix();
    	Matrix4x4& getScaleMatrix();
    	
	private:
		std::vector<std::vector<float> >heights;
		Matrix4x4 translationMatrix;
		Matrix4x4 scaleMatrix;
		
	
};
#endif
