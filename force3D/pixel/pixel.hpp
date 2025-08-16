#ifndef DEF_PIXEL_HPP
#define DEF_PIXEL_HPP
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
#include"../maths/vector.h"
#define SDL_MAIN_HANDLED
#include"C:\library\SDL2-2.0.12\x86_64-w64-mingw32\include\SDL2\SDL.h"

/*
struct SDL_PointAndDepth {
    SDL_Point point; // Coordonnées projetées sur l'écran
    float depth;     // Profondeur (Z) du point 3D
};
*/

template <typename T>
T clamp(T val, T minVal, T maxVal) {
    return (val < minVal) ? minVal : (val > maxVal) ? maxVal : val;
}

struct Point2D {
    int x, y;

    Point2D() : x(0), y(0) {}
    Point2D(int _x, int _y) : x(_x), y(_y) {}

    // Opérateur d'addition
    Point2D operator+(const Point2D& other) const {
        return Point2D(x + other.x, y + other.y);
    }

    // Opérateur de soustraction
    Point2D operator-(const Point2D& other) const {
        return Point2D(x - other.x, y - other.y);
    }
};

class pixel
{
	std::vector<std::vector<float>> zBuffer; // Matrice de profondeur
	public:
		pixel();
		~pixel();
		void clearBuffer();
		Point2D project(const vector3d& v,int width,int height);
		void computeBarycentric(int x, int y, SDL_Point p1, SDL_Point p2, SDL_Point p3, float& alpha, float& beta, float& gamma);
	    void fillTriangle(Uint32* framebuffer, float* framebufferDepth, 
                         SDL_Point p1, SDL_Point p2, SDL_Point p3, 
                         int screenWidth, int screenHeight, 
                         Uint8 r, Uint8 g, Uint8 b, 
                         float z1, float z2, float z3);
        void drawLine(Uint32* framebuffer, Point2D p1, Point2D p2, Uint8 r, Uint8 g, Uint8 b,int screenWidth, int screenHeight);
    	void drawTexturedTriangle(Uint32* framebuffer,float* framebufferDepth,
                          const vector3d& p0, const vector3d& p1, const vector3d& p2,
                          const vector2d& uv0, const vector2d& uv1, const vector2d& uv2,
                          SDL_Surface* texture,int screenWidth, int screenHeight,float z0, float z1, float z2);
	    
};
#endif
