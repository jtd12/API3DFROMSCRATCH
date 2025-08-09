#include "objloader.hpp"


objloader::objloader()
{

}
objloader::~objloader()
{

}

bool objloader::load(const std::string& path, std::vector<Triangle>& triangles) {
	
		 std::map<std::string, Material> materials;
	 std::string currentMaterial;
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Erreur : Impossible d'ouvrir " << path << std::endl;
        return false;
    }

    std::vector<vector3d> vertices;
     std::vector<vector3d> normals;   // Liste des normales
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
        
        else if (type == "mtllib") {  // Charger le fichier de matériaux
    std::string mtlFilename;
    ss >> mtlFilename;
    loadMaterials("data/" + mtlFilename, materials);
} else if (type == "usemtl") {  // Assigner un matériau aux faces suivantes
    ss >> currentMaterial;
} 
      
    
    else if (type == "f") {
    if (faceCount++ > 1900) break;
    int v1, v2, v3, v4, n1, n2, n3, n4;
    char slash;  // Pour ignorer les slashes

    if (line.find("//") != std::string::npos) {
        // Si on a un quad avec des normales (format v//n), alors il y aura 4 sommets et 4 normales.
       sscanf(line.c_str(), "f %d//%d %d//%d %d//%d %d//%d", &v1, &n1, &v2, &n2, &v3, &n3, &v4, &n4);
           
           

        // Diviser le quad en deux triangles et ajouter chaque triangle
        triangles.push_back({v1 - 1, v2 - 1, v3 - 1, 
        vertices[v1 - 1], vertices[v2 - 1], vertices[v3 - 1],
    normals[n1 - 1], normals[n2 - 1], normals[n3 - 1],
    materials[currentMaterial] 
        });
        
        triangles.push_back({v1 - 1, v2 - 1, v3 - 1, 
            vertices[v1 - 1], vertices[v2 - 1], vertices[v3 - 1],
    normals[n1 - 1], normals[n2 - 1], normals[n3 - 1],
    materials[currentMaterial]       // Normales du Triangle 2
        });
    } else {
        // Cas où les normales sont dans un format "f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3"
      sscanf(line.c_str(), "f %d//%d %d//%d %d//%d", &v1, &n1, &v2, &n2, &v3, &n3);
        
        // Ajouter le triangle avec les sommets et les normales
        triangles.push_back({v1 - 1, v2 - 1, v3 - 1, 
    vertices[v1 - 1], vertices[v2 - 1], vertices[v3 - 1],
    normals[n1 - 1], normals[n2 - 1], normals[n3 - 1],
    materials[currentMaterial]  // Associer le matériau
});
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
        } else if (type == "map_Kd") {  // Texture
            ss >> materials[currentMaterial].texturePath;
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





