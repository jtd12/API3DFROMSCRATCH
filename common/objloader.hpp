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
#define M_PI    3.14159265358979323846
#define SDL_MAIN_HANDLED
#include"C:\library\SDL2-2.0.12\x86_64-w64-mingw32\include\SDL2\SDL.h"
#include <cstdlib>
#include <algorithm>  // Pour std::swap
#include"vector.h"
#include"matrix.hpp"
#include"camera.h"
#include"pixel.hpp"
using namespace std;


struct Material {
    std::string name;
    vector3d diffuseColor;  // Couleur diffuse
    std::string texturePath; // Chemin de la texture (optionnel)
};


struct Triangle {
	vector3d v1, v2, v3;        // Coordonnées des sommets
	int index1, index2, index3;  // Indices des sommets
    vector3d n1, n2, n3; 
	Material material; 
	float avgDepth; // Champ pour stocker la profondeur moyenne
	
	 Triangle(int _index1, int _index2, int _index3,
	 		 vector3d _v1, vector3d _v2, vector3d _v3,
             vector3d _n1, vector3d _n2, vector3d _n3,
             Material _material)
        : index1(_index1), index2(_index2), index3(_index3), 
        v1(_v1), v2(_v2), v3(_v3),
          n1(_n1), n2(_n2), n3(_n3),
          material(_material) {}
       
    

};


class objloader {

	 
public:
	objloader();
	~objloader();
	void smoothNormals(std::vector<Triangle>& triangles, std::vector<vector3d>& vertexNormals, int numVertices);
	static bool loadMaterials(const std::string& mtlPath, std::map<std::string, Material>& materials);
    static bool load(const std::string& path, std::vector<Triangle>& triangles);
  /*  void draw(SDL_Renderer* renderer, int screenWidth, int screenHeight, const Camera& camera,Matrix4x4 translationMatrix, Matrix4x4 scaleMatrix, Matrix4x4 rotationMatrixX, 
Matrix4x4 rotationMatrixY,Matrix4x4 rotationMatrixZ,std::vector<Triangle> triangles);*/
   
};

#endif
