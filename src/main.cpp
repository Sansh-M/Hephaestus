#include <iostream>
#include "Universe_init.cpp"
#include "UniversalFunctions.hpp"
#include <chrono>

const std::string JSONPATH = "src/Data/Universe.json";


int main() {
    std::cout << "Physics Engine starting...\n";
    bool EXIT = false;
    //main engine loop 
    std::cout << "Building Universe...\n";
    std::vector<PlanetaryBody> planetary_bodies = Universe_init().buildPlanetaryBodies(JSONPATH);
    auto now = std::chrono::steady_clock::now();
    while (EXIT == false) {
		std::cout << "Press E to exit\n";
        std::string input;
        std::cin >> input;
        if (input == "E" || input == "e") {
            EXIT = true;
        }
    }
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	std::cout << "Exiting Physics Engine...\n";
	std::cout << "Runtime = " + std::to_string(duration.count()) + " microseconds\n";
    return 0;
}