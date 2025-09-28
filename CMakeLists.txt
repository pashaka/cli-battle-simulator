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
        src/Core/Engine/Action.hpp
        src/Core/Engine/Util.cpp
        src/Core/Engine/Util.hpp
        src/Core/Units/MovingUnit.hpp
        src/Core/Units/AttackingUnit.hpp
        src/Core/Units/MovingUnit.cpp
        src/Core/Units/AttackingUnit.cpp
        src/Core/Units/TriggeredUnit.cpp
        src/Core/Units/TriggeredUnit.hpp
        src/Core/Units/ExplodingUnit.cpp
        src/Core/Units/ExplodingUnit.hpp
        src/Core/Units/HealingUnit.hpp
        src/IO/Commands/SpawnMine.hpp
        src/IO/Events/UnitExploded.hpp
        src/IO/Commands/SpawnHealer.hpp
        src/Core/Units/HealingUnit.cpp
        src/IO/Events/UnitHealed.hpp
        src/Core/Units/Action.cpp
)

target_include_directories(sw_battle_test PUBLIC src/)
