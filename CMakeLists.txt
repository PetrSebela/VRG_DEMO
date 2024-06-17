cmake_minimum_required(VERSION 3.8)
project(VRG_DEMO)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

Include(FetchContent)

FetchContent_Declare(
  EnTT
  GIT_REPOSITORY https://github.com/skypjack/entt
  GIT_TAG        v3.13.0 # or a later release
)

FetchContent_MakeAvailable(EnTT)

file(GLOB deps src/*.cpp)
add_executable(sim ${deps})

target_link_libraries(sim PRIVATE EnTT::EnTT)