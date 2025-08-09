#include "objloader.hpp"


//SDL_Surface* textureSurface=nullptr;

objloader::objloader()
{

}
objloader::~objloader()
{

}


bool objloader::load(const std::string& path,std::vector<Triangle>& triangles) {
	
	 std::map<std::string, Material> materials;
	 std::string currentMaterial;
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Erreur : Impossible d'ouvrir " << path << std::endl;
        return false;
    }

    std::vector<vector3d> vertices;
     std::vector<vector3d> normals;   // Liste des normales
     std::vector<vector2d> texcoords; // À créer
    std::string line;
int faceCount = 0;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string type;
        ss >> type;

        if (type == "v") {
            vector3d v;
            ss >> v.x >> v.y >> v.z;
            vertices.push_back(v);
        }
         else if (type == "vn") {  // Normales
            vector3d n;
            ss >> n.x >> n.y >> n.z;
            normals.push_back(n);
        }

		else if (type == "vt") {
		    vector2d uv;
		    ss >> uv.x >> uv.y;
		    texcoords.push_back(uv);
		}
        else if (type == "mtllib") {  // Charger le fichier de matériaux
    std::string mtlFilename;
    ss >> mtlFilename;
    loadMaterials("data/" + mtlFilename, materials);
} else if (type == "usemtl") {  // Assigner un matériau aux faces suivantes
    ss >> currentMaterial;
} 


else if (type == "f") {
    if (faceCount++ > 3000) break;

    int v[4], t[4], n[4];
    int matches = 0;

    if (line.find("//") != std::string::npos) {
        // Format: v//n (sans UV)
        matches = sscanf(line.c_str(), "f %d//%d %d//%d %d//%d %d//%d",
                         &v[0], &n[0], &v[1], &n[1], &v[2], &n[2], &v[3], &n[3]);

        if (matches == 6) {
            // Triangle sans UV
            triangles.push_back(Triangle(
                v[0] - 1, v[1] - 1, v[2] - 1,
                vertices[v[0] - 1], vertices[v[1] - 1], vertices[v[2] - 1],
                normals[n[0] - 1], normals[n[1] - 1], normals[n[2] - 1],
                materials[currentMaterial]
            ));
        } else if (matches == 8) {
            // Quad sans UV -> 2 triangles
            triangles.push_back(Triangle(
                v[0] - 1, v[1] - 1, v[2] - 1,
                vertices[v[0] - 1], vertices[v[1] - 1], vertices[v[2] - 1],
                normals[n[0] - 1], normals[n[1] - 1], normals[n[2] - 1],
                materials[currentMaterial]
            ));
            triangles.push_back(Triangle(
                v[0] - 1, v[2] - 1, v[3] - 1,
                vertices[v[0] - 1], vertices[v[2] - 1], vertices[v[3] - 1],
                normals[n[0] - 1], normals[n[2] - 1], normals[n[3] - 1],
                materials[currentMaterial]
            ));
        }
    } else {
        // Format: v/t/n (avec UV)
        matches = sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",
                         &v[0], &t[0], &n[0], &v[1], &t[1], &n[1], &v[2], &t[2], &n[2], &v[3], &t[3], &n[3]);

        if (matches == 9) {
            // Triangle avec UV
            triangles.push_back(Triangle(
                v[0] - 1, v[1] - 1, v[2] - 1,
                vertices[v[0] - 1], vertices[v[1] - 1], vertices[v[2] - 1],
                normals[n[0] - 1], normals[n[1] - 1], normals[n[2] - 1],
                materials[currentMaterial],
                texcoords[t[0] - 1], texcoords[t[1] - 1], texcoords[t[2] - 1]
            ));
        } else if (matches == 12) {
            // Quad avec UV
            triangles.push_back(Triangle(
                v[0] - 1, v[1] - 1, v[2] - 1,
                vertices[v[0] - 1], vertices[v[1] - 1], vertices[v[2] - 1],
                normals[n[0] - 1], normals[n[1] - 1], normals[n[2] - 1],
                materials[currentMaterial],
                texcoords[t[0] - 1], texcoords[t[1] - 1], texcoords[t[2] - 1]
            ));
            triangles.push_back(Triangle(
                v[0] - 1, v[2] - 1, v[3] - 1,
                vertices[v[0] - 1], vertices[v[2] - 1], vertices[v[3] - 1],
                normals[n[0] - 1], normals[n[2] - 1], normals[n[3] - 1],
                materials[currentMaterial],
                texcoords[t[0] - 1], texcoords[t[2] - 1], texcoords[t[3] - 1]
            ));
        }
    }

}
}



    file.close();
    return true;
}




bool objloader::loadMaterials(const std::string& mtlPath, std::map<std::string, Material>& materials) {
	
    std::ifstream file(mtlPath);
    if (!file.is_open()) {
        std::cerr << "Erreur : Impossible d'ouvrir " << mtlPath << std::endl;
        return false;
    }

    std::string line, currentMaterial;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string type;
        ss >> type;

        if (type == "newmtl") {
            ss >> currentMaterial;
            materials[currentMaterial] = Material{currentMaterial};
        } else if (type == "Kd") {  // Couleur diffuse
            float r, g, b;
            ss >> r >> g >> b;
            materials[currentMaterial].diffuseColor = {r, g, b};
        } else if (type == "map_Kd") {
    ss >> materials[currentMaterial].texturePath;

    std::string fullPath = "data/" + materials[currentMaterial].texturePath;
    SDL_Surface* surface = SDL_LoadBMP(fullPath.c_str());
    if (!surface) {
        std::cerr << "Erreur chargement texture : " << fullPath << " -> " << SDL_GetError() << std::endl;
    } else {
                // Conversion en ARGB8888
                SDL_Surface* formatted = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
                SDL_FreeSurface(surface); // On libère l’originale

                if (!formatted) {
                    std::cerr << "Erreur conversion format : " << fullPath << " -> " << SDL_GetError() << std::endl;
                } else {
                    materials[currentMaterial].textureSurface = formatted;
                }
		}
	}
}
    file.close();
    return true;
}



