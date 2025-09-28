//
// Created by Carpov Pavel on 28.09.2025.
//

#include "ExplodingUnit.hpp"

#include "Core/Engine/MapUnitsController.hpp"
#include "IO/Events/UnitAttacked.hpp"
#include "IO/Events/UnitDied.hpp"
#include "IO/Events/UnitExploded.hpp"
#include "IO/System/EventLog.hpp"

namespace sw::core
{
	bool ExplodingUnit::tryToExecuteExplosion(const MapUnitsController& worldState)
	{
		if (!shouldExplodeNow()) // MineUnit will return true if triggered
		{
			return false;
		}
		std::vector<std::shared_ptr<Unit>> attackableUnits =
			worldState.getUnitsInRange(this->getPosition(), explosionRange);
		std::erase_if(attackableUnits, [](const auto& u) { return !u->canTakeDamage(); });

		if (attackableUnits.empty())
		{
			return false;
		}

		auto damage = power; // uint32_t
		// apply damage
		for (auto& targetUnit : attackableUnits)
		{
			targetUnit->increaseHp(-static_cast<int32_t>(damage));
			worldState.eventLog_.log(worldState.getCurrentTick(), sw::io::UnitAttacked{this->getId(), targetUnit->getId(), damage, targetUnit->getHp().value()});

			if (targetUnit->getHp().value() == 0)
			{
				worldState.eventLog_.log(worldState.getCurrentTick(), sw::io::UnitDied{targetUnit->getId()});
			}
		}
		uint32_t unitsHit = attackableUnits.size();
		worldState.eventLog_.log(worldState.getCurrentTick(), sw::io::UnitExploded{this->getId(), power, unitsHit});

		return true;
	}
}