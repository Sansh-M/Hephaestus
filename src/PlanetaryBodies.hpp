#include <iostream>
#include <vector>
#include "constants.hpp"
#pragma once
/*
Class used to define planetary bodies.
*/
class PlanetaryBodies {
public:
	PlanetaryBodies(const Vec3& origin, float mass, float radius, float atmospheric_density);

private:
	Vec3 origin;
	float mass;
	float radius;
	float atmospheric_density;
};