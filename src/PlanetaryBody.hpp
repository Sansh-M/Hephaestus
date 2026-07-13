#include <iostream>
#include <vector>
#include "constants.hpp"
#pragma once


/*
Class used to define planetary bodies.
*/
class PlanetaryBody {
public:
	PlanetaryBody(std::string name, const Vec3& origin, Vec3& pos, float mass, float radius, std::vector<std::string> CSVPATHS);

	/*
	set the position of the planetary body after it has been computed for the next frame. 
	*/
	void setPos(Vec3& new_pos) {
		pos = new_pos;
	}

private:
	std::string name;
	Vec3 origin;
	Vec3 pos; 
	float mass;
	float radius;
	std::vector<std::string> CSVPATHS;
};
