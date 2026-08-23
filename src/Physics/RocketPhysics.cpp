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
	computes a list of vectors representing thrust applied to entity relative to center of mass of entity. 
	*/
	Vec3 ThrustAcceleration(const Vec3& planetary_acceleration, Entity entity) {	//add additional field for user input defining vectors containing engine orientation
		for (const auto& thrustSource : entity.get_thrust_sources()) {
			//todo: before I can implement this I need to implement a function in main which parses key inputs so they can be translated 
			//to rotation values. RCS thrusters will always fire with 100% thrust. 
			switch(thrustSource.thruster_type) {
			case 0: //rocket engine, compute the thrust vector at point where engine is connected to rocket, this is used to compute the torque applied to the rocket.
				thrustSource.defaultOrientation 
					
			case 1: //RCS thruster

			}

		}
	}



};