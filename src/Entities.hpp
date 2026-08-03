#pragma once

#include <string>
#include <utility>

#include "constants.hpp"

/*
Entity class where Entity can beanything 
*/
class Entity {
public:
	Entity(std::string entity_name, const Vec3& initial_position, float entity_mass)
		// Entity identity is supplied once when the object is created.
		: name(std::move(entity_name)),
		  pos(initial_position),
		  Velocity{ 0.0f, 0.0f, 0.0f },
		  origin(initial_position),
		  mass(entity_mass),
		  aerosurface(0) {
	}
	//entity member function definitions
	const std::string& get_name() const { return name; }
	Vec3 get_pos() const { return pos; }
	void set_pos(const Vec3& position) { pos = position; }
	float get_mass() const { return mass; }
	void set_velocity(const Vec3& vel) { Velocity = vel; }
	Vec3 get_velocity() const { return Velocity; }
private:
	std::string name;
	Vec3 pos;
	Vec3 Velocity;
	Vec3 origin;
	float mass;
	int aerosurface;  //aerosurface cannot be defined as an integer. Needs a better implementation later

};

/*
Substitute for int placeholder to simulate the aerosurface of an entity to compute the roll, pitch and yaw during atmospheric reentry
*/
struct AeroCoefficients {
	double axial;   // CA
	double side;    // CY
	double normal;  // CN

	double roll;    // Cl
	double pitch;   // Cm
	double yaw;     // Cn
};

struct AeroCoefficientPoint {
	double mach;
	double angleOfAttackRadians;
	double sideslipRadians;
	double knudsenNumber;
	AeroCoefficients coefficients;
};

struct CoefficientAeroDefinition {
	double referenceAreaM2;
	double referenceLengthM;

	// Point about which the stored moment coefficients were calculated.
	Vec3 momentReferenceBodyM;

	// Needed by a first-order stagnation-point heating model.
	double noseRadiusM;

	std::vector<AeroCoefficientPoint> table;
};

