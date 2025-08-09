#include"collisionDetection.hpp"

	collisions::collisions()
	{
		
	}
	
	collisions::~collisions()
	{
		
	}
	
bool collisions::pointInTriangle(const vector3d& P, const vector3d& A, const vector3d& B, const vector3d& C) {
    vector3d v0 = C - A;
    vector3d v1 = B - A;
    vector3d v2 = P - A;

    float dot00 = v0.dotproduct(v0);
    float dot01 = v0.dotproduct(v1);
    float dot02 = v0.dotproduct(v2);
    float dot11 = v1.dotproduct(v1);
    float dot12 = v1.dotproduct(v2);

    float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
    float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    return (u >= 0) && (v >= 0) && (u + v <= 1);
}

bool collisions::aabbIntersectsTriangle(const AABB& box, const vector3d& v0, const vector3d& v1, const vector3d& v2) {
    // Vérifie si un triangle touche une AABB
    for (int i = 0; i < 3; i++) {
        float minCoord, maxCoord;
        if (i == 0) { // Axe X
            minCoord = std::min({ v0.x, v1.x, v2.x });
            maxCoord = std::max({ v0.x, v1.x, v2.x });
        } else if (i == 1) { // Axe Y
            minCoord = std::min({ v0.y, v1.y, v2.y });
            maxCoord = std::max({ v0.y, v1.y, v2.y });
        } else { // Axe Z
            minCoord = std::min({ v0.z, v1.z, v2.z });
            maxCoord = std::max({ v0.z, v1.z, v2.z });
        }

        if (box.getMax()[i] < minCoord || box.getMin()[i] > maxCoord) {
            return false;
        }
    }

    return true;
}
