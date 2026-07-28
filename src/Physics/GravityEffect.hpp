#pragma once

#include "constants.hpp"
#include "Entities.hpp"
#include "PlanetaryBody.hpp"
#include <cmath>
#include <vector>

class GravityEffect {
public:
	/*
	Compute the acceleration caused by one planet at a queried position.
	*/
	Vec3 accelerationVectorAt(
		const Vec3& entity_position,
		const PlanetaryBody& planet
	) const {
		const Vec3 displacement = planet.getPos() - entity_position;

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
	Add the acceleration vectors from every planetary gravity source.
	*/
	Vec3 totalAccelerationAt(
		const Vec3& entity_position,
		const std::vector<PlanetaryBody>& planets
	) const {
		Vec3 total_acceleration{ 0.0f, 0.0f, 0.0f };
		for (const auto& planet : planets) {
			total_acceleration += accelerationVectorAt(entity_position, planet);
		}

		return total_acceleration;
	}

	/*
	Advance an entity by one velocity-Verlet step.

	previous_acceleration must be calculated before the planets advance from
	t.previous to t.current. The planets passed here must already be at
	t.current so that the second acceleration sample uses the end of the step.
	*/
	void integrateVelocityVerlet(
		Entity& entity,
		const std::vector<PlanetaryBody>& planets,
		const Vec3& previous_acceleration,
		const SimulationTime& t
	) const {
		const float delta_time = t.delta();
		if (delta_time <= 0.0f) {
			return;
		}

		const float delta_time_squared = delta_time * delta_time;
		const Vec3 previous_position = entity.get_pos();
		const Vec3 previous_velocity = entity.get_velocity();

		// Drift: predict the end-of-step position using the old acceleration.
		const Vec3 new_position =
			previous_position +
			(previous_velocity * delta_time) +
			(previous_acceleration * (0.5f * delta_time_squared));

		// Sample gravity again at the new entity and planetary positions.
		const Vec3 current_acceleration =
			totalAccelerationAt(new_position, planets);

		// Kick: use the average acceleration across the step.
		const Vec3 average_acceleration =
			(previous_acceleration + current_acceleration) * 0.5f;
		const Vec3 new_velocity =
			previous_velocity + (average_acceleration * delta_time);

		entity.set_pos(new_position);
		entity.set_velocity(new_velocity);
	}
};
