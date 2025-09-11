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
#include"heightmap.hpp"
#include"camera.hpp"
#include"../force3D/entities/raceTrack/raceTracks.hpp"



class raceTrackSetup
{

		raceTrackTrack* race;
		
	public:
		raceTrackSetup();
		~raceTrackSetup();
		void initializeTrack(const std::vector<vector3d>& controlPoints, int segmentsPerCurve, float width,height& heightmap);
		vector3d catmullRom(const vector3d& p0, const vector3d& p1, const vector3d& p2, const vector3d& p3, float t);
		void drawTriangle(std::vector<Triangle>& allTriangles,pixel* p,Uint32* framebuffer,float* framebufferDepth,int screenWidth, int screenHeight,const vector3d& v1, const vector3d& v2, const vector3d& v3
		, const Camera& camera,bool isTrack,bool isBorder);
		std::vector<vector3d> generateTrackPath(const std::vector<vector3d>& controlPoints, int segmentsPerCurve);
		std::vector<std::vector<vector3d>> generateTrackBorders(const std::vector<std::tuple<vector3d, vector3d, vector3d, vector3d>>& edges,height& heightmap,float size, float h, float terrainOffset);
		std::vector<std::tuple<vector3d, vector3d, vector3d, vector3d>> generateTrackEdgesWithElevation(const std::vector<vector3d>& path, float width, float elevation,
		height& heightmap,
    float size,
    float h,
    float terrainOffset);
		std::vector<std::pair<vector3d, vector3d>> generateTrackEdges(const std::vector<vector3d>& path, float width, height& heightmap, float size, float h);
		std::vector<std::vector<vector3d>> generateTrackMesh(const std::vector<std::pair<vector3d, vector3d>>& edges, float elevation);
				std::vector<std::pair<vector3d, vector3d>> trackEdges;
				std::vector<std::tuple<vector3d, vector3d, vector3d, vector3d>> trackEdgesElevated;
		std::vector<vector3d> generateWaypoints(const std::vector<vector3d>& controlPoints, float spacing);
		bool  isPointOnTrack(float x, float z);
		raceTrackTrack* getRace();

};

#endif
