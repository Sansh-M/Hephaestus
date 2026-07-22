#pragma once

#include <cmath>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#include "PlanetaryBody.hpp"
#include "constants.hpp"

class PlanetaryRotation {
	/*
	The equatorial reference is a unit vector in the planet's equatorial plane.
	The prime-meridian angle rotates that base direction to longitude zero at the
	epoch, so the angle must not already be encoded in the stored vector.
	*/
	struct RotationParameters {
		Vec3 unitRotationAxis;
		Vec3 equatorialReferenceDirection;
		double primeMeridianAngleAtEpochRadians;
		double angularRotationRateRadiansPerSecond;
	};

	inline static constexpr double TWO_PI_RADIANS = 6.28318530717958647692;
	inline static constexpr double HALF_PI_RADIANS = 1.57079632679489661923;

	static constexpr double angularRateFromPeriodSeconds(
		double rotationPeriodSeconds,
		double rotationDirection)
	{
		return rotationDirection * TWO_PI_RADIANS / rotationPeriodSeconds;
	}

	static double dot(const Vec3& left, const Vec3& right)
	{
		return static_cast<double>(left.x) * right.x
			+ static_cast<double>(left.y) * right.y
			+ static_cast<double>(left.z) * right.z;
	}

	static Vec3 cross(const Vec3& left, const Vec3& right)
	{
		return {
			left.y * right.z - left.z * right.y,
			left.z * right.x - left.x * right.z,
			left.x * right.y - left.y * right.x
		};
	}

	static Vec3 normalized(const Vec3& vector)
	{
		const double length = std::sqrt(dot(vector, vector));

		if (length == 0.0) {
			throw std::invalid_argument("Cannot normalize a zero-length vector");
		}

		return {
			static_cast<float>(vector.x / length),
			static_cast<float>(vector.y / length),
			static_cast<float>(vector.z / length)
		};
	}

	/*
	Project solar-system +X into the planet's equatorial plane. This creates a
	consistent game reference
	*/
	static Vec3 equatorialReferenceFromAxis(const Vec3& unitRotationAxis)
	{
		const Vec3 seed = std::abs(unitRotationAxis.x) < 0.9f
			? Vec3{1.0f, 0.0f, 0.0f}
			: Vec3{0.0f, 1.0f, 0.0f};

		const double projection = dot(seed, unitRotationAxis);
		const double x = seed.x - unitRotationAxis.x * projection;
		const double y = seed.y - unitRotationAxis.y * projection;
		const double z = seed.z - unitRotationAxis.z * projection;
		const double length = std::sqrt(x * x + y * y + z * z);

		if (length == 0.0) {
			throw std::invalid_argument("Rotation axis cannot define an equatorial reference");
		}

		return {
			static_cast<float>(x / length),
			static_cast<float>(y / length),
			static_cast<float>(z / length)
		};
	}

	static RotationParameters makeRotationParameters(
		const Vec3& unitRotationAxis,
		double primeMeridianAngleAtEpochRadians,
		double rotationPeriodSeconds,
		double rotationDirection)
	{
		const Vec3 normalizedRotationAxis = normalized(unitRotationAxis);

		return {
			normalizedRotationAxis,
			equatorialReferenceFromAxis(normalizedRotationAxis),
			primeMeridianAngleAtEpochRadians,
			angularRateFromPeriodSeconds(rotationPeriodSeconds, rotationDirection)
		};
	}

