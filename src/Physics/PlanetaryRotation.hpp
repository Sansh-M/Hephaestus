#include <iostream>
#include <math.h>
#include "PlanetaryBody.hpp"
#include "constants.hpp"
#include <array>
#include <vector>
#include <map>
using namespace std;
using namespace Physics;

class PlanetaryRotation {

	using PolePair = std::array<PlanetaryCoordinates, 2>;

	/*
	Constains the pole longitude and lattitude coordinates for each planet about which their rotation occurs. 
	*/
	inline static const std::map<std::string, PolePair> poles = {
		{"Mercury", {{{281.010300, 61.415500}, {101.010300, -61.415500}}}},
		{"Venus",   {{{272.760000, 67.160000}, { 92.760000, -67.160000}}}},
		{"Earth",   {{{  0.000000, 90.000000}, {180.000000, -90.000000}}}},
		{"Mars",    {{{317.680854, 52.886439}, {137.680854, -52.886439}}}},
		{"Jupiter", {{{268.057204, 64.495810}, { 88.057204, -64.495810}}}},
		{"Saturn",  {{{ 40.589000, 83.537000}, {220.589000, -83.537000}}}},
		{"Titan",   {{{ 39.482700, 83.427900}, {219.482700, -83.427900}}}},
		{"Uranus",  {{{257.311000,-15.175000}, { 77.311000,  15.175000}}}},
		{"Neptune", {{{299.333739, 42.950359}, {119.333739, -42.950359}}}}
	};
};
