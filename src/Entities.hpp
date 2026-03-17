#include <iostream>
#include <vector>
#include "constants.hpp"
#pragma once

/*
Entity class, used to
*/
class Entity {
public:
	Entity(const Vec3& origin, float mass);				//entity constructor definition
	//entity member function definitions
	Vec3 grav_Effect(std::vector<float>& position);		
	Vec3 get_pos();

private:
	Vec3 origin;
	float mass;
	int aerosurface;  //aerosurface cannot be defined as an integer. Needs a better implementation

};

