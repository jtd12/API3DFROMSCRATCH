#include"pixel.hpp"

pixel::pixel()
{
	zBuffer.resize(800, std::vector<float>(600, std::numeric_limits<float>::infinity()));
}

pixel::~pixel()
{
	
}


void pixel::drawTexturedTriangle(Uint32*  framebuffer,float* framebufferDepth,
                          const vector3d& p0, const vector3d& p1, const vector3d& p2,
                          const vector2d& uv0, const vector2d& uv1, const vector2d& uv2,
                          SDL_Surface* texture,int screenWidth, int screenHeight,float z0, float z1, float z2) {
    // Projection simple 2D (à adapter si besoin)
   auto project = [](const vector3d& v) -> SDL_Point {
        return SDL_Point{ (int)v.x, (int)v.y };
    };

    SDL_Point a = project(p0);
    SDL_Point b = project(p1);
    SDL_Point c = project(p2);

    // Bounding box
    int minX = std::max(0, std::min({ a.x, b.x, c.x }));
    int maxX = std::min(screenWidth - 1, std::max({ a.x, b.x, c.x }));
    int minY = std::max(0, std::min({ a.y, b.y, c.y }));
    int maxY = std::min(screenHeight - 1, std::max({ a.y, b.y, c.y }));

    float denom = ((b.y - c.y)*(a.x - c.x) + (c.x - b.x)*(a.y - c.y));
    if (denom == 0) return; // Triangle dégénéré

    // Pas besoin de lock le framebuffer ici (c’est un tableau brut)
	
	// On calcule les inverses des z

		float invZ0 = 1.0f / z0;
		float invZ1 = 1.0f / z1;
		float invZ2 = 1.0f / z2;
		
		float u0 = uv0.x * invZ0;
		float v0 = uv0.y * invZ0;
		float u1 = uv1.x * invZ1;
		float v1 = uv1.y * invZ1;
		float u2 = uv2.x * invZ2;
		float v2 = uv2.y * invZ2;

    for (int y = minY; y <= maxY; y++) {
        for (int x = minX; x <= maxX; x++) {
            float w1 = ((b.y - c.y)*(x - c.x) + (c.x - b.x)*(y - c.y)) / denom;
            float w2 = ((c.y - a.y)*(x - c.x) + (a.x - c.x)*(y - c.y)) / denom;
            float w3 = 1.0f - w1 - w2;

            if (w1 >= 0 && w2 >= 0 && w3 >= 0) {
                // Interpolation UV
                
             
				float invZ = w1 * invZ0 + w2 * invZ1 + w3 * invZ2;
				float u = (w1 * u0 + w2 * u1 + w3 * u2) / invZ;
				float v = (w1 * v0 + w2 * v1 + w3 * v2) / invZ;

                int texU = clamp(int(u * texture->w), 0, texture->w - 1);
                int texV = clamp(int((1.0f - v) * texture->h), 0, texture->h - 1);

                Uint32* texPixels = (Uint32*)texture->pixels;
                Uint32 texel = ((Uint32*)((Uint8*)texture->pixels + texV * texture->pitch))[texU];
				Uint8 a = (texel >> 24) & 0xFF;
				Uint8 r = (texel >> 16) & 0xFF;
				Uint8 g = (texel >> 8)  & 0xFF;
				Uint8 b = texel & 0xFF;
				
				// Réassemblage ARGB
				Uint32 corrected = (a << 32) | (b<< 24) | (r << 16) | (g << 8) ;
				
                // Interpolation Z
                
				float z = 1.0f / invZ;
                int index = y * screenWidth + x;

                if (z < framebufferDepth[index]) {
                    framebufferDepth[index] = z;
                    framebuffer[index] = corrected;
                }
            }
        }
    }
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


void pixel::drawLine(Uint32* framebuffer, Point2D p1, Point2D p2, Uint8 r, Uint8 g, Uint8 b,int screenWidth, int screenHeight) {
    int x0 = p1.x;
    int y0 = p1.y;
    int x1 = p2.x;
    int y1 = p2.y;

    // Bresenham ou DDA selon ta préférence
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;

    while (true) {
        if (x0 >= 0 && x0 < screenWidth && y0 >= 0 && y0 < screenHeight)
            framebuffer[y0 * screenWidth + x0] = (255 << 24) | (r << 16) | (g << 8) | b;

        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

void pixel::computeBarycentric(int x, int y, SDL_Point p1, SDL_Point p2, SDL_Point p3, float& alpha, float& beta, float& gamma) {
    float denom = (p2.y - p3.y) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.y - p3.y);
    alpha = ((p2.y - p3.y) * (x - p3.x) + (p3.x - p2.x) * (y - p3.y)) / denom;
    beta = ((p3.y - p1.y) * (x - p3.x) + (p1.x - p3.x) * (y - p3.y)) / denom;
    gamma = 1.0f - alpha - beta;
}

Point2D pixel::project(const vector3d& v,int width,int height) {
	
if (v.z <= 0.1f) return { -1, -1 };

    // Pré-calculer les valeurs constantes
    const float invZ = 1.0f / v.z;
    const float halfWidth = width * 0.5f;
    const float halfHeight = height * 0.5f;

    // Projection optimisée (moins d'opérations)
    int x = static_cast<int>((v.x * invZ) * halfWidth + halfWidth);
    int y = static_cast<int>(halfHeight - (v.y * invZ) * halfHeight);

    // Clipping manuel sans `std::max` ni `std::min`
    if (x < 0) x = 0;
    else if (x >= width) x = width - 1;
    
    if (y < 0) y = 0;
    else if (y >= height) y = height - 1;

    return Point2D(x, y);
}


void pixel::clearBuffer() {
    for (auto& row : zBuffer) {
         std::fill(row.begin(), row.end(), std::numeric_limits<float>::max()); 
    }
}


