#include <iostream>
#include "Universe_init.cpp"
#include "UniversalFunctions.hpp"

const std::string JSONPATH = "src/Data/Universe.json";


int main() {
    std::cout << "Physics Engine starting...\n";
    bool EXIT = false;
    //main engine loop 
    std::cout << "Building Universe...\n";
    std::vector<PlanetaryBody> planetary_bodies = Universe_init().buildPlanetaryBodies(JSONPATH);
    while (EXIT = false) {
        
    }
    return 0;
}