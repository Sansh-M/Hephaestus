#include <iostream>
#include "Universe_init.hpp"
#include <chrono>
#include <atomic>
#include <thread>
#include "constants.hpp"
#include "Entities.hpp"
#include "Universe.hpp"

const std::string JSONPATH = "src/Data/Universe.json";

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

    SimulationTime t;

    std::cout << "Building Universe...\n";

	Universe universe{
		Universe_init().buildPlanetaryBodies(JSONPATH)
	};
	// Add a runtime entity to the collection owned and updated by Universe.
	universe.addEntity(Entity("test_entity", { 1.0e11f, 0.0f, 0.0f }, 1000.0f));

	// Establish planetary positions at the initial epoch before sampling gravity.
	universe.advance(t);

	auto start = std::chrono::steady_clock::now();  
	std::thread inputThread(listenForExit, std::ref(EXIT)); //pass a reference so that the function listenForExit can modify the EXIT variable in the main thread
    t.previous = 0.0f;

    while (!EXIT) {
        auto now = std::chrono::steady_clock::now();
        t.current = std::chrono::duration<float>(now - start).count();

		universe.advance(t);
        t.previous = t.current;
    }

	inputThread.join(); // Wait for the input thread to finish

	auto stop = std::chrono::steady_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	std::cout << "Exiting Physics Engine...\n";
	std::cout << "Runtime = " + std::to_string(duration.count()) + " microseconds\n";
    return 0;
}
