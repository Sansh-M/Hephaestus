cmake_minimum_required(VERSION 3.20)
project(PhysicsEngine VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Hephaestus src/main.cpp "src/constants.hpp" "src/Entities.cpp" "src/PlanetaryBodies.cpp")