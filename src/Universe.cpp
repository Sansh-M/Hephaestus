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
#include "Universe.hpp"
#include <algorithm>
#include <utility>


Universe::Universe(std::vector<PlanetaryBody> planets)
	: planets_(std::move(planets)) {
}

/*
Handles the calls that are made at each epoch
*/
void Universe::advance(SimulationTime & time) {
	planetaryMotion_.updateAll(planets_, time);
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
	return Entities_;
}

/*
searches for a planet and returns it
*/
const PlanetaryBody* Universe::findPlanet(std::string_view id) const noexcept {

}
