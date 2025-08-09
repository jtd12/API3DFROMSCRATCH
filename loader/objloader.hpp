#ifndef OBJLOADER_H
#define OBJLOADER_H
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
#include <map>
#define SDL_MAIN_HANDLED
#include"C:\library\SDL2-2.0.12\x86_64-w64-mingw32\include\SDL2\SDL.h"
#include <cstdlib>
#include <algorithm>  // Pour std::swap
#include"../maths/vector.h"
#include"../collision/boundingBox.hpp"
#include"../maths/matrix.hpp"
#include"../entities/camera/camera.h"
#include"../pixel/pixel.hpp"
#include"triangles.hpp"
using namespace std;




class objloader {
AABB box;
public:
	objloader();
	~objloader();
	void smoothNormals(std::vector<Triangle>& triangles, std::vector<vector3d>& vertexNormals, int numVertices);
	 bool loadMaterials(const std::string& mtlPath, std::map<std::string, Material>& materials);
     bool load(const std::string& path,std::vector<Triangle>& triangles);

  /*  void draw(SDL_Renderer* renderer, int screenWidth, int screenHeight, const Camera& camera,Matrix4x4 translationMatrix, Matrix4x4 scaleMatrix, Matrix4x4 rotationMatrixX, 
Matrix4x4 rotationMatrixY,Matrix4x4 rotationMatrixZ,std::vector<Triangle> triangles);*/
   
};

#endif
