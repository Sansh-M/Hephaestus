cmake_minimum_required(VERSION 3.20)
project(PhysicsEngine VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Hephaestus
    src/main.cpp
    src/constants.hpp
    src/Entities.hpp
    
    src/PlanetaryBody.hpp
    src/UniversalFunctions.hpp
    src/Universe.cpp
    src/Universe_init.cpp
    src/Universe_init.hpp
    src/Physics/atmos_physics.cpp
    src/Physics/PlanetaryMotion.hpp
)

target_include_directories(Hephaestus PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Physics
)

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(Hephaestus PRIVATE nlohmann_json::nlohmann_json)