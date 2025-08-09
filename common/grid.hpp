#ifndef DEF_GRID_HPP
#define DEF_GRID_HPP
#include<stdio.h>
#include<iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>
#include <chrono>
#include <thread>
#include <conio.h> // Pour _kbhit() et _getch()
#include <algorithm>
#include <cfloat>
#include<cmath>
#define MAXLEVEL 4
#define M_PI    3.14159265358979323846
#define SDL_MAIN_HANDLED
#include"C:\library\SDL2-2.0.12\x86_64-w64-mingw32\include\SDL2\SDL.h"
#include <cstdlib>
#include <algorithm>  // Pour std::swap
#include"objloader.hpp"
#include"vector.h"
#include"matrix.hpp"
using namespace std;

class grid
{
	vector3d position;
	vector3d scale;
	Matrix4x4 translationMatrix;
	Matrix4x4 scaleMatrix;
	Matrix4x4 rotationMatrixX;
	Matrix4x4 rotationMatrixY;
	Matrix4x4 rotationMatrixZ;
	objloader *obj;
	public:
	grid();
	~grid();
	void drawGrid3D(Uint32* framebuffer, float* framebufferDepth, int screenWidth, int screenHeight, 
                       int gridSize, int numLines,  const Camera& camera, pixel* p);
	void applyCameraOrientationToGrid(Matrix4x4& gridRotationMatrix, const Camera& camera);
};

#endif
