#include"racetracks.hpp"

raceTrackTrack::raceTrackTrack()
{
	
}

raceTrackTrack::~raceTrackTrack()
{
	
}


bool raceTrackTrack::pointInTriangle(const vector3d& p, const vector3d& a, const vector3d& b, const vector3d& c) {
    vector3d v0 = c - a;
    vector3d v1 = b - a;
    vector3d v2 = p - a;

    float dot00 = v0.dotproduct(v0);
    float dot01 = v0.dotproduct(v1);
    float dot02 = v0.dotproduct(v2);
    float dot11 = v1.dotproduct(v1);
    float dot12 = v1.dotproduct(v2);

    float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
    float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    return (u >= 0) && (v >= 0) && (u + v < 1);
}

bool raceTrackTrack::pointInQuad(const vector3d& p, const vector3d& a, const vector3d& b, const vector3d& c, const vector3d& d) {
    return pointInTriangle(p, a, b, c) || pointInTriangle(p, a, c, d);
}


bool raceTrackTrack::isInsideTrack(const vector3d& pos, const std::vector<std::pair<vector3d, vector3d>>& trackEdges) {
    for (size_t i = 0; i < trackEdges.size() - 1; i++) {
        vector3d left = trackEdges[i].first;
        vector3d right = trackEdges[i].second;
        vector3d nextLeft = trackEdges[i+1].first;
        vector3d nextRight = trackEdges[i+1].second;

        // Former un quadrilatère (left, right, nextRight, nextLeft)
        // et tester si pos est dedans
        if (pointInQuad(pos, left, right, nextRight, nextLeft)) {
            return true;
        }
    }
    return false;
}

vector3d raceTrackTrack::getClosestWaypoint(const vector3d& carPos, const std::vector<vector3d>& waypoints) {
    float minDist = std::numeric_limits<float>::max();
    vector3d closest = waypoints[0];

    for (const auto& wp : waypoints) {
        float dist = (carPos - wp).length();
        if (dist < minDist) {
            minDist = dist;
            closest = wp;
        }
    }
    return closest;
}

vector3d raceTrackTrack::closestPointOnSegment(const vector3d& p, const vector3d& a, const vector3d& b) {
    vector3d ab = b - a;
    float t = (p - a).dotproduct(ab) / ab.dotproduct(ab);
    t = std::max(0.0f, std::min(1.0f, t));
    return a + ab * t;
}

vector3d raceTrackTrack::getClosestPointOnTrack(const vector3d& pos, const std::vector<std::pair<vector3d, vector3d>>& trackEdges) {
    float bestDist = 1e9f;
    vector3d bestPoint;

    for (size_t i = 0; i < trackEdges.size(); i++) {
        vector3d left = trackEdges[i].first;
        vector3d right = trackEdges[i].second;

        // Trouver point le plus proche sur la ligne [left, right]
        vector3d closest = closestPointOnSegment(pos, left, right);
        float dist = (pos - closest).length();

        if (dist < bestDist) {
            bestDist = dist;
            bestPoint = closest;
        }
    }
    return bestPoint;
}


void raceTrackTrack::initializeTrack(const std::vector<vector3d>& controlPoints, int segmentsPerCurve, float width,height& heightmap) {
    std::vector<vector3d> path = generateTrackPath(controlPoints, segmentsPerCurve);
    trackEdges = generateTrackEdges(path, 3400.0f, heightmap, TERRAINHEIGHT, 500.0f); // Stocker les bords du circuit
    trackEdgesElevated = generateTrackEdgesWithElevation(path, 3400, 400,heightmap,TERRAINHEIGHT,TERRAINSIZE,-1000);
}

vector3d raceTrackTrack::catmullRom(const vector3d& p0, const vector3d& p1, const vector3d& p2, const vector3d& p3, float t) {
    float t2 = t * t;
    float t3 = t2 * t;
    
    return p1 * (2.0f * t3 - 3.0f * t2 + 1.0f) +
           p2 * (-2.0f * t3 + 3.0f * t2) +
           (p2 - p0) * (t3 - 2.0f * t2 + t) +
           (p3 - p1) * (t3 - t2);
}

