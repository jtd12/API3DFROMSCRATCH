#include "vector.h"


// Constructeur
    vector4d::vector4d() : x(0), y(0), z(0), w(1) {}
    vector4d::vector4d(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    
     vector4d vector4d:: normalize() const {
        float length = std::sqrt(x * x + y * y + z * z + w * w);
        return vector4d(x / length, y / length, z / length, w / length);
    }

    // Opérations de base
    vector4d vector4d::operator+(const vector4d& other) const {
        return vector4d(x + other.x, y + other.y, z + other.z, w + other.w);
    }

    vector4d vector4d::operator-(const vector4d& other) const {
        return vector4d(x - other.x, y - other.y, z - other.z, w - other.w);
    }

    vector4d vector4d::operator*(float scalar) const {
        return vector4d(x * scalar, y * scalar, z * scalar, w * scalar);
    }



vector3d::vector3d()
{
	x=y=z=0;
}

vector3d::vector3d(float a,float b)
{
	x=a;
	y=b;
	z=0;
}

vector3d::vector3d(float a,float b,float c)
{
	x=a;
	y=b;
	z=c;
}

float vector3d::operator[](int index) const {
        if (index == 0) return x;
        if (index == 1) return y;
        if (index == 2) return z;
        throw std::out_of_range("Index out of range for vector3d");
    }

float vector3d::dotproduct(const vector3d& vec2) const
{
	return (x*vec2.x+y*vec2.y+z*vec2.z);
}

vector3d vector3d::crossproduct(const vector3d& vec2)
{
	return (vector3d(y*vec2.z-z*vec2.y,x*vec2.z-z*vec2.x,x*vec2.y-y*vec2.x));
}

float vector3d::length()
{
	return (sqrt(x*x+y*y+z*z));
}

void vector3d::change(float a,float b,float c)
{
	x=a;
	y=b;
	z=c;
}

void vector3d::change(vector3d vec)
{
	x=vec.x;
	y=vec.y;
	z=vec.z;
}



void vector3d::changeX(float a)
{
	x=a;
}

void vector3d::changeY(float b)
{
	y=b;
}

void vector3d::changeZ(float c)
{
	z=c;
}


vector3d vector3d::normalize()
{
   float length = std::sqrt(x * x + y * y + z * z);
        return (length != 0) ? vector3d(x / length, y / length, z / length) : vector3d();
}

vector3d vector3d::operator+(const vector3d& vec2) const
{
	return (vector3d(x+vec2.x,y+vec2.y,z+vec2.z));
}

vector3d vector3d::operator-(const vector3d& vec2)const
{
	return (vector3d(x-vec2.x,y-vec2.y,z-vec2.z));
}

vector3d vector3d::operator*(const float& num) const
{
	return (vector3d(x*num,y*num,z*num));
}

vector3d vector3d::operator/(const float& num) {
    return vector3d(x / num, y / num, z / num);
}

vector3d& vector3d::operator+=(const vector3d& vec2)
{
	x+=vec2.x;
	y+=vec2.y;
	z+=vec2.z;
	return *this;
}

vector3d& vector3d::operator-=(const vector3d& vec2)
{
	x-=vec2.x;
	y-=vec2.y;
	z-=vec2.z;
	return *this;
}

vector3d& vector3d::operator*=(const float& num)
{
	x*=num;
	y*=num;
	z*=num;
	return *this;
}

vector3d& vector3d::operator/=(const float& num)
{
	if(num!=0)
	{
		x/=num;
		y/=num;
		z/=num;
	}
	return *this;
}

bool vector3d::operator==(const vector3d& vec2) const
{
	return (x==vec2.x && y==vec2.y && z==vec2.z);
}

bool vector3d::operator!=(const vector3d& vec2)
{
	return (x!=vec2.x && y!=vec2.y && z!=vec2.z);
}

std::ostream& operator<<(std::ostream& out,vector3d vec2)
{
	out << vec2.x << "\t" << vec2.y << "\t" << vec2.z << std::endl;
	return out;
}

vector3d  vector3d::cross(const vector3d& a, const vector3d& b) const{
    return vector3d(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

  vector3d vector3d::normalise(const vector3d& v) {
    float length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    if (length == 0) return vector3d(0, 0, 0);  // Évite la division par zéro
    return vector3d(v.x / length, v.y / length, v.z / length);
}

float  vector3d::dot(const vector3d& a, const vector3d& b) const {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

vector3d vector3d::reflect(const vector3d& normal)  {
    vector3d n = normal;  // Copie pour éviter les problèmes de `const`
    return *this - n * (2.0f * this->dotproduct(normal));
}

 bool vector3d::operator<(const vector3d& other) const {
        if (x != other.x) return x < other.x;
        if (y != other.y) return y < other.y;
        return z < other.z;
    }

