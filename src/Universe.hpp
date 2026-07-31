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

/*
Handles the progression of time for the game engine and the movement of each epoch
*/
void advance(SimulationTime& time) {

}

/*
Creates the vector containing all planets in the universe.
*/
const std::vector<PlanetaryBody>& planets() const noexcept {

}

/*
Creates the veector containing all of the entity objects in the universe 
*/
const std::vector<Entity>& entities() const noexcept {

}

/*
searches for a planet and returns it 
*/
const PlanetaryBody* findPlanet(std::string_view id) const noexcept {

}

