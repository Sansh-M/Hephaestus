#include <iostream>
#include <math.h>
using namespace std;

//*
// class containing functions for defining the motion of planetary bodies of the solar system around the sun. 
// 
class PlanetaryMotion {
public:
	//*
	// Function to update coordinate for next timestep for a planetary body. 
	void updatePosition(PlanetaryBody& planet) {
		planet.rotation_frame.R11 = (cos(planet.orbit_params.raan) * cos(planet.orbit_params.argOfPeriapsis)) 
			- sin(planet.orbit_params.raan) * sin(planet.orbit_params.argOfPeriapsis) * cos(planet.orbit_params.inclination); // R11 computation 

		//TODO: Implement the rest of the rotation matrix calculations (R12, R21, R22, R31, R32) based on the orbital parameters.

		float x, y, z;
		x = planet.rotation_frame.R11 * planet.pos.x + planet.rotation_frame.R12 * planet.pos.y; 
		y = planet.rotation_frame.R21 * planet.pos.x + planet.rotation_frame.R22 * planet.pos.y;
		z = planet.rotation_frame.R31 * planet.pos.x + planet.rotation_frame.R32 * planet.pos.y;

		Vec3 newPos = { x, y, z };
		planet.setPos(newPos);
	}
	

}