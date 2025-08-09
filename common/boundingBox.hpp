#ifndef DEF_BOUNDINGBOX_HPP
#define DEF_BOUNDINGBOX_HPP
#include<stdio.h>
#include<iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>
#include <chrono>
#include <thread>
#include <conio.h> // Pour _kbhit() et _getch()
#include <algorithm>
#include <cfloat>
#include<cmath>
#include <map>
#define M_PI    3.14159265358979323846
#include <cstdlib>
#include <algorithm>  // Pour std::swap
#include"vector.h"

class AABB {
    vector3d min;  // Coin inférieur gauche de la boîte
    vector3d max;  // Coin supérieur droit de la boîte
    
    public:
    	AABB();
		~AABB();
		void update(const vector3d& position);
		vector3d getMin() const;
		vector3d getMax() const;
    // Méthode pour tester si un point est dans la boîte
    bool isPointInside(const vector3d& point) const;
};



#endif
