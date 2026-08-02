#include "Universe.hpp"

#include <algorithm>
#include <utility>


Universe::Universe(std::vector<PlanetaryBody> planets)
	// Move the loaded vector into the Universe instead of copying every body.
	: planets_(std::move(planets)) {
}

/*
Handles the calls that are made at each epoch
*/
void Universe::advance(SimulationTime & time) {
	// Entity update order: sample old gravity, move planets once, then integrate.
	std::vector<Vec3> previousAccelerations;
	previousAccelerations.reserve(entities_.size());

	for (const Entity& entity : entities_) {
		previousAccelerations.push_back(
			gravityEffect_.totalAccelerationAt(entity.get_pos(), planets_)
		);
	}

	planetaryMotion_.updateAll(planets_, time);

	for (std::size_t i = 0; i < entities_.size(); ++i) {
		gravityEffect_.integrateVelocityVerlet(
			entities_[i],
			planets_,
			previousAccelerations[i],
			time
		);
	}
}

/*
returns the vector containing all planets in the universe.
*/
const std::vector<PlanetaryBody>& Universe::planets() const noexcept {
	return planets_;
}

/*
returns the vector containing all of the entity objects in the universe
*/
const std::vector<Entity>& Universe::entities() const noexcept {
	return entities_;
}

/*
searches for a planet and returns it
*/
const PlanetaryBody* Universe::findPlanet(std::string_view id) const noexcept {
	const auto found = std::find_if(
		planets_.cbegin(),
		planets_.cend(),
		[id](const PlanetaryBody& planet) {
			return std::string_view(planet.getName()) == id;
		}
	);

	// A null pointer explicitly tells the caller that the requested body is absent.
	return found == planets_.cend() ? nullptr : &*found;
}
