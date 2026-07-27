#include "constants.hpp"
#include "Entities.hpp"
#include "PlanetaryBody.hpp"
#include <iostream>
#include <Vector>


class GravityEffect {
	
	/*
	Function to compute the acceleration vector from a specific planet on an entity 
	*/
	Vec3 accelerationVector(Entity& entity, PlanetaryBody& planet, SimulationTime& t) {
		Vec3 planet_pos = planet.getPos();
		float planet_mass = planet.getMass();
		float entity_mass = entity.get_mass();
		Vec3 entity_pos = entity.get_pos();
		float planet_radius = planet.getRadius();
		
		planet_pos.sub(entity_pos);
		float grav_factor = (Physics::GRAV_CONST * planet_mass)/planet_radius^3;
		Vec3 result_accceleration = { planet_pos.x * grav_factor, planet_pos.y * grav_factor, planet_pos.z * grav_factor };
		return result_accceleration;
	}

	/*
	function to compute the velocity of an entity in m/s 
	*/
	Vec3 compute_velocity(Entity& entity, Vector<PlanetaryBody> planets, SimulationTime& t) {
		Vec3 acceleration_component;
		for (auto& planet in planets) {
			Vec3 planet_acceleration = accelerationVector(entity, planet);
			acceleration_component.x += planet_acceleration.x;
			acceleration_component.y += planet_acceleration.y;
			acceleration_component.z += planet_acceleration.z;
		}
		acceleration_component.multiply({ t.delta(), t.delta(), t.delta()});
		Vec3 current_velocity = entity.get_velocity().add(acceleration_component);
		entity.set_velocity(current_velocity);
		entity.set_pos(compute_position(entity, t, acceleration_component));
	}

	/*
	function to compute the global position of the entity 
	*/
	Vec3 compute_position(Entity& const entity, SimulationTime& const t, Vec3 accelerationVector) {
		Vec3 new_pos = {
			(entity.get_velocity().x * t.delta()) - 1 / 2 * (accelerationVector.x * (t.delta() * t.delta())),
			(entity.get_velocity().y * t.delta()) - 1 / 2 * (accelerationVector.y * (t.delta() * t.delta())),
			(entity.get_velocity().z * t.delta()) - 1 / 2 * (accelerationVector.z * (t.delta() * t.delta()))
		}
		return new_pos;
	}
};