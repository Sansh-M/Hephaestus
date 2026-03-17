#include <iostream>
#include <vector>
#include "constants.hpp"
#include "PlanetaryBodies.hpp"


PlanetaryBodies::PlanetaryBodies(const Vec3& origin, float mass, float radius, float atmospheric_density)	//def of constructor for PlanetaryBodies
	: origin(origin), mass(mass) {}

