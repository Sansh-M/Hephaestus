#include <iostream>
#include "PlanetaryBody.hpp"
#include "Entities.hpp"
#include "PlanetaryMotion.hpp"
#include "constants.hpp"
#include "Universe_init.hpp"
#include <chrono>
#include <atomic>
#include <thread>
#include "GravityEffect.hpp"

class Universe final {
public: 
	void advance(SimulationTime& time);
	const std::vector<PlanetaryBody>& planets() const noexcept;
	const std::vector<Entity>& entities() const noexcept;
	const PlanetaryBody* findPlanet(std::string_view id) const noexcept;
	

private: 
	std::vector<PlanetaryBody> planets_;
	std::vector<Entity> Entities_;
	PlanetaryMotion planetaryMotion_;
};
