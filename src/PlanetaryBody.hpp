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
};

/*
Struct to define the transition matrix for the orbital rotation per timestep.
*/
struct RotationFrame {
	float R11, R12, R21, R22, R31, R32; // Rotation matrix components
};

//longitude and lattitude are angles, coordinates can be computed for the global system by multiplying 
struct PlanetaryCoordinates {
	double longitude; 
	double lattitude; 
};

/*
Class used to define planetary bodies.
*/
class PlanetaryBody {
public:
	PlanetaryBody(std::string name, const Vec3& origin, Vec3& pos, double planetRotation, float mass, float radius, std::vector<std::string> CSVPATHS, OrbitParams orbit_params, RotationFrame rotation_frame)
		: name(name), origin(origin), pos(pos), planetRotation(planetRotation), mass(mass), radius(radius), CSVPATHS(CSVPATHS), orbit_params(orbit_params), rotation_frame(rotation_frame) {
	}

	/*
	set the position of the planetary body after it has been computed for the next frame. 
	*/
	void setPos(Vec3& new_pos) {
		pos = new_pos;
	}

	const Vec3& getPos() const { return pos; }
	const OrbitParams orbit_params;
	const RotationFrame rotation_frame;
	const std::string& getName() const { return name; }
	const Vec3& getReferenceCoordinateAtEpoch() const {
		return SurfaceReferenceCoordinateAtEpoch;
	}

	const Vec3& getReferenceCoordinate() const {
		return SurfaceReferenceCoordinate;
	}
	
	void setReferenceCoordinate(const Vec3& updated_reference_coordinate) {
		SurfaceReferenceCoordinate = updated_reference_coordinate;
	}

private:
	std::string name;
	Vec3 origin;
	Vec3 pos; 
	double planetRotation;
	float mass;
	float radius;
	std::vector<std::string> CSVPATHS;
	Vec3 SurfaceReferenceCoordinateAtEpoch = { radius, 0.0f, 0.0f };
	Vec3 SurfaceReferenceCoordinate = {
		pos.x + radius,
		pos.y,
		pos.z
	};
};
