#include <iostream>
#include "Universe_init.hpp"
#include "UniversalFunctions.hpp"
#include <chrono>
#include "PlanetaryMotion.hpp"
#include <atomic>
#include <thread>

const std::string JSONPATH = "src/Data/Universe.json";
PlanetaryMotion motion; 

int main() {
    std::cout << "Physics Engine starting...\n";
    std::atomic<bool> EXIT{ false };
    //main engine loop 

    std::cout << "Building Universe...\n";
    std::vector<PlanetaryBody> planetary_bodies = Universe_init().buildPlanetaryBodies(JSONPATH);
	auto start = std::chrono::steady_clock::now();  
    while (EXIT == false) {
		std::cout << "Press E to exit\n";
        std::string input;
        std::cin >> input;
        auto now = std::chrono::steady_clock::now();
        float t = std::chrono::duration<float>(now - start).count();
        
        motion.updateAll(planetary_bodies, t);

        if (input == "E" || input == "e") {
            EXIT = true;
        }
    }
	auto stop = std::chrono::steady_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	std::cout << "Exiting Physics Engine...\n";
	std::cout << "Runtime = " + std::to_string(duration.count()) + " microseconds\n";
    return 0;
}