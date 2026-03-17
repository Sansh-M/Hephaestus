#include <iostream>
#include <vector>
#include "constants.hpp"
#include "Entities.hpp"
/*
Entity class, used to 
*/

Entity::Entity(const Vec3& origin, float mass) 
	: origin(origin), mass(mass) {}

Vec3 Entity:: grav_Effect(std::vector<float>& position) {
		//TODO
	}	

Vec3 Entity::get_pos() {
	return origin;
	}

		
