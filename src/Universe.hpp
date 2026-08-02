#pragma once

#include <string_view>
#include <vector>

#include "PlanetaryBody.hpp"
#include "Entities.hpp"
#include "PlanetaryMotion.hpp"
#include "constants.hpp"
#include "GravityEffect.hpp"

class Universe final {
public:
	// A Universe must be constructed with the planets it will own.
	explicit Universe(std::vector<PlanetaryBody> planets);

	void advance(SimulationTime& time);

	const std::vector<PlanetaryBody>& planets() const noexcept;
	const std::vector<Entity>& entities() const noexcept;

	const PlanetaryBody* findPlanet(std::string_view id) const noexcept;

	void addEntity(const Entity& entity) {
		entities_.push_back(entity);
	}


private:
	// These collections have one owner; outside systems receive const views.
	std::vector<PlanetaryBody> planets_;
	std::vector<Entity> entities_;
	PlanetaryMotion planetaryMotion_;
	GravityEffect gravityEffect_;
};

