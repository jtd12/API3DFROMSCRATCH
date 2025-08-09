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
    vector3d min;  // Coin inf�rieur gauche de la bo�te
    vector3d max;  // Coin sup�rieur droit de la bo�te
    
    public:
    	AABB();
		~AABB();
		void update(const vector3d& position);
		vector3d getMin() const;
		vector3d getMax() const;
    // M�thode pour tester si un point est dans la bo�te
    bool isPointInside(const vector3d& point) const;
};



#endif