void objloader::smoothNormals(std::vector<Triangle>& triangles, std::vector<vector3d>& vertexNormals, int numVertices)
{
    std::vector<vector3d> normalSums(numVertices, vector3d(0.0f, 0.0f, 0.0f));  // Vecteur de sommes
    std::vector<int> normalCounts(numVertices, 0);  // Vecteur de comptages

    // Parcourir les triangles pour accumuler les normales
    for (const auto& tri : triangles) {
        normalSums[tri.index1] += tri.n1;
        normalSums[tri.index2] += tri.n2;
        normalSums[tri.index3] += tri.n3;

        normalCounts[tri.index1]++;
        normalCounts[tri.index2]++;
        normalCounts[tri.index3]++;
    }

    // Calculer les normales moyennes
    for (int i = 0; i < numVertices; ++i) {
        if (normalCounts[i] > 0) {
            vector3d avgNormal = normalSums[i] / static_cast<float>(normalCounts[i]);
            vertexNormals.push_back(avgNormal);
        } else {
            // Si un sommet n'a pas de normales, on l'ignore ou on lui assigne une normale par défaut
            vertexNormals.push_back(vector3d(0.0f, 1.0f, 0.0f));  // Exemple : normale par défaut
        }
    }
}




/*

void objloader::draw(SDL_Renderer* renderer, int screenWidth, int screenHeight, const Camera& camera,Matrix4x4 translationMatrix, Matrix4x4 scaleMatrix, Matrix4x4 rotationMatrixX, 
Matrix4x4 rotationMatrixY,Matrix4x4 rotationMatrixZ,std::vector<Triangle> triangles) {
	
	getPixel()->clearBuffer();

    Matrix4x4 carTransform = translationMatrix * rotationMatrixX * rotationMatrixY * rotationMatrixZ * scaleMatrix;
    
    Matrix4x4 projectionMatrix = camera.getProjectionMatrix(); // Obtenir la matrice de projection
    Matrix4x4 viewMatrix=camera.getViewMatrix(camera);
    Matrix4x4 viewProjectionMatrix = projectionMatrix * viewMatrix * carTransform;
    
    std::vector<Triangle> carTriangles;
    carTriangles.reserve(triangles.size());

    for (auto& tri : triangles) {
        Triangle transformedTri = tri;
        transformedTri.v1 = viewProjectionMatrix.apply(tri.v1);
        transformedTri.v2 = viewProjectionMatrix.apply(tri.v2);
        transformedTri.v3 = viewProjectionMatrix.apply(tri.v3);
        transformedTri.avgDepth = (transformedTri.v1.z + transformedTri.v2.z + transformedTri.v3.z) / 3.0f;
        
        carTriangles.push_back(transformedTri);
    }

    // ?? Tri des triangles du plus éloigné au plus proche (Z-sorting)
    std::sort(carTriangles.begin(), carTriangles.end(), [](const Triangle& t1, const Triangle& t2) {
        return t1.avgDepth > t2.avgDepth;
    });

    // ?? Affichage des triangles triés
    for (auto& tri : carTriangles) {
        SDL_Point p1 = getPixel()->project(tri.v1, screenWidth, screenHeight);
        SDL_Point p2 = getPixel()->project(tri.v2, screenWidth, screenHeight);
        SDL_Point p3 = getPixel()->project(tri.v3, screenWidth, screenHeight);

        if (p1.x < 0 || p1.x >= screenWidth || p1.y < 0 || p1.y >= screenHeight ||
            p2.x < 0 || p2.x >= screenWidth || p2.y < 0 || p2.y >= screenHeight ||
            p3.x < 0 || p3.x >= screenWidth || p3.y < 0 || p3.y >= screenHeight) {
            continue;
        }

        vector3d edge1 = tri.v3 - tri.v1;
        vector3d edge2 = tri.v2 - tri.v1;
        vector3d normal = edge1.crossproduct(edge2).normalize();
        vector3d lightDir = vector3d(0.0f, -1.0f, 1000.0f).normalize();
        float intensity = std::max(0.0f, normal.dotproduct(lightDir));
        Uint8 r = std::min(255, static_cast<int>(tri.material.diffuseColor.x * intensity * 255));
        Uint8 g = std::min(255, static_cast<int>(tri.material.diffuseColor.y * intensity * 255));
        Uint8 b = std::min(255, static_cast<int>(tri.material.diffuseColor.z * intensity * 255));

        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        getPixel()->fillTriangle(renderer, {p1.x, screenHeight - p1.y}, 
                                 {p2.x, screenHeight - p2.y}, 
                                 {p3.x, screenHeight - p3.y}, 
                                 screenWidth, screenHeight, tri.v1.z, tri.v2.z, tri.v3.z);
    }
}

*/





