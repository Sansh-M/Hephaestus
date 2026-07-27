#include <iostream>
#include <math.h>
#include "PlanetaryBody.hpp"
#include "constants.hpp"
#include <vector>
#include <algorithm>
using namespace std;
using namespace Physics;

//*
// class containing functions for defining the motion of planetary bodies of the solar system around the sun. 
// 
class PlanetaryMotion {
public:

	void updateAll(std::vector<PlanetaryBody>& planetary_bodies, SimulationTime& t) {
		for (auto& planet : planetary_bodies) {
			if (planet.orbit_params.orbitalPeriod == 0.0f) {
				continue;
			}
			auto parentObject = std::find_if(
				planetary_bodies.begin(),
				planetary_bodies.end(),
				[&](const PlanetaryBody& candidate) { 
					return candidate.getName() == planet.parent;
				}
			);
			
			if (parentObject == planetary_bodies.end()) {
				throw std::runtime_error(
					"Could not find parent for " +
					planet.getName()
				);
			}
			
			PlanetaryBody& parent = *parentObject;
			
			updatePosition(planet, t, parent.getPos());

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
	void updatePosition(PlanetaryBody& planet, SimulationTime& t, Vec3 parentPos) {


		float n = 2 * PI / planet.orbit_params.orbitalPeriod; // mean motion
		float mean_anomaly = planet.orbit_params.meanAnomaly0 + n * t.current; // mean anomaly at time t
		float E = solveEccentricAnomaly(mean_anomaly, planet.orbit_params.eccentricity); // eccentric anomaly

		float x_P = planet.orbit_params.semiMajorAxis * (cos(E) - planet.orbit_params.eccentricity); // position in orbital plane)
		float y_Q = planet.orbit_params.semiMajorAxis * sqrt(1.0f - planet.orbit_params.eccentricity * planet.orbit_params.eccentricity) * sin(E); // position in orbital plane

		float x, y, z;
		//using the rotation frame to tilt the 2D computation for the planet's location in the orbit
		x = planet.rotation_frame.R11 * x_P + planet.rotation_frame.R12 * y_Q; 
		y = planet.rotation_frame.R21 * x_P + planet.rotation_frame.R22 * y_Q;
		z = planet.rotation_frame.R31 * x_P + planet.rotation_frame.R32 * y_Q;
		std::cout << planet.getName() << " is at (" << x << ", " << y << ", " << z << ")\n";

		Vec3 newPos = {
			x + parentPos.x,
			y + parentPos.y,
			z + parentPos.z
		};

		const float delta_time = t.delta();
		if (delta_time > 0.0f) {
			const Vec3 previous_position = planet.getPos();
			const Vec3 velocity = {
				(newPos.x - previous_position.x) / delta_time,
				(newPos.y - previous_position.y) / delta_time,
				(newPos.z - previous_position.z) / delta_time
			};
			planet.setVelocity(velocity);
		}

		planet.setPos(newPos);
	}

};