	/*
	Rotation data for each body. The axes and sidereal rotation periods are derived
	from the NAIF pck00011.tpc orientation model. Periods are supplied in seconds,
	so the resulting angular rates match the main loop's simulation-time unit.

	The starting prime-meridian angles use the simplified game convention of zero
	radians rather than each body's real J2000 prime-meridian angle.
	*/
	inline static const std::map<std::string, RotationParameters> rotationParameters = {
		{"Mercury", makeRotationParameters({ 0.091377818f, -0.469647350f,  0.878112442f}, 0.0,  5'067'027.005964,  1.0)},
		{"Venus",   makeRotationParameters({ 0.018690814f, -0.387708808f,  0.921592390f}, 0.0, 20'996'797.016381, -1.0)},
		{"Earth",   makeRotationParameters({ 0.000000000f,  0.000000000f,  1.000000000f}, 0.0,     86'164.096228,  1.0)},
		{"Mars",    makeRotationParameters({ 0.446155271f, -0.406242671f,  0.797441137f}, 0.0,     88'642.663715,  1.0)},
		{"Jupiter", makeRotationParameters({-0.014597291f, -0.430329594f,  0.902553799f}, 0.0,     35'729.711350,  1.0)},
		{"Saturn",  makeRotationParameters({ 0.085478832f,  0.073235758f,  0.993644752f}, 0.0,     38'362.399998,  1.0)},
		{"Titan",   makeRotationParameters({ 0.088337048f,  0.072774657f,  0.993428616f}, 0.0,  1'377'686.670609,  1.0)},
		{"Uranus",  makeRotationParameters({-0.211999582f, -0.941559157f, -0.261768086f}, 0.0,     62'064.000001, -1.0)},
		{"Neptune", makeRotationParameters({ 0.358576509f, -0.638095102f,  0.681364460f}, 0.0,     57'478.679995,  1.0)}
	};

	static Vec3 computeE0(const RotationParameters& parameters, double timeSinceEpochSeconds)
	{
		const double angle = std::remainder(
			parameters.primeMeridianAngleAtEpochRadians
				+ parameters.angularRotationRateRadiansPerSecond * timeSinceEpochSeconds,
			TWO_PI_RADIANS);

		return rotateAroundUnitAxis(
			parameters.equatorialReferenceDirection,
			parameters.unitRotationAxis,
			angle);
	}

	/*
	Implementation of Rodrigues' formula to compute planet reference coordinate after rotation. 
	*/
	static Vec3 rotateAroundUnitAxis(
		const Vec3& vectorFromCenter,
		const Vec3& unitAxis,
		double angleRadians)
	{
		const double cosine = std::cos(angleRadians);
		const double sine = std::sin(angleRadians);

		const double parallelAmount = dot(unitAxis, vectorFromCenter);
		const Vec3 perpendicular = cross(unitAxis, vectorFromCenter);

		return {
			static_cast<float>(
				vectorFromCenter.x * cosine
				+ perpendicular.x * sine
				+ unitAxis.x * parallelAmount * (1.0 - cosine)
			),

			static_cast<float>(
				vectorFromCenter.y * cosine
				+ perpendicular.y * sine
				+ unitAxis.y * parallelAmount * (1.0 - cosine)
			),

			static_cast<float>(
				vectorFromCenter.z * cosine
				+ perpendicular.z * sine
				+ unitAxis.z * parallelAmount * (1.0 - cosine)
			)
		};
	}

	static const RotationParameters& parametersFor(const PlanetaryBody& planet)
	{
		const auto parameters = rotationParameters.find(planet.getName());
		if (parameters == rotationParameters.end()) {
			throw std::invalid_argument(
				"No rotation parameters exist for planetary body: " + planet.getName());
		}

		return parameters->second;
	}

	static Vec3 worldCoordinateFromFrame(
		const PlanetaryBody& planet,
		const PlanetaryCoordinate& coordinate,
		const RotationParameters& parameters,
		const Vec3& e0Current,
		const Vec3& e90Current)
	{
		if (!std::isfinite(coordinate.latitudeRadians)
			|| !std::isfinite(coordinate.longitudeRadians)
			|| !std::isfinite(coordinate.altitudeMeters)) {
			throw std::invalid_argument("Planetary coordinate values must be finite");
		}

		if (coordinate.latitudeRadians < -HALF_PI_RADIANS
			|| coordinate.latitudeRadians > HALF_PI_RADIANS) {
			throw std::out_of_range("Latitude must be between -pi/2 and pi/2 radians");
		}

		const double distanceFromCenter = planet.getRadius() + coordinate.altitudeMeters;
		if (distanceFromCenter < 0.0) {
			throw std::out_of_range("Altitude places the coordinate beyond the planet center");
		}

		const double cosineLatitude = std::cos(coordinate.latitudeRadians);
		const double sineLatitude = std::sin(coordinate.latitudeRadians);
		const double cosineLongitude = std::cos(coordinate.longitudeRadians);
		const double sineLongitude = std::sin(coordinate.longitudeRadians);

		const double e0Scale = distanceFromCenter * cosineLatitude * cosineLongitude;
		const double e90Scale = distanceFromCenter * cosineLatitude * sineLongitude;
		const double axisScale = distanceFromCenter * sineLatitude;

		const Vec3& planetCenter = planet.getPos();
		return {
			static_cast<float>(planetCenter.x
				+ e0Current.x * e0Scale
				+ e90Current.x * e90Scale
				+ parameters.unitRotationAxis.x * axisScale),
			static_cast<float>(planetCenter.y
				+ e0Current.y * e0Scale
				+ e90Current.y * e90Scale
				+ parameters.unitRotationAxis.y * axisScale),
			static_cast<float>(planetCenter.z
				+ e0Current.z * e0Scale
				+ e90Current.z * e90Scale
				+ parameters.unitRotationAxis.z * axisScale)
		};
	}

public:
	/*
	Convert one body-fixed latitude/longitude/altitude coordinate into the
	solar-system frame without changing the stored local coordinate.
	*/
	Vec3 queryWorldCoordinate(
		const PlanetaryBody& planet,
		const PlanetaryCoordinate& coordinate,
		double timeSinceEpochSeconds) const
	{
		const auto& parameters = parametersFor(planet);
		const Vec3 e0Current = computeE0(parameters, timeSinceEpochSeconds);
		const Vec3 e90Current = normalized(cross(parameters.unitRotationAxis, e0Current));

		return worldCoordinateFromFrame(
			planet, coordinate, parameters, e0Current, e90Current);
	}

	std::vector<Vec3> queryWorldCoordinates(
		const PlanetaryBody& planet,
		const std::vector<PlanetaryCoordinate>& coordinates,
		double timeSinceEpochSeconds) const
	{
		std::vector<Vec3> worldCoordinates;
		worldCoordinates.reserve(coordinates.size());
		const auto& parameters = parametersFor(planet);
		const Vec3 e0Current = computeE0(parameters, timeSinceEpochSeconds);
		const Vec3 e90Current = normalized(cross(parameters.unitRotationAxis, e0Current));

		for (const auto& coordinate : coordinates) {
			worldCoordinates.push_back(
				worldCoordinateFromFrame(
					planet, coordinate, parameters, e0Current, e90Current));
		}

		return worldCoordinates;
	}

};
