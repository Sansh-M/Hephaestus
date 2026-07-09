cmake_minimum_required(VERSION 3.20)
project(PhysicsEngine VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Hephaestus src/main.cpp "src/constants.hpp" "src/Entities.cpp"   "src/Physics/atmos_physics.cpp" "src/Universe.cpp" "src/Engine_Init.cpp")

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(Hephaestus PRIVATE nlohmann_json::nlohmann_json)