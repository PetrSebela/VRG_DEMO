cmake_minimum_required(VERSION 3.8)
project(VRG_DEMO)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(libs/entt)

file(GLOB deps src/*.cpp)
add_executable(sim ${deps})

target_link_libraries(sim PRIVATE EnTT::EnTT)