std::vector<vector3d> raceTrackTrack:: generateTrackPath(const std::vector<vector3d>& controlPoints, int segmentsPerCurve) {
    std::vector<vector3d> path;
    
        if (controlPoints.size() < 4) {
        std::cerr << "Pas assez de points de contrôle pour générer le chemin." << std::endl;
        return path;  // Retourner un vecteur vide si le nombre de points est insuffisant
    }

    for (size_t i = 0; i < controlPoints.size() - 3; i++) {
        for (int j = 0; j <= segmentsPerCurve; j++) {
            float t = (float)j / segmentsPerCurve;
            path.push_back(catmullRom(controlPoints[i], controlPoints[i+1], controlPoints[i+2], controlPoints[i+3], t));
        }
    }
    
    return path;
}



std::vector<std::pair<vector3d, vector3d>> raceTrackTrack::generateTrackEdges(const std::vector<vector3d>& path, float width, height& heightmap, float size, float h) {
    std::vector<std::pair<vector3d, vector3d>> edges;
    

    for (size_t i = 0; i < path.size() - 1; i++) {
        vector3d tangent = (path[i+1] - path[i]).normalize();
        vector3d normal = { -tangent.z, 0, tangent.x };  // perpendiculaire au sol

        // Calcul des bords gauche et droit
        vector3d leftEdge = path[i] + normal * (width * 0.5f);
        vector3d rightEdge = path[i] - normal * (width * 0.5f);

        // Ajuster la hauteur avec la heightmap
        leftEdge.y  = heightmap.getHeightAt(leftEdge.x, leftEdge.z, size, h)  - 100.0f;
        rightEdge.y = heightmap.getHeightAt(rightEdge.x, rightEdge.z, size, h) - 100.0f;

        edges.emplace_back(leftEdge, rightEdge);
    }

    return edges;
}

std::vector<std::tuple<vector3d, vector3d, vector3d, vector3d>> 
raceTrackTrack::generateTrackEdgesWithElevation(const std::vector<vector3d>& path, float width, float elevation,
height& heightmap,
    float size,
    float h,
    float terrainOffset) {
    std::vector<std::tuple<vector3d, vector3d, vector3d, vector3d>> edges;

    for (size_t i = 0; i < path.size() - 1; i++) {
        vector3d tangent = (path[i + 1] - path[i]).normalize();
        vector3d normal = { -tangent.z, 0, tangent.x };  // Perpendiculaire au sol

        vector3d leftEdge = path[i] + normal * (width * 0.5f);
        vector3d rightEdge = path[i] - normal * (width * 0.5f);
        
        leftEdge.y  = heightmap.getHeightAt(leftEdge.x, leftEdge.z, size, h) + terrainOffset;
        rightEdge.y = heightmap.getHeightAt(rightEdge.x, rightEdge.z, size, h) + terrainOffset;
        
        vector3d leftElevated = leftEdge + vector3d(0, elevation, 0);
        vector3d rightElevated = rightEdge + vector3d(0, elevation, 0);

        edges.emplace_back(leftEdge, rightEdge, leftElevated, rightElevated);
    }

    return edges;
}

