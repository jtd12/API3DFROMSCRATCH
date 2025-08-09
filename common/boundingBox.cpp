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
    
bool AABB::isPointInside(const vector3d& point) const
{ 
        
return (point.x >= min.x && point.x <= max.x &&
                point.y >= min.y && point.y <= max.y &&
                point.z >= min.z && point.z <= max.z);
                
	
}

	vector3d AABB::getMin() const
	{
		return min;
	}
	
	vector3d AABB::getMax() const
	{
			return max;
	}
