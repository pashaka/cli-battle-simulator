// Created by automated assistant
#pragma once

#include "ConcreteUnits.hpp"

#include <Core/Units/Unit.hpp>
#include <IO/Commands/SpawnHunter.hpp>
#include <IO/Commands/SpawnSwordsman.hpp>
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
            auto unit = std::make_unique<SwordsmanUnit>(cmd.strength);
        	unit->setName("Swordsman");
            unit->setId(cmd.unitId);
            unit->setHp(cmd.hp);
            unit->setPosition(Coordinate(static_cast<int32_t>(cmd.x), static_cast<int32_t>(cmd.y)));
            return unit;
        }

        static std::unique_ptr<Unit> create(const sw::io::SpawnHunter& cmd)
        {
            auto unit = std::make_unique<HunterUnit>(cmd.strength, cmd.agility, cmd.range);
        	unit->setName("Hunter");
            unit->setId(cmd.unitId);
            unit->setHp(cmd.hp);
            unit->setPosition(Coordinate(static_cast<int32_t>(cmd.x), static_cast<int32_t>(cmd.y)));
            return unit;
        }

    	static std::unique_ptr<Unit> create(const sw::io::SpawnMine& cmd)
        {
        	auto unit = std::make_unique<MineUnit>(cmd.power, cmd.triggerRange, cmd.explosionRange);
        	unit->setName("Mine");
        	unit->setId(cmd.unitId);
        	unit->setPosition(Coordinate(static_cast<int32_t>(cmd.x), static_cast<int32_t>(cmd.y)));
        	return unit;
        }

    	static std::unique_ptr<Unit> create(const sw::io::SpawnHealer& cmd)
        {
        	auto unit = std::make_unique<HealerUnit>(cmd.spirit, cmd.healRange);
        	unit->setName("Healer");
        	unit->setId(cmd.unitId);
        	unit->setHp(cmd.hp);
        	unit->setPosition(Coordinate(static_cast<int32_t>(cmd.x), static_cast<int32_t>(cmd.y)));
        	return unit;
        }
    };
}
