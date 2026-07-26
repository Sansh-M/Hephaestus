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
	
	//function to compute dot product of two vectors
	Vec3 dot(Vec3 vector1, Vec3 vector2) {
		x = vector1.x * vector2.x;
		y = vector1.y * vector2.y;
		z = vector1.z * vector2.z;
		return Vec3{ x,y,z };
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

