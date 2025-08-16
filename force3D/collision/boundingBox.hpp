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
#include <cstdlib>
#include <algorithm>  // Pour std::swap
#include"../maths/vector.h"

class AABB {
 	vector3d min;
	vector3d max;

    public:
    		AABB();
		~AABB();
		void update(const vector3d& position);
		vector3d getMin() const;
		vector3d getMax() const;
		void set(const vector3d& min_, const vector3d& max_);
    // Méthode pour tester si un point est dans la boîte
    bool isPointInsideAABB(const vector3d& point, const vector3d& min_, const vector3d& max_);
    bool intersects(const AABB& other) const;
    std::vector<vector3d> getCorners() const;
};



#endif
