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
#include"vector.h"
#define M_PI    3.14159265358979323846
#define SDL_MAIN_HANDLED
#include"C:\library\SDL2-2.0.12\x86_64-w64-mingw32\include\SDL2\SDL.h"

/*
struct SDL_PointAndDepth {
    SDL_Point point; // Coordonnées projetées sur l'écran
    float depth;     // Profondeur (Z) du point 3D
};
*/

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
        void drawLineInFramebuffer(Uint32* framebuffer, float* framebufferDepth, int x1, int y1, int x2, int y2, 
                            int screenWidth, int screenHeight, Uint32 color, float z1, float z2);
    
	    
};
#endif
