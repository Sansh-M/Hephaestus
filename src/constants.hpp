#include <iostream>
#include <vector>
#include <cmath>
#pragma once

namespace Physics {
	// physics constants
	// ------------mechanics constants----------------- 
	constexpr float GRAVITY = 9.81f; // m/s^2
	constexpr float GRAV_CONST = 6.67430e-11f; // m^3 kg^-1 s^-2
	constexpr int LIGHT_SPEED = 299792458; // m s^-1
	//-------------atmospheric constants---------------
	constexpr float UNIVERSAL_GAS_CONSTANT = 8.314f; //J/mol*K
	constexpr float PI = 3.14159265f;
	
}

struct SimulationTime {
	float current{ 0.0f };
	float previous{ 0.0f };
	float delta() const {
		return current - previous; 
	}
};

struct Vec3 { 
	
	float x,y,z; 
	void add(Vec3 delta) {
		x += delta.x;
		y += delta.y;
		z += delta.z;
	}

	void sub(Vec3 delta) {
		x -= delta.x;
		y -= delta.y;
		z -= delta.z;
	}

	void multiply(Vec3 factor) {
		x *= factor.x;
		y *= factor.y;
		z *= factor.z;
	}

	void divide(Vec3 factor) {
		x /= factor.x; 
		y /= factor.y;
		z /= factor.z;
	}

	Vec3 operator+(const Vec3& other) const {
		return {
			x + other.x,
			y + other.y,
			z + other.z
		};
	}

	Vec3 operator-(const Vec3& other) const {
		return {
			x - other.x,
			y - other.y,
			z - other.z
		};
	}

	Vec3 operator*(float scalar) const {
		return {
			x * scalar,
			y * scalar,
			z * scalar
		};
	}

	Vec3& operator+=(const Vec3& other) {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	Vec3& operator*=(float scalar) {
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}
	
	//function to compute dot product of two vectors
	double dot(const Vec3& other) const {
		return x * other.x +
			y * other.y +
			z * other.z;
	}

	//function to normalize a vector 
	Vec3 normalize(Vec3 vec)
	{
		float vecLength = std::sqrt(
			vec.x * vec.x +
			vec.y * vec.y +
			vec.z * vec.z
		);

		if (vecLength == 0.0f) {
			return { 0.0f, 0.0f, 0.0f };
		}

		return {
			vec.x / vecLength,
			vec.y / vecLength,
			vec.z / vecLength
		};
	}
};

