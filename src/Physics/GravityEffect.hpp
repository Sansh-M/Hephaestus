#pragma once

#include "constants.hpp"
#include "Entities.hpp"
#include "PlanetaryBody.hpp"
#include <cmath>
#include <vector>

class GravityEffect {
public:
	/*
	Function to compute the acceleration vector from a specific planet on an entity 
	*/
	Vec3 accelerationVector(const Entity& entity, const PlanetaryBody& planet) const {
		Vec3 displacement = planet.getPos();
		displacement.sub(entity.get_pos());

		const float distance_squared =
			displacement.x * displacement.x +
			displacement.y * displacement.y +
			displacement.z * displacement.z;

		if (distance_squared == 0.0f) {
			return { 0.0f, 0.0f, 0.0f };
		}

		const float distance = std::sqrt(distance_squared);
		const float grav_factor =
			(Physics::GRAV_CONST * planet.getMass()) /
			(distance_squared * distance);

		return {
			displacement.x * grav_factor,
			displacement.y * grav_factor,
			displacement.z * grav_factor
		};
	}

	/*
	function to compute the velocity of an entity in m/s 
	*/
	Vec3 compute_velocity(
		Entity& entity,
		const std::vector<PlanetaryBody>& planets,
		const SimulationTime& t
	) const {
		const float delta_time = t.delta();
		if (delta_time <= 0.0f) {
			return entity.get_velocity();
		}

		Vec3 total_acceleration{ 0.0f, 0.0f, 0.0f };
		for (const auto& planet : planets) {
			const Vec3 planet_acceleration = accelerationVector(entity, planet);
			total_acceleration.x += planet_acceleration.x;
			total_acceleration.y += planet_acceleration.y;
			total_acceleration.z += planet_acceleration.z;
		}

		Vec3 velocity_change = total_acceleration;
		velocity_change.multiply({ delta_time, delta_time, delta_time });

		Vec3 current_velocity = entity.get_velocity();
		current_velocity.add(velocity_change);
		entity.set_velocity(current_velocity);
		entity.set_pos(compute_position(entity, t, total_acceleration));

		return current_velocity;
	}

	/*
	function to compute the global position of the entity 
	*/
	Vec3 compute_position(
		const Entity& entity,
		const SimulationTime& t,
		const Vec3& acceleration_vector
	) const {
		const float delta_time = t.delta();
		const float delta_time_squared = delta_time * delta_time;
		const Vec3 current_position = entity.get_pos();
		const Vec3 current_velocity = entity.get_velocity();

		return {
			current_position.x +
				(current_velocity.x * delta_time) -
				(0.5f * acceleration_vector.x * delta_time_squared),
			current_position.y +
				(current_velocity.y * delta_time) -
				(0.5f * acceleration_vector.y * delta_time_squared),
			current_position.z +
				(current_velocity.z * delta_time) -
				(0.5f * acceleration_vector.z * delta_time_squared)
		};
	}
};
