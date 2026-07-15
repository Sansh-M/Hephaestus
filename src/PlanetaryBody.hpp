#include <iostream>
#include <vector>
#include "constants.hpp"
#pragma once

struct OrbitParams {
	float semiMajorAxis;
	float eccentricity;
	float orbitalPeriod;      // seconds
	float meanAnomaly0;       // radians, at t = 0
	float inclination;        // radians, tilt of orbital plane
	float raan;               // radians, longitude of ascending node (Ω)
	float argOfPeriapsis;     // radians, orientation of ellipse within its plane (ω)
	float orbitalPeriod; 
};

/*
Struct to define the transition matrix for the orbital rotation per timestep.
*/
struct RotationFrame {
	float R11, R12, R21, R22, R31, R32; // Rotation matrix components
};

/*
Struct to define the parameters for the rotation of the orbit
*/
struct RotationState {
	float angle0; // Initial angle of rotation (radians)
	float angularVelocity; // Angular velocity of rotation (radians per second)
};

/*
Class used to define planetary bodies.
*/
class PlanetaryBody {
public:
	PlanetaryBody(std::string name, const Vec3& origin, Vec3& pos, float mass, float radius, std::vector<std::string> CSVPATHS, OrbitParams orbit_params, RotationFrame rotation_frame, RotationState rotation_state);

	/*
	set the position of the planetary body after it has been computed for the next frame. 
	*/
	void setPos(Vec3& new_pos) {
		pos = new_pos;
	}

private:
	std::string name;
	Vec3 origin;
	Vec3 pos; 
	float mass;
	float radius;
	std::vector<std::string> CSVPATHS;
	OrbitParams orbit_params;
	RotationFrame rotation_frame;
	RotationState rotation_state;

};
