#include <iostream>
#include <vector>
#pragma once
#include "PlanetaryBodies.hpp"
/*
set of functions related to gravitational interaction between entities
*/
namespace Gravity_Components {

}

/*
functions that are used for building components and the environment 
*/
namespace Build_Functions {
	
	void buildEntity() {

	}

	PlanetaryBodies buildPlanetaryBodies(Vec3 origin, float mass, float radius, float atmospheric_density) {
		PlanetaryBodies p = PlanetaryBodies(origin, mass, radius, atmospheric_density);
		return p;
	}
}