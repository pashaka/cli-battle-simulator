cmake_minimum_required(VERSION 3.27)
project(sw_battle_test)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES src/*.cpp src/*.hpp)
add_executable(sw_battle_test ${SOURCES}
        src/Core/Units/Unit.hpp
        src/Core/Engine/Engine.cpp
        src/Core/Engine/Engine.hpp
        src/Core/Engine/MapUnitsController.cpp
        src/Core/Engine/MapUnitsController.hpp
        src/Core/Engine/Coordinate.cpp
        src/Core/Engine/Coordinate.hpp
        src/Core/Engine/Action.cpp
        src/Core/Engine/Action.hpp
        src/Core/Engine/Util.cpp
        src/Core/Engine/Util.hpp
)

target_include_directories(sw_battle_test PUBLIC src/)
