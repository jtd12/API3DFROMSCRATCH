#include"heightmap.h"


height::height()
{

}

height::~height()
{
	
}

void height::loadHeightMap(const char* filename )
	{
		SDL_Surface* img=SDL_LoadBMP(filename);
		std::vector<float> tmp;
		
		if(!img)
		{
			std::cout<<"error img!"<<std::endl;
			return;
		}
		heights.resize(img->h, std::vector<float>(img->w));
		
	   for (int y = 0; y < img->h; y++) {
        for (int x = 0; x < img->w; x++) {
            Uint32 pixel = ((Uint32*) img->pixels)[y * img->pitch / 4 + x];
            Uint8 r, g, b;
            SDL_GetRGB(pixel, img->format, &r, &g, &b);
            heights[y][x] = r / 255.0f;
        }
    }

    SDL_FreeSurface(img);
		
	}


/*std::vector<Triangle> height::convertTriangleStripToTriangles(
    const std::vector<vector3d>& vertices,
    const Material& defaultMaterial)
{
    std::vector<Triangle> triangles;

    if (vertices.size() < 3) return triangles;

    for (size_t i = 0; i < vertices.size() - 2; ++i) {
        // Alternance de l’ordre pour chaque triangle pair/impair
        if (i % 2 == 0) {
            triangles.emplace_back(-1, -1, -1,
                vertices[i], vertices[i + 1], vertices[i + 2],
                vector3d(), vector3d(), vector3d(),
                defaultMaterial
            );
        } else {
            triangles.emplace_back(-1, -1, -1,
                vertices[i + 1], vertices[i], vertices[i + 2],
                vector3d(), vector3d(), vector3d(),
                defaultMaterial
            );
        }
    }

    return triangles;
}
*/


std::vector<Triangle> height::generateTriangles(float size, float h,float r,float g,float b)
{
    std::vector<Triangle> tris;
    tris.reserve((heights.size() - 1) * (heights[0].size() - 1) * 2);

    	//	float baseG = 0.5f + static_cast<float>(rand()) / RAND_MAX * 1.0f;
          //  float baseR = static_cast<float>(rand()) / RAND_MAX * .5f;
           // float baseB = static_cast<float>(rand()) / RAND_MAX * .7f;
           
           float baseG=g;
           float baseR=r;
           float baseB=b;
           
            Material mat;
            mat.diffuseColor = vector3d(baseR, baseG, baseB);

    for (int i = 0; i < heights.size() - 1; i++) {
        for (int j = 0; j < heights[0].size() - 1; j++) {
            vector3d v00(i * size,     heights[i][j] * h,     j * size);
            vector3d v10((i+1) * size, heights[i+1][j] * h,   j * size);
            vector3d v01(i * size,     heights[i][j+1] * h,  (j+1) * size);
            vector3d v11((i+1) * size, heights[i+1][j+1] * h,(j+1) * size);

            tris.emplace_back(v00, v10, v11, mat);
            tris.emplace_back(v00, v11, v01, mat);
        }
    }

    return tris;
}

void height::drawHeight(float size,float h,pixel* pixel_,Uint32* framebuffer,float* framebufferDepth, int screenWidth, int screenHeight, const Camera& camera,float r,float g,float b)
{
	
 	static std::vector<Triangle> terrainTris = generateTriangles(size, h,r,g,b);
    renderTriangles(pixel_, terrainTris, framebuffer, framebufferDepth,
                    screenWidth, screenHeight, camera);
}

void height::renderTriangles(pixel* pixel_,std::vector<Triangle>& allTriangles,Uint32* framebuffer,float* framebufferDepth,int screenWidth, int screenHeight,const Camera& camera)
{
	
 	Matrix4x4 viewProjectionMatrix = camera.getProjectionMatrix() * camera.getViewMatrix(camera,1);
	Matrix4x4 modelTransform =   translationMatrix * scaleMatrix;
	Matrix4x4 finalMatrix = viewProjectionMatrix * modelTransform;

    for (const auto& tri : allTriangles) {
        // Appliquer la transformation (ici identités, à adapter si besoin)
        vector3d tV1 = finalMatrix.apply(tri.v1);
        vector3d tV2 = finalMatrix.apply(tri.v2);
        vector3d tV3 = finalMatrix.apply(tri.v3);

        Point2D p1 = pixel_->project(tV1, screenWidth, screenHeight);
        Point2D p2 = pixel_->project(tV2, screenWidth, screenHeight);
        Point2D p3 = pixel_->project(tV3, screenWidth, screenHeight);

        if (p1.x < 0 || p1.x >= screenWidth-10 || p1.y < 0 || p1.y >= screenHeight-50 ||
            p2.x < 0 || p2.x >= screenWidth-10 || p2.y < 0 || p2.y >= screenHeight-50 ||
            p3.x < 0 || p3.x >= screenWidth-10 || p3.y < 0 || p3.y >= screenHeight-50)
            continue;

        // Lumière
        vector3d normal = (tV3 - tV1).crossproduct(tV2 - tV1).normalize();
        vector3d lightDir = vector3d(100.0f, 100.0f, 15.0f).normalize();
        float intensity = std::max(0.0f, normal.dotproduct(lightDir));

        vector3d baseColor = tri.material.diffuseColor;

		// Appliquer l’éclairage (avec un minimum pour éviter le noir complet)
		float r = baseColor.x * (0.3f + 0.7f * intensity*200);
		float g = baseColor.y * (0.3f + 0.7f * intensity*200);
		float b = baseColor.z * (0.3f + 0.7f * intensity*200);
		
        pixel_->fillTriangle(framebuffer, framebufferDepth, 
                                         { p1.x, screenHeight - p1.y }, 
                                         { p2.x, screenHeight - p2.y }, 
                                         { p3.x, screenHeight - p3.y },
                                         screenWidth, screenHeight, r, g, b, 
                                         tV1.z, tV2.z, tV3.z);
    }
    


}


float height::getHeightAt(float x, float z, float size, float h) {
    int i = static_cast<int>(x / size);
    int j = static_cast<int>(z / size);

    if (i < 0 || j < 0 || i >= heights.size()-1 || j >= heights[0].size()-1)
        return 0.0f; // en dehors de la heightmap

    float fx = (x / size) - i;
    float fz = (z / size) - j;

    // interpolation bilinéaire
    float h00 = heights[i][j] * h;
    float h10 = heights[i+1][j] * h;
    float h01 = heights[i][j+1] * h;
    float h11 = heights[i+1][j+1] * h;

    float h0 = h00 * (1-fx) + h10 * fx;
    float h1 = h01 * (1-fx) + h11 * fx;
    return h0 * (1-fz) + h1 * fz;
}

Matrix4x4& height::getTranslationMatrix()
{
	return translationMatrix;
}



Matrix4x4& height::getScaleMatrix()
{
	return scaleMatrix;
}

