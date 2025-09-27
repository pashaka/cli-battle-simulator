// Created by automated assistant
#pragma once

#include <Core/Units/Unit.hpp>
#include <IO/Commands/SpawnSwordsman.hpp>
#include <IO/Commands/SpawnHunter.hpp>
#include <memory>

namespace sw::core
{
    // UnitFactory constructs and returns a new heap-allocated Unit (wrapped in unique_ptr)
    // initialized from spawn command data.
    class UnitFactory
    {
    public:
        static std::unique_ptr<Unit> create(const sw::io::SpawnSwordsman& cmd)
        {
            auto unit = std::make_unique<SwordsmanUnit>();
        	unit->setName("Swordsman");
            unit->setId(cmd.unitId);
            unit->setHp(cmd.hp);
            unit->setStrength(cmd.strength);
            unit->setPosition(Coordinate(static_cast<int32_t>(cmd.x), static_cast<int32_t>(cmd.y)));
            return unit;
        }

        static std::unique_ptr<Unit> create(const sw::io::SpawnHunter& cmd)
        {
            auto unit = std::make_unique<HunterUnit>();
        	unit->setName("Hunter");
            unit->setId(cmd.unitId);
            unit->setHp(cmd.hp);
            unit->setStrength(cmd.strength);
            unit->setAgility(cmd.agility);
            unit->setRange(cmd.range);
            unit->setPosition(Coordinate(static_cast<int32_t>(cmd.x), static_cast<int32_t>(cmd.y)));
            return unit;
        }
    };
}
