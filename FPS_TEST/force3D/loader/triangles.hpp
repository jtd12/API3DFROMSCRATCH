#ifndef TRIANGLES_HPP
#define TRIANGLES_HPP
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
#include <cstdlib>
#include <algorithm>  // Pour std::swap
#include"../maths/vector.h"
#include"../maths/matrix.hpp"




struct Material {
    std::string name;
    vector3d diffuseColor;  // Couleur diffuse
    std::string texturePath; // Chemin de la texture (optionnel)
    SDL_Surface* textureSurface;
    
};

struct uvs
{
	 vector2d uv1, uv2, uv3;

    uvs() : uv1{0, 0}, uv2{0, 0}, uv3{0, 0} {}  // constructeur par défaut

    uvs(const vector2d& _uv1, const vector2d& _uv2, const vector2d& _uv3)
        : uv1(_uv1), uv2(_uv2), uv3(_uv3) {}
	 
};


struct Triangle {
	uvs uv;
	vector3d v1, v2, v3;        // Coordonnées des sommets
	int index1, index2, index3;  // Indices des sommets
    vector3d n1, n2, n3; 
	Material material; 
	bool wireframe = false; // ?? nouveau champ
	float avgDepth; // Champ pour stocker la profondeur moyenne
	
	   Triangle(int _index1, int _index2, int _index3,
             vector3d _v1, vector3d _v2, vector3d _v3,
             vector3d _n1, vector3d _n2, vector3d _n3,
             Material _material)
        : index1(_index1), index2(_index2), index3(_index3),
          v1(_v1), v2(_v2), v3(_v3),
          n1(_n1), n2(_n2), n3(_n3),
          material(_material),
          uv(),   // Initialise à (0,0) si non fournis
          avgDepth(0) {}

    // Constructeur avec UV
    Triangle(int _index1, int _index2, int _index3,
             vector3d _v1, vector3d _v2, vector3d _v3,
             vector3d _n1, vector3d _n2, vector3d _n3,
             Material _material,
             vector2d _uv1, vector2d _uv2, vector2d _uv3)
        : index1(_index1), index2(_index2), index3(_index3),
          v1(_v1), v2(_v2), v3(_v3),
          n1(_n1), n2(_n2), n3(_n3),
          uv{_uv1, _uv2, _uv3},
          material(_material),
          avgDepth(0) {}
          
    Triangle(vector3d _v1, vector3d _v2, vector3d _v3, Material _material)
    : index1(-1), index2(-1), index3(-1),
      v1(_v1), v2(_v2), v3(_v3),
      n1(), n2(), n3(),
      material(_material),
      uv(), avgDepth(0) {}
  

};



#endif
