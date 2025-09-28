//
// Created by Carpov Pavel on 29.09.2025.
//

#include "HealingUnit.hpp"

#include "Core/Engine/MapUnitsController.hpp"
#include "Core/Engine/Util.hpp"
#include "IO/Events/UnitHealed.hpp"
#include "IO/System/EventLog.hpp"

namespace sw::core
{
	bool HealingUnit::tryToExecuteHeal(const MapUnitsController& worldState)
	{
		std::vector<std::shared_ptr<Unit>> healableUnits =
			worldState.getUnitsInRange(this->getPosition(), healingRange);
		std::erase_if(healableUnits, [](const auto& u) { return !u->canTakeDamage(); });

		if (healableUnits.empty())
		{
			return false;
		}
		// heal a random unit
		uint32_t targetIndex = Util::randomInRange(static_cast<uint32_t>(healableUnits.size() - 1));
		auto targetUnit = healableUnits[targetIndex];
		targetUnit->increaseHp(this->spirit);
		// log UNIT_HEALED
		worldState.eventLog_.log(worldState.getCurrentTick(), sw::io::UnitHealed{this->getId(), targetUnit->getId(), this->spirit, targetUnit->getHp().value()});
		return true;
	}
}