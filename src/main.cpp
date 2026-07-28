#include <iostream>
#include "Universe_init.hpp"
#include <chrono>
#include "PlanetaryMotion.hpp"
#include <atomic>
#include <thread>
#include "constants.hpp"
#include "Entities.hpp"
#include "GravityEffect.hpp"

const std::string JSONPATH = "src/Data/Universe.json";
PlanetaryMotion motion; 
GravityEffect grav;
Entity test_entity({ 1.0e11f, 0.0f, 0.0f }, 1000.0f);

std::atomic<bool> EXIT{ false };

void listenForExit(std::atomic<bool>& exitFlag) {
    std::string input; 
    while (!exitFlag) {
        std::cin >> input; 
        if (input == "E" || input == "e") {
            exitFlag = true; 
        }
    }
}

int main() {
    std::cout << "Physics Engine starting...\n";
    
    //main engine loop 

    std::cout << "Building Universe...\n";
    std::vector<PlanetaryBody> planetary_bodies = Universe_init().buildPlanetaryBodies(JSONPATH);

    SimulationTime t;
    motion.updateAll(planetary_bodies, t);
	auto start = std::chrono::steady_clock::now();  
	std::thread inputThread(listenForExit, std::ref(EXIT)); //pass a reference so that the function listenForExit can modify the EXIT variable in the main thread
    t.previous = 0.0f;
    while (!EXIT) {
        auto now = std::chrono::steady_clock::now();
        t.current = std::chrono::duration<float>(now - start).count();

        const Vec3 previous_acceleration =
            grav.totalAccelerationAt(test_entity.get_pos(), planetary_bodies);

        motion.updateAll(planetary_bodies, t);
        grav.integrateVelocityVerlet(
            test_entity,
            planetary_bodies,
            previous_acceleration,
            t
        );

        t.previous = t.current;
    }

	inputThread.join(); // Wait for the input thread to finish

	auto stop = std::chrono::steady_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	std::cout << "Exiting Physics Engine...\n";
	std::cout << "Runtime = " + std::to_string(duration.count()) + " microseconds\n";
    return 0;
}
