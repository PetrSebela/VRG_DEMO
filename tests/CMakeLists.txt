cmake_minimum_required(VERSION 3.8)
find_package(Catch2 3 REQUIRED)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(tests tests.cpp ../src/balistics.cpp ../src/transform.cpp ../src/vector3.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
