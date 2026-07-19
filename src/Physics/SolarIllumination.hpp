#include <iostream>
#include <math.h>
#include "PlanetaryBody.hpp"
#include "constants.hpp"
#include <vector>
using namespace std;
using namespace Physics;


/*
class housing functions which are used for the illumination of all PlanetaryBody objects
*/
class SolarIllumination {

	struct SolarFrame {
		Vec3 directionToSunWorld;
		double distanceToSunMeters;
		double sunAngularRadiusRadians;
		double irradianceScale;
	};


	/*
	Determine the lighting per coordinate 
	*/
	Vec3 calculateSolarIncidence(PlanetaryBody& planet, Vec3 point) {
		Vec3 directionToSun = Vec3::normalize({ 0,0,0 } - planet.getPos());
		Vec3 surfaceNormal = Vec3::normalize(point - planet.getPos());
		return dot(surfaceNormal, directionToSun);
	 }

	SolarFrame calculateSolarFrame(
		const PlanetaryBody& planet,
		const PlanetaryBody& sun)
	{
		Vec3d difference = sun.getPos() - planet.getPos();
		double distance = length(difference);

		return {
			difference / distance,
			distance,
			std::asin(sun.getRadius() / distance),
			(ASTRONOMICAL_UNIT / distance) *
			(ASTRONOMICAL_UNIT / distance)
		};
	}
};