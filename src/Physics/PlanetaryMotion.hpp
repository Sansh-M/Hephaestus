#include <iostream>
#include <math.h>
using namespace std;

//*
// class containing functions for defining the motion of planetary bodies of the solar system around the sun. 
// 
class PlanetaryMotion {
public:


	float solveEccentricAnomaly(float M, float e) {
		float E = M;
		for int(i = 0; i < 6; i++) {
			float f = E - e * sin(E) - M;
			float fp = 1.0f - e * cos(E);
			E -= f / fp;
		}
		return E;
 	}


	//*
	// Function to update coordinate for next timestep for a planetary body. 
	void updatePosition(PlanetaryBody& planet) {
	
		//TODO: Implement the rest of the rotation matrix calculations (R12, R21, R22, R31, R32) based on the orbital parameters.

		float n = 2 * M_PI / planet.orbit_params.orbitalPeriod; // mean motion
		float mean_anomaly = planet.orbit_params.meanAnomaly0 + n * std::chrono::duration<float>(now - start).count(); // mean anomaly at time t
		float E = solveEccentricAnomaly(mean_anomaly, planet.orbit_params.eccentricity); // eccentric anomaly

		float x_P = planet.orbit_params.semiMajorAxis * (cos(E) - planet.orbit_params.eccentricity); // position in orbital plane)
		float y_Q = planet.orbit_params.semiMajorAxis * sqrt(1.0f - planet.orbit_params.eccentricity * planet.orbit_params.eccentricity) * sin(E); // position in orbital plane
		
		float x, y, z;
		x = planet.rotation_frame.R11 * x_P + planet.rotation_frame.R12 * y_Q; //computing the new position for x,y,z
		y = planet.rotation_frame.R21 * x_P + planet.rotation_frame.R22 * y_Q;
		z = planet.rotation_frame.R31 * x_P + planet.rotation_frame.R32 * y_Q;

		Vec3 newPos = { x, y, z };

		planet.setPos(newPos);
	}
	

}