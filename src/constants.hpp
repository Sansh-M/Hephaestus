#include <iostream>
#include <vector>
#pragma once

namespace Physics {
	// physics constants
	constexpr float GRAVITY = 9.81f; // m/s^2
	constexpr float GRAV_CONST = 6.67430e-11f; // m^3 kg^-1 s^-2
	constexpr int LIGHT_SPEED = 299792458; // m s^-1
}
struct Vec3 { float x,y,z; 
	void add(Vec3 delta) {
		x += delta.x;
		y += delta.y;
		z += delta.z;
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
};

