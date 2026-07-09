#include <iostream>
#include <vector>
#include "constants.hpp"
#pragma once


/*
Class used to define planetary bodies.
*/
class PlanetaryBody {
public:
	PlanetaryBody(std::string name, const Vec3& origin, float mass, float radius, std::string CSVPATHS[10]);

private:
	std::string name;
	Vec3 origin;	
	float mass;
	float radius;
	std::string CSVPATHS[10];
};
