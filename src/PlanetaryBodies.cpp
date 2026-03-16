#include <iostream>
#include <vector>
#include "constants.hpp"

/*
Class used to define planetary bodies. 
*/
class PlanetaryBodies {
	public:
		PlanetaryBodies(const Vec3& origin, float mass, float radius, float atmospheric_density) 
			: origin(origin), mass(mass) {}
		
	private:	
		Vec3 origin;
		float mass;
		float radius; 
		float atmospheric_densit
};