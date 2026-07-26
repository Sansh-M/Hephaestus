#include "constants.hpp"
#include "Entities.hpp"
#include "PlanetaryBody.hpp"
#include <iostream>
#include <Vector>


class GravityEffect {
	
	/*
	Function to compute the acceleration vector from a specific planet on an entity 
	*/
	Vec3 accelerationVector(Entity& entity, PlanetaryBody& planet) {
		Vec3 planet_pos = planet.getPos();
		float planet_mass = planet.getMass();
		float entity_mass = entity.get_mass();
		Vec3 entity_pos = entity.get_pos();
		float planet_radius = planet.getRadius();
		
		planet_pos.sub(entity_pos);
		float grav_factor = (GRAV_CONST * planet_mass)/planet_radius^3;
		Vec3 result_accceleration = { planet_pos.x * grav_factor, planet_pos.y * grav_factor, planet_pos.z * grav_factor };
		return result_accceleration;
	}

	Vec3 compute_position(Entity& entity, Vector<PlanetaryBody> planets) {
		Vec3 acceleration_component;
		for (auto& planet in planets) {
			Vec3 planet_acceleration = accelerationVector(entity, planet);
			acceleration_component.x += planet_acceleration.x;
			acceleration_component.y += planet_acceleration.y;
			acceleration_component.z += planet_acceleration.z;
		}

	}

};