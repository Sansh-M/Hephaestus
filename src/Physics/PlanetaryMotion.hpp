#include <iostream>
#include <math.h>
#include "PlanetaryBody.hpp"
#include "constants.hpp"
#include <vector>
using namespace std;
using namespace Physics;

//*
// class containing functions for defining the motion of planetary bodies of the solar system around the sun. 
// 
class PlanetaryMotion {
public:

	void updateAll(std::vector<PlanetaryBody>& planetary_bodies, float t) {
		for (auto& planet : planetary_bodies) {
			if (planet.orbit_params.orbitalPeriod == 0.0f) {
				continue;
			}
			updatePosition(planet, t);

		}
	}


	float solveEccentricAnomaly(float M, float e) {
		float E = M;
		for (int i = 0; i < 6; i++) {
			float f = E - e * sin(E) - M;
			float fp = 1.0f - e * cos(E);
			E -= f / fp;
		}
		return E;
	}


	//*
	// Function to update coordinate for next timestep for a planetary body. 
	void updatePosition(PlanetaryBody& planet, float t) {


		float n = 2 * PI / planet.orbit_params.orbitalPeriod; // mean motion
		float mean_anomaly = planet.orbit_params.meanAnomaly0 + n * t; // mean anomaly at time t
		float E = solveEccentricAnomaly(mean_anomaly, planet.orbit_params.eccentricity); // eccentric anomaly

		float x_P = planet.orbit_params.semiMajorAxis * (cos(E) - planet.orbit_params.eccentricity); // position in orbital plane)
		float y_Q = planet.orbit_params.semiMajorAxis * sqrt(1.0f - planet.orbit_params.eccentricity * planet.orbit_params.eccentricity) * sin(E); // position in orbital plane

		float x, y, z;
		x = planet.rotation_frame.R11 * x_P + planet.rotation_frame.R12 * y_Q; //computing the new position for x,y,z
		y = planet.rotation_frame.R21 * x_P + planet.rotation_frame.R22 * y_Q;
		z = planet.rotation_frame.R31 * x_P + planet.rotation_frame.R32 * y_Q;
		std::cout << planet.getName() << " is at (" << x << ", " << y << ", " << z << ")\n";
		Vec3 newPos = { x, y, z };

		planet.setPos(newPos);
	}

};