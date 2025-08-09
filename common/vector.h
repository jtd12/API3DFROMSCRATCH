#ifndef VECTOR_H
#define VECTOR_H
#include <iostream>
#include <cmath>
#include <stdexcept>

class vector4d {
    
	
	public:
		float x, y, z, w;
    // Constructeur
    vector4d();
    vector4d(float x, float y, float z, float w);
    
     vector4d normalize() const;

    // Opérations de base
    vector4d operator+(const vector4d& other) const;
    vector4d operator-(const vector4d& other) const;

    vector4d operator*(float scalar) const ;

    // Ajouter d'autres opérations si nécessaire
};


class vector3d{
		public:
		float x,y,z;
		vector3d();
		vector3d(float a,float b);
		vector3d(float a,float b,float c);
		~vector3d(){};
		float operator[](int index) const;
		float dotproduct(const vector3d& vec2) const;
		vector3d crossproduct(const vector3d& vec2);
		float length();
		void change(float a,float b,float c);
		 vector3d cross(const vector3d& a, const vector3d& b)const;
		static vector3d normalise(const vector3d& v);
		float dot(const vector3d& a, const vector3d& b)const;
		void change(vector3d);
		void changeX(float a);
		void changeY(float b);
		void changeZ(float c);
		vector3d normalize();
		void applyProjection(float fov, float aspect, float near, float far) ;
		vector3d operator+(const vector3d& vec2) const;
		vector3d operator-(const vector3d& vec2)const;
		vector3d operator*(const float& num) const;
		vector3d operator/(const float& num);
		vector3d& operator+=(const vector3d& vec2);
		vector3d& operator-=(const vector3d& vec2);
		vector3d& operator*=(const float& num);
		vector3d& operator/=(const float& num);
		vector3d reflect(const vector3d& normal);
		bool operator<(const vector3d& a) const;
		bool operator==(const vector3d& vec2) const;
		bool operator!=(const vector3d& vec2);
		friend std::ostream& operator<<(std::ostream& out,vector3d vec2); 
};
#endif