std::vector<std::vector<vector3d>> raceTrackTrack::generateTrackBorders(const std::vector<std::tuple<vector3d, vector3d, vector3d, vector3d>>& edges,height& heightmap
,float size, float h, float terrainOffset) {
    std::vector<std::vector<vector3d>> borderTriangles;

    size_t gap = 40; // nombre de segments sans bordure à la fin

    // On boucle seulement jusqu'à edges.size() - 1 - gap
    for (size_t i = 0; i < edges.size() - 1 - gap; i++) {
        size_t next = i + 1;

        vector3d leftEdge = std::get<0>(edges[i]);
        vector3d rightEdge = std::get<1>(edges[i]);
        vector3d leftElevated = std::get<2>(edges[i]);
        vector3d rightElevated = std::get<3>(edges[i]);

        vector3d leftEdgeNext = std::get<0>(edges[next]);
        vector3d rightEdgeNext = std::get<1>(edges[next]);
        vector3d leftElevatedNext = std::get<2>(edges[next]);
        vector3d rightElevatedNext = std::get<3>(edges[next]);
        
        


        // Optionnel : vérifier l'angle entre segments
        vector3d dirCurrent = (leftEdgeNext - leftEdge).normalize();
        vector3d dirPrevious = (leftEdge - std::get<0>(edges[(i + edges.size() - 1) % edges.size()])).normalize();

        float dp = dirCurrent.dotproduct(dirPrevious);
        if (dp < -1.0f) dp = -1.0f;
        if (dp > 1.0f) dp = 1.0f;
        float angle = acos(dp);
        if (angle > M_PI / 4) {
            continue;
        }
        
      leftEdge.y  = heightmap.getHeightAt(leftEdge.x, leftEdge.z, size, h) + terrainOffset;
        rightEdge.y = heightmap.getHeightAt(rightEdge.x, rightEdge.z, size, h) + terrainOffset;
        leftEdgeNext.y  = heightmap.getHeightAt(leftEdgeNext.x, leftEdgeNext.z, size, h) + terrainOffset;
        rightEdgeNext.y = heightmap.getHeightAt(rightEdgeNext.x, rightEdgeNext.z, size, h) + terrainOffset;
        
        leftElevated = leftEdge + vector3d(0, 400, 0);   // ou l'épaisseur de bordure que tu veux
		rightElevated = rightEdge + vector3d(0, 400, 0);
		leftElevatedNext = leftEdgeNext + vector3d(0, 400, 0);
		rightElevatedNext = rightEdgeNext + vector3d(0, 400, 0);

        // Générer les bordures latérales uniquement
        borderTriangles.push_back({ leftEdge, leftElevated, leftEdgeNext });
        borderTriangles.push_back({ leftElevated, leftElevatedNext, leftEdgeNext });

        borderTriangles.push_back({ rightEdge, rightEdgeNext, rightElevated });
        borderTriangles.push_back({ rightElevated, rightEdgeNext, rightElevatedNext });
        
 
    }

    return borderTriangles;

}


	std::vector<vector3d> raceTrackTrack::generateWaypoints(const std::vector<vector3d>& controlPoints, float spacing) {
    std::vector<vector3d> waypoints;

    for (size_t i = 0; i < controlPoints.size() - 1; ++i) {
        vector3d p1 = controlPoints[i];
        vector3d p2 = controlPoints[i + 1];
        vector3d segment = p2 - p1;
        float length = segment.length();
        int numSteps = static_cast<int>(length / spacing);
        vector3d dir = segment.normalize();

        for (int j = 0; j <= numSteps; ++j) {
            float t = j * spacing;
            vector3d wp = p1 + dir * t;
            waypoints.push_back(wp);
        }
    }

    return waypoints;
}

std::vector<std::vector<vector3d>> raceTrackTrack::generateTrackMesh(const std::vector<std::pair<vector3d, vector3d>>& edges, float elevation) {
    std::vector<std::vector<vector3d>> triangles;

  for (size_t i = 0; i < edges.size() - 1; i++) {
        vector3d leftEdge = edges[i].first;
        vector3d rightEdge = edges[i].second;
        vector3d leftEdgeNext = edges[i + 1].first;
        vector3d rightEdgeNext = edges[i + 1].second;

        // Élever les bords
        vector3d leftElevated = leftEdge + vector3d(0, elevation, 0);
        vector3d rightElevated = rightEdge + vector3d(0, elevation, 0);
        vector3d leftElevatedNext = leftEdgeNext + vector3d(0, elevation, 0);
        vector3d rightElevatedNext = rightEdgeNext + vector3d(0, elevation, 0);

        // Triangles de la route (mais pas au début et à la fin)
        if (i != 0 && i != edges.size() - 2) {
            // Triangles du sol de la route
            triangles.push_back({ leftEdge, rightEdge, leftEdgeNext });
            triangles.push_back({ rightEdge, rightEdgeNext, leftEdgeNext });
        }

        // Triangles des bordures gauche et droite (pas au début et à la fin)
        if (i != 0 && i != edges.size() - 2) {
            triangles.push_back({ leftEdge, leftElevated, leftEdgeNext });
            triangles.push_back({ leftElevated, leftElevatedNext, leftEdgeNext });

            triangles.push_back({ rightEdge, rightEdgeNext, rightElevated });
            triangles.push_back({ rightElevated, rightEdgeNext, rightElevatedNext });
        }

        // Triangles pour les murs latéraux
        if (i != 0 && i != edges.size() - 2) {
            triangles.push_back({ leftElevated, rightElevated, leftElevatedNext });
            triangles.push_back({ rightElevated, rightElevatedNext, leftElevatedNext });
        }
    }

    return triangles;
}

