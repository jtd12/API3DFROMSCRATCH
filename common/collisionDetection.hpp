#ifndef DEF_COLLISIONDETECTION_HPP
#define DEF_COLLISIONDETECTION_HPP
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
#include"vector.h"
#include"boundingBox.hpp"

class collisions
{	
	public:
	collisions();
	~collisions();
	bool pointInTriangle(const vector3d& P, const vector3d& A, const vector3d& B, const vector3d& C);
	bool aabbIntersectsTriangle(const AABB& box, const vector3d& v0, const vector3d& v1, const vector3d& v2);
};
#endif
