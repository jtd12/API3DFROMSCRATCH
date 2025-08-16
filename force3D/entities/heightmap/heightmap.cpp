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


std::vector<Triangle> height::generateTriangles(float size, float h)
{
    std::vector<Triangle> tris;
    tris.reserve((heights.size() - 1) * (heights[0].size() - 1) * 2);

    Material mat;
    mat.diffuseColor = vector3d(1.0f, 1.0f, 1.0f);

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

void height::drawHeight(float size,float h,pixel* pixel_,Uint32* framebuffer,float* framebufferDepth, int screenWidth, int screenHeight, const Camera& camera)
{
	
 	static std::vector<Triangle> terrainTris = generateTriangles(size, h);
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

        Uint8 r = std::min(255, static_cast<int>(150 * intensity));
        Uint8 g = r;
        Uint8 b = r;

        pixel_->fillTriangle(framebuffer, framebufferDepth, 
                                         { p1.x, screenHeight - p1.y }, 
                                         { p2.x, screenHeight - p2.y }, 
                                         { p3.x, screenHeight - p3.y },
                                         screenWidth, screenHeight, r, g, b, 
                                         tV1.z, tV2.z, tV3.z);
    }
    


}


Matrix4x4& height::getTranslationMatrix()
{
	return translationMatrix;
}


Matrix4x4& height::getScaleMatrix()
{
	return scaleMatrix;
}

