#include"drawPixels.hpp"

drawPixels::drawPixels()
{
	pixel_=new pixel();
}

drawPixels::~drawPixels()
{
delete pixel_;	
}

void drawPixels::draw(std::vector<Triangle>& allTriangles,Uint32* framebuffer,float* framebufferDepth, int screenWidth, int screenHeight, const Camera& camera)
{
	   for (auto& tri : allTriangles) {
        tri.avgDepth = (tri.v1.z + tri.v2.z + tri.v3.z) / 3.0f;
        
         
    }

    // **Tri rapide des triangles (éviter `std::stable_sort` si possible)**
    std::sort(allTriangles.begin(), allTriangles.end(), 
              [](const Triangle& t1, const Triangle& t2) {
                  return t1.avgDepth > t2.avgDepth;
              });
              
    
     for (auto& tri : allTriangles) {
     	

        Point2D p1 = pixel_->project(tri.v1, screenWidth, screenHeight);
        Point2D p2 = pixel_->project(tri.v2, screenWidth, screenHeight);
        Point2D p3 = pixel_->project(tri.v3, screenWidth, screenHeight);

        // Vérification des limites écran
        if (p1.x < 0 || p1.x >= screenWidth -0.5f || p1.y < 0|| p1.y >= screenHeight-0.5f ||
            p2.x < 0 || p2.x >= screenWidth -0.5f|| p2.y < 0 || p2.y >= screenHeight-0.5f ||
            p3.x < 0 || p3.x >= screenWidth-0.5f || p3.y < 0| p3.y >= screenHeight-0.5f) {
            continue;
        }

        vector3d normal = (tri.v3 - tri.v1).crossproduct(tri.v2 - tri.v1).normalize();
        vector3d lightDir = vector3d(0.0f, -1.0f, 1000.0f).normalize();
        float intensity = std::max(0.0f, normal.dotproduct(lightDir));

        Uint8 r = std::min(255, static_cast<int>(tri.material.diffuseColor.x * intensity * 255));
        Uint8 g = std::min(255, static_cast<int>(tri.material.diffuseColor.y * intensity * 255));
        Uint8 b = std::min(255, static_cast<int>(tri.material.diffuseColor.z * intensity * 255));
        
        if (tri.material.textureSurface) {
        // Rendu texturé
        pixel_->drawTexturedTriangle(framebuffer, framebufferDepth,
            { p1.x, screenHeight - p1.y },
            { p2.x, screenHeight - p2.y },
            { p3.x, screenHeight - p3.y },
            tri.uv.uv1, tri.uv.uv2, tri.uv.uv3,
            tri.material.textureSurface,screenWidth, screenHeight,
            tri.v1.z, tri.v2.z, tri.v3.z
        );
       
        
    }
	else if (tri.wireframe) {
    pixel_->drawLine(framebuffer, { p1.x, screenHeight - p1.y }, { p2.x, screenHeight - p2.y }, r, g, b, screenWidth, screenHeight);
    pixel_->drawLine(framebuffer, { p2.x, screenHeight - p2.y }, { p3.x, screenHeight - p3.y }, r, g, b, screenWidth, screenHeight);
    pixel_->drawLine(framebuffer, { p3.x, screenHeight - p3.y }, { p1.x, screenHeight - p1.y }, r, g, b, screenWidth, screenHeight);
}  
	else {
        // Rendu coloré simple
        pixel_->fillTriangle(framebuffer, framebufferDepth,
            { p1.x, screenHeight - p1.y },
            { p2.x, screenHeight - p2.y },
            { p3.x, screenHeight - p3.y },
            screenWidth, screenHeight, r, g, b,
            tri.v1.z, tri.v2.z, tri.v3.z
        );
    }
    
    

}
}


pixel* drawPixels::getPixel()
{
	return pixel_;
}

