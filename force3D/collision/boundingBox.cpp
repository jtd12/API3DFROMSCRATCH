#include"boundingBox.hpp"

AABB::AABB()
{
	
}

AABB::~AABB()
{
	
}

  void AABB::update(const vector3d& position) {
        vector3d halfSize = vector3d(200.0f,200,200);
        min = position - halfSize;
        max = position + halfSize;
    }
    
bool AABB::isPointInsideAABB(const vector3d& point, const vector3d& min_, const vector3d& max_) {
    return (point.x >= min_.x && point.x <= max_.x) &&
           (point.y >= min_.y && point.y <= max_.y) &&
           (point.z >= min_.z && point.z <= max_.z);
}

std::vector<vector3d> AABB::getCorners() const {
    return {
        {min.x, min.y, min.z},
        {max.x, min.y, min.z},
        {max.x, max.y, min.z},
        {min.x, max.y, min.z},
        {min.x, min.y, max.z},
        {max.x, min.y, max.z},
        {max.x, max.y, max.z},
        {min.x, max.y, max.z}
    };
}


bool AABB::intersects(const AABB& other) const {
    return (min.x <= other.max.x && max.x >= other.min.x) &&
           (min.y <= other.max.y && max.y >= other.min.y) &&
           (min.z <= other.max.z && max.z >= other.min.z);
}


void AABB::set(const vector3d& min_, const vector3d& max_) {
    min = min_;
    max = max_;
}

	vector3d AABB::getMin() const
	{
		return min;
	}
	
	vector3d AABB::getMax() const
	{
			return max;
	}
