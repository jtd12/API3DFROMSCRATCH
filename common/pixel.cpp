#include"pixel.hpp"

pixel::pixel()
{
	zBuffer.resize(800, std::vector<float>(600, std::numeric_limits<float>::infinity()));
}

pixel::~pixel()
{
	
}

void pixel::fillTriangle(Uint32* framebuffer, float* framebufferDepth, 
                         SDL_Point p1, SDL_Point p2, SDL_Point p3, 
                         int screenWidth, int screenHeight, 
                         Uint8 r, Uint8 g, Uint8 b, 
                         float z1, float z2, float z3) {

   
    
    // Trier les points par y croissant
    if (p1.y > p2.y) std::swap(p1, p2), std::swap(z1, z2);
    if (p1.y > p3.y) std::swap(p1, p3), std::swap(z1, z3);
    if (p2.y > p3.y) std::swap(p2, p3), std::swap(z2, z3);

    // Calcul des bords du triangle (gauche et droite)
    auto interpolate = [](int y1, int y2, int x1, int x2, float z1, float z2) {
        float dx = (float)(x2 - x1) / (y2 - y1);
        float dz = (z2 - z1) / (y2 - y1);
        return [=](int y) {
            float x = x1 + dx * (y - y1);
            float z = z1 + dz * (y - y1);
            return std::make_pair((int)x, z);
        };
    };

    // Crée les fonctions d'interpolation pour les bords gauche et droit
    auto leftEdge = interpolate(p1.y, p3.y, p1.x, p3.x, z1, z3);
    auto rightEdge = interpolate(p1.y, p2.y, p1.x, p2.x, z1, z2);
    auto rightEdge2 = interpolate(p2.y, p3.y, p2.x, p3.x, z2, z3);

    // Remplir le triangle scanline par scanline
    for (int y = p1.y; y <= p3.y; y++) {
        if (y < 0 || y >= screenHeight) continue; // Clip écran

        // Obtenir les coordonnées x et z pour la ligne actuelle
        auto left = leftEdge(y);
        auto right = rightEdge(y);
        auto right2 = rightEdge2(y);

        int xl = left.first, zl = left.second;
        int xr = right.first, zr = right.second;

        // Fusionner les segments pour le bord droit
        if (y >= p2.y) {
            xr = right2.first;
            zr = right2.second;
        }

        // Correction si inversion
        if (xl > xr) std::swap(xl, xr), std::swap(zl, zr);

        // Ne pas dessiner si la ligne est hors écran
        if (xl < 0 || xl >= screenWidth || xr < 0 || xr >= screenWidth) {
            continue;
        }

       float dz = 0;
if (xr != xl) {
    dz = (zr - zl) / (xr - xl);
}
float z = zl;

// Remplir les pixels entre xl et xr
for (int x = xl; x <= xr; x++, z += dz) {
    if (x < 0 || x >= screenWidth) continue; // Clip écran

    int index = y * screenWidth + x;

    if (z < framebufferDepth[index]) { // Test du Z-Buffer
        framebufferDepth[index] = z;

        // Encodage de la couleur en 32 bits (RGBA)
        framebuffer[index] = (r << 24) | (g << 16) | (b << 8) | 255;
    }
}
    }
}


void pixel:: drawLineInFramebuffer(Uint32* framebuffer, float* framebufferDepth, int x1, int y1, int x2, int y2, 
                            int screenWidth, int screenHeight, Uint32 color, float z1, float z2) {
    
    // Calcul des différences
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    int x = x1;
    int y = y1;

    // Dessiner la ligne
    while (true) {
        // Vérifier si le pixel est dans les limites de l'écran
        if (x >= 0 && x < screenWidth && y >= 0 && y < screenHeight) {
            int index = y * screenWidth + x;
            
            // Vérifier la profondeur avec le Z-buffer
            float z = z1 + ((float)(x - x1) / (x2 - x1)) * (z2 - z1);  // Interpolation linéaire de la profondeur
            if (z < framebufferDepth[index]) {
                framebufferDepth[index] = z;
                framebuffer[index] = color;  // Dessiner le pixel
            }
        }

        // Vérifier si on a atteint la fin de la ligne
        if (x == x2 && y == y2) break;

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x += sx;
        }
        if (e2 < dx) {
            err += dx;
            y += sy;
        }
    }
}

void pixel::computeBarycentric(int x, int y, SDL_Point p1, SDL_Point p2, SDL_Point p3, float& alpha, float& beta, float& gamma) {
    float denom = (p2.y - p3.y) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.y - p3.y);
    alpha = ((p2.y - p3.y) * (x - p3.x) + (p3.x - p2.x) * (y - p3.y)) / denom;
    beta = ((p3.y - p1.y) * (x - p3.x) + (p1.x - p3.x) * (y - p3.y)) / denom;
    gamma = 1.0f - alpha - beta;
}

Point2D pixel::project(const vector3d& v,int width,int height) {
	
if (v.z <= 1.0f) return { -1, -1 };

    // Pré-calculer les valeurs constantes
    const float invZ = 1.0f / v.z;
    const float halfWidth = width * 0.5f;
    const float halfHeight = height * 0.5f;

    // Projection optimisée (moins d'opérations)
    int x = static_cast<int>((v.x * invZ) * halfWidth + halfWidth);
    int y = static_cast<int>(halfHeight - (v.y * invZ) * halfHeight);

    // Clipping manuel sans `std::max` ni `std::min`
    if (x < 0) x = 0;
    else if (x >= width) x = width - 10;
    
    if (y < 0) y = 0;
    else if (y >= height) y = height;

    return Point2D(x, y);
}


void pixel::clearBuffer() {
    for (auto& row : zBuffer) {
         std::fill(row.begin(), row.end(), std::numeric_limits<float>::max()); 
    }
}


