#include"racetrack.hpp"

raceTrackSetup::raceTrackSetup()
{
	race=new raceTrackTrack();
}

raceTrackSetup::~raceTrackSetup()
{
	delete race;
}



void raceTrackSetup::initializeTrack(const std::vector<vector3d>& controlPoints, int segmentsPerCurve, float width,height& heightmap) {
    race->initializeTrack( controlPoints, segmentsPerCurve,  width,heightmap);
}




std::vector<vector3d> raceTrackSetup:: generateTrackPath(const std::vector<vector3d>& controlPoints, int segmentsPerCurve) {
     return race->generateTrackPath(controlPoints,segmentsPerCurve);
}



std::vector<std::pair<vector3d, vector3d>> raceTrackSetup::generateTrackEdges(const std::vector<vector3d>& path, float width, height& heightmap, float size, float h) {
    std::vector<std::pair<vector3d, vector3d>> edges;
    
 return race->generateTrackEdges( path,width, heightmap,  size,  h); 
}

std::vector<std::tuple<vector3d, vector3d, vector3d, vector3d>> 
raceTrackSetup::generateTrackEdgesWithElevation(const std::vector<vector3d>& path, float width, float elevation,
height& heightmap,
    float size,
    float h,
    float terrainOffset) {
     return race->generateTrackEdgesWithElevation( path, width, elevation,heightmap,size,h,terrainOffset);
}

std::vector<std::vector<vector3d>> raceTrackSetup::generateTrackBorders(const std::vector<std::tuple<vector3d, vector3d, vector3d, vector3d>>& edges,height& heightmap
,float size, float h, float terrainOffset) {
   
 return race->generateTrackBorders(edges, heightmap,size,  h,  terrainOffset);
}


	std::vector<vector3d> raceTrackSetup::generateWaypoints(const std::vector<vector3d>& controlPoints, float spacing) {
   
 return race->generateWaypoints( controlPoints,  spacing);
 
}

std::vector<std::vector<vector3d>> raceTrackSetup::generateTrackMesh(const std::vector<std::pair<vector3d, vector3d>>& edges, float elevation) {

   return race->generateTrackMesh(edges, elevation);
}

void raceTrackSetup::drawTriangle(std::vector<Triangle>& allTriangles,pixel* p,Uint32* framebuffer,float* framebufferDepth,int screenWidth, int screenHeight,const vector3d& v1, const vector3d& v2, const vector3d& v3, const Camera& camera,
bool isTrack,bool isBorder) {
	
 race->drawTriangle( allTriangles, p, framebuffer,framebufferDepth,screenWidth,  screenHeight,  v1,  v2,  v3, camera, isTrack, isBorder);
}

bool  raceTrackSetup::isPointOnTrack(float x, float z) {
 	return	race->isPointOnTrack(x,z);
    }

raceTrackTrack* raceTrackSetup::getRace()
{
	return race;
}


