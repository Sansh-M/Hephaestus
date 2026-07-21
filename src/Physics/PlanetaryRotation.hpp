#include <iostream>
#include <math.h>
#include "PlanetaryBody.hpp"
#include "constants.hpp"
#include <vector>
#include <map>
using namespace std;
using namespace Physics;

class PlanetaryRotation {

	struct RotationParameters {
		Vec3 unitRotationAxis;
		double primeMeridianAngleAtEpochRadians;
		double angularRotationRateRadiansPerSecond;
	};

	inline static constexpr double TWO_PI_RADIANS = 6.28318530717958647692;

	static constexpr double angularRateFromPeriodSeconds(
		double rotationPeriodSeconds,
		double rotationDirection)
	{
		return rotationDirection * TWO_PI_RADIANS / rotationPeriodSeconds;
	}

	/*
	Rotation data for each body. The axes and sidereal rotation periods are derived
	from the NAIF pck00011.tpc orientation model. Periods are supplied in seconds,
	so the resulting angular rates match the main loop's simulation-time unit.

	The starting prime-meridian angles use the simplified game convention of zero
	radians rather than each body's real J2000 prime-meridian angle.
	*/
	inline static const std::map<std::string, RotationParameters> rotationParameters = {
		{"Mercury", {{ 0.091377818f, -0.469647350f,  0.878112442f}, 0.0, angularRateFromPeriodSeconds( 5'067'027.005964,  1.0)}},
		{"Venus",   {{ 0.018690814f, -0.387708808f,  0.921592390f}, 0.0, angularRateFromPeriodSeconds(20'996'797.016381, -1.0)}},
		{"Earth",   {{ 0.000000000f,  0.000000000f,  1.000000000f}, 0.0, angularRateFromPeriodSeconds(    86'164.096228,  1.0)}},
		{"Mars",    {{ 0.446155271f, -0.406242671f,  0.797441137f}, 0.0, angularRateFromPeriodSeconds(    88'642.663715,  1.0)}},
		{"Jupiter", {{-0.014597291f, -0.430329594f,  0.902553799f}, 0.0, angularRateFromPeriodSeconds(    35'729.711350,  1.0)}},
		{"Saturn",  {{ 0.085478832f,  0.073235758f,  0.993644752f}, 0.0, angularRateFromPeriodSeconds(    38'362.399998,  1.0)}},
		{"Titan",   {{ 0.088337048f,  0.072774657f,  0.993428616f}, 0.0, angularRateFromPeriodSeconds( 1'377'686.670609,  1.0)}},
		{"Uranus",  {{-0.211999582f, -0.941559157f, -0.261768086f}, 0.0, angularRateFromPeriodSeconds(    62'064.000001, -1.0)}},
		{"Neptune", {{ 0.358576509f, -0.638095102f,  0.681364460f}, 0.0, angularRateFromPeriodSeconds(    57'478.679995,  1.0)}}
	};


	void updateAll(std::vector<PlanetaryBody>& planetary_bodies, float t) {
		for (auto& planet : planetary_bodies) {
			updateRotation(planet, t);
		}

	}


	void updateRotation(PlanetaryBody& planet, float t) {
		const auto& parameters = rotationParameters.at(planet.getName());

		const double planetRotation = rotationParameters.at(planet.getName()).primeMeridianAngleAtEpochRadians 
			+ (rotationParameters.at(planet.getName()).angularRotationRateRadiansPerSecond * t);
		
		const Vec3 referenceFromCenterAtEpoch = planet.getReferenceCoordinateAtEpoch();

		const Vec3 rotatedReferenceFromCenter = rotateAroundUnitAxis(referenceFromCenterAtEpoch, parameters.unitRotationAxis, planetRotation);

		const Vec3 planetCenter = planet.getPos();

		const Vec3 referenceWorld{
			planetCenter.x + rotatedReferenceFromCenter.x;
			planetCenter.y + rotatedReferenceFromCenter.y; 
			planetCenter.z + rotatedReferenceFromCenter.z; 
		}

		planet.setReferenceCoordinate(referenceWorld);
	}


	/*
	Implementation of Rodrigues' formula to compute planet reference coordinate after rotation. 
	*/
	Vec3 rotateAroundUnitAxis(
		const Vec3& vectorFromCenter,
		const Vec3& unitAxis,
		double angleRadians)
	{
		const double cosine = std::cos(angleRadians);
		const double sine = std::sin(angleRadians);

		const double dot =
			unitAxis.x * vectorFromCenter.x +
			unitAxis.y * vectorFromCenter.y +
			unitAxis.z * vectorFromCenter.z;

		// unitAxis cross vectorFromCenter
		const Vec3 cross{
			unitAxis.y * vectorFromCenter.z
				- unitAxis.z * vectorFromCenter.y,

			unitAxis.z * vectorFromCenter.x
				- unitAxis.x * vectorFromCenter.z,

			unitAxis.x * vectorFromCenter.y
				- unitAxis.y * vectorFromCenter.x
		};

		return {
			static_cast<float>(
				vectorFromCenter.x * cosine
				+ cross.x * sine
				+ unitAxis.x * dot * (1.0 - cosine)
			),

			static_cast<float>(
				vectorFromCenter.y * cosine
				+ cross.y * sine
				+ unitAxis.y * dot * (1.0 - cosine)
			),

			static_cast<float>(
				vectorFromCenter.z * cosine
				+ cross.z * sine
				+ unitAxis.z * dot * (1.0 - cosine)
			)
		};
	}

};
