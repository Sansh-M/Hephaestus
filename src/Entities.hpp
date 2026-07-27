#include <iostream>
#include <vector>
#include "constants.hpp"
#pragma once

/*
Entity class where Entity can beanything 
*/
class Entity {
public:
	Entity(const Vec3& initial_position, float entity_mass)
		: pos(initial_position),
		  Velocity{ 0.0f, 0.0f, 0.0f },
		  origin(initial_position),
		  planetaryVelocity{ 0.0f, 0.0f, 0.0f },
		  mass(entity_mass),
		  aerosurface(0) {
	}
	//entity member function definitions
	Vec3 grav_Effect(std::vector<float>& position);		
	Vec3 get_pos() const { return pos; }
	void set_pos(const Vec3& position) { pos = position; }
	float get_mass() const { return mass; }
	void set_planetaryVelocity(const Vec3& planet_vel) { planetaryVelocity = planet_vel;}
	Vec3 get_planetaryVelocity() const { return planetaryVelocity; }
	void set_velocity(const Vec3& vel) { Velocity = vel; }
	Vec3 get_velocity() const { return Velocity; }
private:
	Vec3 pos;
	Vec3 Velocity;
	Vec3 origin;
	Vec3 planetaryVelocity;
	float mass;
	int aerosurface;  //aerosurface cannot be defined as an integer. Needs a better implementation later

};