void raceTrackTrack::drawTriangle(std::vector<Triangle>& allTriangles,pixel* p,Uint32* framebuffer,float* framebufferDepth,int screenWidth, int screenHeight,const vector3d& v1, const vector3d& v2, const vector3d& v3,const Camera& camera,
bool isTrack,bool isBorder) {
	
    Matrix4x4 viewProjectionMatrix = camera.getProjectionMatrix() * camera.getViewMatrix(camera,1);
    Matrix4x4 raceTransform = translationMatrix * rotationMatrixX * scaleMatrix;
    Matrix4x4 finalMatrix = viewProjectionMatrix * raceTransform;

    // Appliquer la transformation aux sommets
    vector3d tV1 = finalMatrix.apply(v1);
    vector3d tV2 = finalMatrix.apply(v2);
    vector3d tV3 = finalMatrix.apply(v3);

    // Projeter les sommets sur l'écran
    Point2D p1 = p->project(tV1, screenWidth, screenHeight);
    Point2D p2 = p->project(tV2, screenWidth, screenHeight);
    Point2D p3 = p->project(tV3, screenWidth, screenHeight);

       // Vérification des limites écran
        if (p1.x < .0 || p1.x >= 800 -5.1 || p1.y < .0 || p1.y >= 600-5.1 ||
            p2.x < .0 || p2.x >= 800 -5.1|| p2.y < .0 || p2.y >= 600-5.1 ||
            p3.x < .0 || p3.x >= 800-5.1 || p3.y < .0 || p3.y >= 600-5.1) {
            return;
        }


    // Calculer la normale avec les points transformés
    vector3d normal = (tV3 - tV1).crossproduct(tV2 - tV1).normalize();
    vector3d lightDir = vector3d(0.0f, 100.0f, 15.0f).normalize();
    float intensity = std::max(0.0f, normal.dotproduct(lightDir));

    Uint8 r, g, b;
    if (isTrack) {
        r = std::min(255, static_cast<int>(80 * intensity));  // Gris foncé
        g = std::min(255, static_cast<int>(80 * intensity));
        b = std::min(255, static_cast<int>(80 * intensity));
    }
	else if (isBorder) {
	    r = 255*intensity; g = 50*intensity; b = 50*intensity;  // Couleur rouge foncé pour les bordures
	}
	 else {
        r = std::min(255, static_cast<int>(50 * intensity));  // Marron / Vert
        g = std::min(255, static_cast<int>(150 * intensity));
        b = std::min(255, static_cast<int>(50 * intensity));
    }
    // Dessiner le triangle
    p->fillTriangle(framebuffer, framebufferDepth, 
                    { p1.x, screenHeight - p1.y }, 
                    { p2.x, screenHeight - p2.y }, 
                    { p3.x, screenHeight - p3.y },
                    screenWidth, screenHeight, r, g, b, 
                    tV1.z, tV2.z, tV3.z);

}

bool  raceTrackTrack::isPointOnTrack(float x, float z) {
if (trackEdges.empty()) return false;

    vector3d p(x, 0, z);

    for (size_t i = 0; i < trackEdges.size() - 1; i++) {
        vector3d left = trackEdges[i].first;
        vector3d right = trackEdges[i].second;

        // Segment route
        vector3d seg = right - left;
        vector3d toP = p - left;

        // Projection scalaire [0,1] de p sur le segment
        float t = toP.dotproduct(seg) / seg.dotproduct(seg);
        t = std::max(0.0f, std::min(1.0f, t));

        // Point projeté
        vector3d proj = left + seg * t;

        // Distance entre p et ce point projeté
        float dist = (p - proj).length();

        if (dist < (seg.length() * 0.5f)) {
            return true; // le point est dans la largeur de la piste
        }
    }

    return false;
    }

