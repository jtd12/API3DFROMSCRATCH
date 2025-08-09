#ifndef DEF_RACETRACK_HPP
#define DEF_RACETRACK_HPP
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
#include"vector.h"
#include"matrix.hpp"
#include"camera.h"
#include"pixel.hpp"
#include"objloader.hpp"

class raceTrack
{
		Matrix4x4 translationMatrix;
		Matrix4x4 scaleMatrix;
		Matrix4x4 rotationMatrixX;
		Matrix4x4 rotationMatrixY;
		Matrix4x4 rotationMatrixZ;
	public:
		raceTrack();
		~raceTrack();
		void initializeTrack(const std::vector<vector3d>& controlPoints, int segmentsPerCurve, float width);
		vector3d catmullRom(const vector3d& p0, const vector3d& p1, const vector3d& p2, const vector3d& p3, float t);
		void drawTriangle(std::vector<Triangle>& allTriangles,pixel* p,Uint32* framebuffer,float* framebufferDepth,int screenWidth, int screenHeight,const vector3d& v1, const vector3d& v2, const vector3d& v3
		,const Camera& camera,bool isTrack,bool isBorder);
		float getTerrainHeight(float x, float z);
		std::vector<std::vector<vector3d>> generateTerrain(float width, float length, int resolution);
		std::vector<vector3d> generateTrackPath(const std::vector<vector3d>& controlPoints, int segmentsPerCurve);
		std::vector<std::vector<vector3d>> generateTrackBorders(const std::vector<std::tuple<vector3d, vector3d, vector3d, vector3d>>& edges);
		std::vector<std::tuple<vector3d, vector3d, vector3d, vector3d>> generateTrackEdgesWithElevation(const std::vector<vector3d>& path, float width, float elevation);
		std::vector<std::pair<vector3d, vector3d>> generateTrackEdges(const std::vector<vector3d>& path, float width);
		std::vector<std::vector<vector3d>> generateTrackMesh(const std::vector<std::pair<vector3d, vector3d>>& edges, float elevation);
				std::vector<std::pair<vector3d, vector3d>> trackEdges;
				std::vector<std::tuple<vector3d, vector3d, vector3d, vector3d>> trackEdgesElevated;
			
		std::vector<vector3d> generateWaypoints(const std::vector<vector3d>& controlPoints, float spacing);
};

#endif
