#include <iostream>
#include <vector>
#include "constants.hpp"
#pragma once

/*
Entity class where Entity can beanything 
*/
class Entity {
public:
	Entity(const Vec3& origin, float mass);				//entity constructor definition
	//entity member function definitions
	Vec3 grav_Effect(std::vector<float>& position);		
	Vec3 get_pos() { return pos; }
	void set_pos(Vec3 position) { pos = position; }
	float get_mass() const { return mass; }
	void set_planetaryVelocity(Vec3 planet_vel) { planetaryVelocity = planet_vel;}
	Vec3 get_planetaryVelocity() { return planetaryVelocity; }
	void set_velocity(Vec3 vel) { Velocity = vel; }
	Vec3 get_velocity() { return Velocity; }
private:
	Vec3 pos;
	Vec3 Velocity;
	Vec3 origin;
	Vec3 planetaryVelocity;
	float mass;
	int aerosurface;  //aerosurface cannot be defined as an integer. Needs a better implementation later

};

