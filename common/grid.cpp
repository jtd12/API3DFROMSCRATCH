#include"grid.hpp"

grid::grid()
{
	obj=new objloader();
	position=vector3d(0,-30,100);
	scale=vector3d(5,1,5);

}
grid::~grid()
{
	
}


void grid::applyCameraOrientationToGrid(Matrix4x4& gridRotationMatrix, const Camera& camera) {
    // Récupérer l'orientation de la caméra
    vector3d camForward, camRight, camUp;
    camera.getOrientation(camForward, camRight, camUp);

    // Créer une matrice de rotation basée sur l'orientation de la caméra
    // La matrice de rotation est construite en utilisant les axes de la caméra
    Matrix4x4 cameraRotationMatrix;
    cameraRotationMatrix.setColumn(0, camRight);  // axe "right"
    cameraRotationMatrix.setColumn(1, camUp);     // axe "up"
    cameraRotationMatrix.setColumn(2, camForward); // axe "forward"

    // Appliquer la matrice de rotation de la caméra au grid
    gridRotationMatrix = cameraRotationMatrix;  // Appliquer la rotation de la caméra
}

void grid::drawGrid3D(Uint32* framebuffer, float* framebufferDepth, int screenWidth, int screenHeight, 
                       int gridSize, int numLines,  const Camera& camera, pixel* p) {
	
	
	rotationMatrixX.setRotationX(1.5f);
	Uint32 color = SDL_MapRGB(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888), 100, 100, 100);
	
    Matrix4x4 viewProjectionMatrix = camera.getProjectionMatrix() * camera.getViewMatrix(camera,-1);
	Matrix4x4 gridTransform = translationMatrix * rotationMatrixX * scaleMatrix;
	Matrix4x4 finalMatrix = viewProjectionMatrix * gridTransform;

    
float halfSize = (numLines / 2) * gridSize;

    for (int i = -numLines / 2; i <= numLines / 2; i++) {
    	
        float x = i * gridSize;
        float zMin = -halfSize;
        float zMax = halfSize;
        float z = i * gridSize;
        float xMin = -halfSize;
        float xMax = halfSize;

        // Lignes parallèles à l'axe Z
        vector3d startX = { x,  zMin,5000 };
        vector3d endX   = { x,  zMax,5000 };

        // Lignes parallèles à l'axe X
        vector3d startZ = { xMin, z, 5000 };
        vector3d endZ   = { xMax, z, 5000 };

        startX = finalMatrix.apply(startX);
        endX = finalMatrix.apply(endX);
        startZ = finalMatrix.apply(startZ);
        endZ = finalMatrix.apply(endZ);

   

        // Projeter les points en 2D
        Point2D p1 = p->project(startX, screenWidth, screenHeight);
        Point2D p2 = p->project(endX, screenWidth, screenHeight);
        Point2D p3 = p->project(startZ, screenWidth, screenHeight);
        Point2D p4 = p->project(endZ, screenWidth, screenHeight);

        // Dessiner les lignes avec SDL
         p->drawLineInFramebuffer(framebuffer, framebufferDepth, p1.x, p1.y, p2.x, p2.y, screenWidth, screenHeight, color, startX.z, endX.z);
         p->drawLineInFramebuffer(framebuffer, framebufferDepth, p3.x, p3.y, p4.x, p4.y, screenWidth, screenHeight, color, startZ.z, endZ.z);
    }
}

