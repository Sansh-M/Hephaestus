#pragma once

#include <string>
#include <utility>
#include <vector>
#include "constants.hpp"

/*
Entity class where Entity can beanything 
*/
class Entity {
public:
	Entity(std::string entity_name, const Vec3& initial_position, float entity_mass, std::vector<thrustSource> thrust_sources)
		// Entity identity is supplied once when the object is created.
		: name(std::move(entity_name)),
		  pos(initial_position),
		  Velocity{ 0.0f, 0.0f, 0.0f },
		  origin(initial_position),
		  mass(entity_mass),
		  aerosurface(0),
		  thrust_sources(std::move(thrust_sources)) {
	}
	//entity member function definitions
	const std::string& get_name() const { return name; }
	Vec3 get_pos() const { return pos; }
	void set_pos(const Vec3& position) { pos = position; }
	float get_mass() const { return mass; }
	void set_velocity(const Vec3& vel) { Velocity = vel; }
	Vec3 get_velocity() const { return Velocity; }
	std::vector<thrustSource> get_thrust_sources() const { return thrust_sources; }
	void set_thrust_sources(const std::vector<thrustSource>& sources) { thrust_sources = sources; }
private:
	std::vector<thrustSource> thrust_sources;
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

struct thrustSource {
	int thruster_type; //defines what type of engine is used, thruster, RCS, etc. thruster_type=0 (rocket engine), thruster_type=1 (RCS thruster)
	float thrust; // defined how much thrust is produced by each engine
	Vec3 thrustVector; // THIS DEFINES THE VECTOR FROM THE CENTER OF TEH ENTITY TO THE POINT WHERE THE THRUST IS APPLIED. 
	Vec3 defaultOrientation; //defines the orientation of the engine relative to point where it is connected, default value is orientationo defined at time of launch. 
};
