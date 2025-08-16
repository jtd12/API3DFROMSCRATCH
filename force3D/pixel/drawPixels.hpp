#ifndef DEF_DRAW_PIXELS_HPP
#define DEF_DRAW_PIXELS_HPP
#include"../entities/camera/camera.h"
#include"../maths/vector.h"
#include"../loader/objloader.hpp"
#include"pixel.hpp"
#include"../gameEngine/gameLoop.hpp"
#include"../gameEngine/frameBuffer.hpp"

class drawPixels
{
	pixel *pixel_;
	public:
		drawPixels();
		~drawPixels();
		void draw(std::vector<Triangle>& allTriangles,Uint32* framebuffer,float* framebufferDepth, int screenWidth, int screenHeight, const Camera& camera);
		pixel* getPixel();
};
#endif
