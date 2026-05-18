#include <iostream>
#include <vector>
#include "constants.hpp"
#pragma once
/*
Class used to define planetary bodies.
*/
class PlanetaryBodies {
public:
	PlanetaryBodies(const Vec3& origin, float mass, float radius, float atmospheric_density);

private:
	Vec3 origin;
	float mass;
	float radius;
	float ref_density;
	float temp_lb;		//here lb refers to "layer base" which is the lower bound for an atmospheric layer
	float altitude_lb;
	float temp_lr;
	float high_start;
	float mid_start;
	float low_start;
	std::string HIGHCSVPATH;
	std::string MIDCSVPATH;
	std::string LOWCSVPATH;
};