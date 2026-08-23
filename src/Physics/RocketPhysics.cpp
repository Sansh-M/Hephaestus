#include "Entities.hpp"
#include <iostream>
#include "PlanetaryBody.hpp"
#include "Universe.hpp"
#include "constants.hpp"

/*
Class with functions that define the motion of rockets defined by user inputs.
*/
class RocketPhysics {

	/*
	Function to define the change in motion given thrust vector from rocket engine.
	Returns the modified acceleration vector which can then be used to update the velocity and position of the entity using Universe.advance()	
	*/
	
	Vec3 ThrustAcceleration(const Vec3& planetary_acceleration, Entity entity, Vec3 thrust_vector) {

	}

	Vec3 RCSAcceleration(const Vec3& planetary_acceleration, Entity entity, Vec3 thrust_vector) {

	}

};